#include "psg.h"
#include <stdint.h>

// ------------------------------------------
// --- Software implementation of SN79489 ---
// ------------------------------------------
#define VOLUME_L(x) psg_volume_table[reg::vol[x]]
using namespace std;

    //PSG emulator configuration
    uint32_t psg_sample_rate = 22050;            ///<-- Output sample rate, in Hz
    uint32_t psg_clock = psg_clock_ntsc;     ///<-- PSG clock
    
    //PSG emulator state
    //Next sample (see .h)
    int16_t psg_next_sample = 0;
    namespace state{
        uint32_t cycle = 0; ///<-- Current clock cycle

        ///Clock ratio between psg clock and sample rate *128
        uint32_t clock_ratio = (psg_clock << 7) / psg_sample_rate;
        
        ///Increased by 128 every clock cycle, if greater than clock_ratio,
        ///a new sample will be generated. The lower 7 bits are used for
        ///sub-sample calculations.
        uint32_t clock_current_ratio = 0;
        
        ///Current counter state for each channel
        uint16_t tone_counter[4] = { 1, 1, 1, 1 };
        
        ///Current channel polarity
        uint8_t channel_polarity[4] = { 1, 1, 1, 1 };

        ///Current LFSR position
        uint16_t lfsr_state = lfsr_reset;

        ///Currently selected register for register writes
        uint8_t selected_reg = 0;
        uint8_t selected_reg_type = 0;

        ///Clock 16-divider state
        uint8_t divider_state = 0;
    }

    //PSG writeable registers
    namespace reg{
        int8_t  vol[4] = { 15, 15, 15, 15 }; //Silent
        uint16_t tone[4] = { 0x3ff, 0x3ff, 0x3ff, 0x7 }; //Largest period
    }

    //Default location of bus variables
    namespace bus_default{
        bool n_wr = 1;  // Write pins disabled by default
        uint8_t ready = 1;  // Always ready (this may change, hence being declared here)
    }

    //PSG IO ports
    uint8_t  z80_data;    ///<-- Data bus, 8 bit wide (Input)
    uint16_t z80_address; ///<-- Address bus (bit 7 up selects PSG)
    bool z80_n_wr = 1;  ///<-- ¬Write enable (Input)

    bool psg_ready = 1; ///<-- Data read Ready (Output, open collector)

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
        sample += state::channel_polarity[0] ? VOLUME_L(0) : -VOLUME_L(0);
        sample += state::channel_polarity[1] ? VOLUME_L(1) : -VOLUME_L(1);
        sample += state::channel_polarity[2] ? VOLUME_L(2) : -VOLUME_L(2);

        //Noise
        if (reg::tone[3] & 0x4){ //White noise
            sample += lfsr[state::lfsr_state % sizeof(lfsr)] ? VOLUME_L(3) : -VOLUME_L(3);
        }
        else{ //Pulsed noise
            const bool duty = (state::channel_polarity[3] != 0);
            if (duty){
                sample += lfsr[state::lfsr_state % sizeof(lfsr)] ? VOLUME_L(3) : -VOLUME_L(3);
            }
            else{

            }
        }
        sample = sample / 8;
        return (int16_t)(sample); //Dividing by two (lose one bit) improves sound quality.
    }

    //Perform a clock cycle, without clock divider (See .h)
    bool tick(){
        //Read IO ports and update the data accordingly
        if (!z80_n_wr){ //Write pin is pulled-down
            if (z80_data & (1 << 7)){ //It is a LATCH/DATA bit
                //Extract the selected reg
                const uint8_t param_reg = (z80_data & (3 << 5)) >> 5;
                //Get the type
                const uint8_t param_type = (z80_data & (1 << 4));
                //Get the 4-bit value
                const uint8_t param_value = (z80_data & 0xF);
                //Set the currently selected reg
                state::selected_reg = param_reg;
                state::selected_reg_type = param_type;

                if (param_type){ //Latch to volume register
                    reg::vol[param_reg] = param_value;
                }
                else{ //Latch to tone register
                    reg::tone[param_reg] = (param_reg == 3) ?
                        (param_value & 0x7) :
                        (reg::tone[param_reg] & 0x3F0) | (param_value & 0xf);
                }
            }
            else{  //It is a DATA bit
                const uint16_t param_data = (z80_data & (0x3f));

                if (state::selected_reg_type){ //Volume register
                    const uint8_t  data_vol = param_data & 0xF;

                    reg::vol[state::selected_reg] = data_vol;
                }
                else{ //Tone register
                    const uint16_t data_tone = param_data << 4;
                    const uint16_t  data_noise = param_data & 0x7;

                    switch (state::selected_reg){
                    case 0:
                    case 1:
                    case 2:
                        reg::tone[state::selected_reg] = 
                            (reg::tone[state::selected_reg] & 0x0F)
                            | data_tone;
                        break;
                    case 3:
                        reg::tone[3] = data_noise;
                    }
                }

            }
        }
        //Update tone counters #[0-2]
        for (unsigned int i = 0; i < 3; i++){
            --state::tone_counter[i];
            if (state::tone_counter[i] == 0){
                state::channel_polarity[i] = state::channel_polarity[i] ?  0 : 1;
                state::tone_counter[i] = reg::tone[i];
            }
        }

        //Update the noise channel (#3)
        --state::tone_counter[3];
        ++state::lfsr_state;
        if (state::tone_counter[3] == 0){
            state::channel_polarity[3] = state::channel_polarity[3] ? 0 : 1;
            switch (reg::tone[3] & 0x3){
            case 0:
                state::tone_counter[3] = 0x10;
                break;
            case 1:
                state::tone_counter[3] = 0x20;
                break;
            case 2:
                state::tone_counter[3] = 0x40;
                break;
            case 3:
                state::tone_counter[3] = reg::tone[2];
            }
        }

        //Update clock state
        ++state::cycle;
        state::clock_current_ratio += 128;

        //Generate sample, if needed
        if (state::clock_current_ratio >= state::clock_ratio){
            state::clock_current_ratio = (state::clock_current_ratio & 127) + (state::clock_ratio & 127);
            psg_next_sample = make_sample();
            return true;
        }
        return false;
    }

    //Configure sample rate (see .h)
    void set_sample_rate(uint32_t rate){
        psg_sample_rate = rate;
        state::clock_current_ratio = 0;
        state::clock_ratio = (psg_clock << 7) / rate;
    }

    //Preform a clock cycle (with clock divider) (see .h)
    bool clock(){
        ++state::divider_state;
        if (state::divider_state == 16){
            state::divider_state = 0;
            return tick();
        }
        return false;
    }