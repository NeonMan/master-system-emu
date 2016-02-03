// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 *  @file psg.c
 *  @brief PSG implementation.
 *
 *  Implements the SN79489 sound generator IC as used
 *  in the sega master system.
 */
#include "psg.h"
#include "psg_constants.h"
#include <stdint.h>

// --- Helper functions for talking to the circuit ---

uint8_t make_latch_data(uint8_t channel, uint8_t volume_ntone, uint8_t data){
    uint8_t rv = 1 << 7;
    rv |= (channel & 0x3) << 5;
    rv |= volume_ntone ? (1 << 4) : 0;
    rv |= (data & 0x0F);
    return rv;
}

uint8_t make_data(uint8_t data){
    return (data & 0x3F);
}

// ------------------------------------------
// --- Software implementation of SN79489 ---
// ------------------------------------------
#define VOLUME_L(x) psg_volume_table[vol[x]]

    //PSG emulator configuration
    uint32_t psg_sample_rate = 22050;            ///<-- Output sample rate, in Hz
    uint32_t psg_clock_f = PSG_CLOCK_NTSC;     ///<-- PSG clock
    
    //PSG emulator state
    //Next sample (see .h)
    int16_t psg_next_sample = 0;

    // --- Internal PSG state ---
        //uint32_t cycle = 0; ///<-- Current clock cycle

        ///Clock ratio between psg clock and sample rate *128
        uint32_t clock_ratio; // = (psg_clock_f << 7) / psg_sample_rate;
        
        ///Increased by 128 every clock cycle, if greater than clock_ratio,
        ///a new sample will be generated. The lower 7 bits are used for
        ///sub-sample calculations.
        uint32_t clock_current_ratio = 0;
        
        ///Current counter state for each channel
        uint16_t tone_counter[4] = { 1, 1, 1, 1 };
        
        ///Current channel polarity
        uint8_t channel_polarity[4] = { 1, 1, 1, 1 };

        ///Current LFSR position
        uint16_t lfsr_state = PSG_LFSR_RESET;

        ///Currently selected register for register writes
        uint8_t selected_reg = 0;
        uint8_t selected_reg_type = 0;

        ///Clock 16-divider state
        uint8_t divider_state = 0;
    // --------------------------

    // --- PSG writeable registers ---
        int8_t  vol[4] = { 15, 15, 15, 15 }; //Silent
        uint16_t tone[4] = { 0x3ff, 0x3ff, 0x3ff, 0x7 }; //Largest period
    // -------------------------------

    //PSG IO ports
    uint8_t psg_ready = 1; ///<-- Data read Ready (Output, open collector)

    // -----------------
    // --- Functions ---
    // -----------------

    /** @brief Generate a sample from the registers (Signed 16bit).
     *
     *  @returns A 16-bit unsigned sound sample.
     */
    int16_t make_sample(){
        int32_t sample = 0;
        //Tone channels
        sample += channel_polarity[0] ? VOLUME_L(0) : -VOLUME_L(0);
        sample += channel_polarity[1] ? VOLUME_L(1) : -VOLUME_L(1);
        sample += channel_polarity[2] ? VOLUME_L(2) : -VOLUME_L(2);

        //Noise
        if (tone[3] & 0x4){ //White noise
            sample += lfsr[lfsr_state % sizeof(lfsr)] ? VOLUME_L(3) : -VOLUME_L(3);
        }
        else{ //Pulsed noise
            const uint8_t duty = (channel_polarity[3] != 0);
            if (duty){
                sample += lfsr[lfsr_state % sizeof(lfsr)] ? VOLUME_L(3) : -VOLUME_L(3);
            }
            else{

            }
        }
        sample = sample / 8;
        return (int16_t)(sample); //Dividing by two (lose one bit) improves sound quality.
    }

    //Perform a clock cycle, without clock divider (See .h)
    uint8_t psg_tick(){
        //Read IO ports and update the data accordingly

        //For a write to be valid, Z80's IOREQ and WR must be low
        //And bit 7 on the address bus high
        if (!z80_n_wr && !z80_n_ioreq && (z80_address & (1<<6))){ //Write pin is pulled-down
            if (z80_data & (1 << 7)){ //It is a LATCH/DATA bit
                //Extract the selected reg
                const uint8_t param_reg = (z80_data & (3 << 5)) >> 5;
                //Get the type
                const uint8_t param_type = (z80_data & (1 << 4));
                //Get the 4-bit value
                const uint8_t param_value = (z80_data & 0xF);
                //Set the currently selected reg
                selected_reg = param_reg;
                selected_reg_type = param_type;

                if (param_type){ //Latch to volume register
                    vol[param_reg] = param_value;
                }
                else{ //Latch to tone register
                    tone[param_reg] = (param_reg == 3) ?
                        (param_value & 0x7) :
                        (tone[param_reg] & 0x3F0) | (param_value & 0xf);
                }
            }
            else{  //It is a DATA bit
                const uint16_t param_data = (z80_data & (0x3f));

                if (selected_reg_type){ //Volume register
                    const uint8_t  data_vol = param_data & 0xF;

                    vol[selected_reg] = data_vol;
                }
                else{ //Tone register
                    const uint16_t data_tone = param_data << 4;
                    const uint16_t  data_noise = param_data & 0x7;

                    switch (selected_reg){
                    case 0:
                    case 1:
                    case 2:
                        tone[selected_reg] = 
                            (tone[selected_reg] & 0x0F)
                            | data_tone;
                        break;
                    case 3:
                        tone[3] = data_noise;
                    }
                }

            }
        }
        //Update tone counters #[0-2]
        for (unsigned int i = 0; i < 3; i++){
            --tone_counter[i];
            if (tone_counter[i] == 0){
                channel_polarity[i] = channel_polarity[i] ?  0 : 1;
                tone_counter[i] = tone[i];
            }
        }

        //Update the noise channel (#3)
        --tone_counter[3];
        ++lfsr_state;
        if (tone_counter[3] == 0){
            channel_polarity[3] = channel_polarity[3] ? 0 : 1;
            switch (tone[3] & 0x3){
            case 0:
                tone_counter[3] = 0x10;
                break;
            case 1:
                tone_counter[3] = 0x20;
                break;
            case 2:
                tone_counter[3] = 0x40;
                break;
            case 3:
                tone_counter[3] = tone[2];
            }
        }

        //Update clock state
        //++cycle;
        clock_current_ratio += 128;

        //Generate sample, if needed
        if (clock_current_ratio >= clock_ratio){
            clock_current_ratio = (clock_current_ratio & 127) + (clock_ratio & 127);
            psg_next_sample = make_sample();
            return 1;
        }
        return 0;
    }

    //Configure sample rate (see .h)
    void psg_set_rate(uint32_t rate){
        psg_sample_rate = rate;
        clock_current_ratio = 0;
        clock_ratio = (psg_clock_f << 7) / rate;
    }

    void psg_io(){
        clock_current_ratio -= 128;
        psg_tick();
    }

    uint8_t* psgdbg_get_volume(){
        return vol;
    }

    uint16_t* psgdbg_get_tone(){
        return tone;
    }