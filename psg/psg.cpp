#include "psg.h"
#include <stdint.h>

// ------------------------------------------
// --- Software implementation of SN79489 ---
// ------------------------------------------
#define VOLUME_L(x) psg::volume_table[psg::reg::vol[x]]
using namespace std;
namespace psg{

    //PSG emulator configuration
    namespace cfg{
        uint_fast32_t sample_rate = 22050;            ///<-- Output sample rate, in Hz
        uint_fast32_t psg_clock = psg_clock_ntsc;     ///<-- PSG clock
    }
    
    //PSG emulator state
    namespace state{
        uint_fast32_t cycle = 0; ///<-- Current clock cycle
        //Next sample (see .h)
        int16_t next_sample = 0;

        ///Clock ratio between psg clock and sample rate *128
        uint_fast32_t clock_ratio = (cfg::psg_clock << 7) / cfg::sample_rate;
        
        ///Increased by 128 every clock cycle, if greater than clock_ratio,
        ///a new sample will be generated. The lower 7 bits are used for
        ///sub-sample calculations.
        uint_fast32_t clock_current_ratio = 0;
        
        ///Current counter state for each channel
        uint_fast16_t tone_counter[4] = { 1, 1, 1, 1 };
        
        ///Current channel polarity
        uint_fast8_t channel_polarity[4] = { 1, 1, 1, 1 };

        ///Current LFSR position
        uint16_t lfsr_state = psg::lfsr_reset;

        ///Currently selected register for register writes
        uint_fast8_t selected_reg = 0;
        uint_fast8_t selected_reg_type = 0;

        ///Clock 16-divider state
        uint_fast8_t divider_state = 0;
    }

    //PSG writeable registers
    namespace reg{
        int_fast8_t  vol[4] = { 15, 15, 15, 15 }; //Silent
        uint_fast16_t tone[4] = { 0x3ff, 0x3ff, 0x3ff, 0x7 }; //Largest period
    }

    //Default location of bus variables
    namespace bus_default{
        uint_fast8_t data = 0x00;
        uint_fast8_t n_oe = 1;  //
        uint_fast8_t n_we = 1;  // Write pins disabled by default
        uint_fast8_t ready = 1;  // Always ready (this may change, hence being declared here)
    }

    //PSG IO ports
    namespace bus{
        uint_fast8_t* data = &bus_default::data;  ///<-- Data bus, 8 bit wide (Input)
        uint_fast8_t* n_oe = &bus_default::n_oe;  ///<-- ¬Chip enable (Input)
        uint_fast8_t* n_we = &bus_default::n_we;  ///<-- ¬Write enable (Input)
        uint_fast8_t* ready = &bus_default::ready; ///<-- Data read Ready (Output, open collector)
    }

    // -----------------
    // --- Functions ---
    // -----------------

    /** @brief Generate a sample from the registers (Signed 16bit).
     *
     *  @returns A 16-bit unsigned sound sample.
     */
    int16_t make_sample(){
        int_fast32_t sample = 0;
        //Tone channels
        sample += psg::state::channel_polarity[0] ? VOLUME_L(0) : -VOLUME_L(0);
        sample += psg::state::channel_polarity[1] ? VOLUME_L(1) : -VOLUME_L(1);
        sample += psg::state::channel_polarity[2] ? VOLUME_L(2) : -VOLUME_L(2);

        //Noise
        if (psg::reg::tone[3] & 0x4){ //White noise
            sample += psg::lfsr[psg::state::lfsr_state % sizeof(psg::lfsr)] ? VOLUME_L(3) : -VOLUME_L(3);
        }
        else{ //Pulsed noise
            const bool duty = (psg::state::channel_polarity[3] != 0);
            if (duty){
                sample += psg::lfsr[psg::state::lfsr_state % sizeof(psg::lfsr)] ? VOLUME_L(3) : -VOLUME_L(3);
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
        if (*psg::bus::n_we == 0){ //Write pin is pulled-down
            if (*psg::bus::data & (1 << 7)){ //It is a LATCH/DATA bit
                //Extract the selected reg
                const uint_fast8_t param_reg = (*psg::bus::data & (3 << 5)) >> 5;
                //Get the type
                const uint_fast8_t param_type = (*psg::bus::data & (1 << 4));
                //Get the 4-bit value
                const uint_fast8_t param_value = (*psg::bus::data & 0xF);
                //Set the currently selected reg
                psg::state::selected_reg = param_reg;
                psg::state::selected_reg_type = param_type;

                if (param_type){ //Latch to volume register
                    psg::reg::vol[param_reg] = param_value;
                }
                else{ //Latch to tone register
                    psg::reg::tone[param_reg] = (param_reg == 3) ?
                        (param_value & 0x7) :
                        (reg::tone[param_reg] & 0x3F0) | (param_value & 0xf);
                }
            }
            else{  //It is a DATA bit
                const uint_fast16_t param_data = (*psg::bus::data & (0x3f));

                if (psg::state::selected_reg_type){ //Volume register
                    const uint_fast8_t  data_vol = param_data & 0xF;

                    psg::reg::vol[psg::state::selected_reg] = data_vol;
                }
                else{ //Tone register
                    const uint_fast16_t data_tone = param_data << 4;
                    const uint_fast16_t  data_noise = param_data & 0x7;

                    switch (psg::state::selected_reg){
                    case 0:
                    case 1:
                    case 2:
                        psg::reg::tone[psg::state::selected_reg] = 
                            (psg::reg::tone[psg::state::selected_reg] & 0x0F)
                            | data_tone;
                        break;
                    case 3:
                        psg::reg::tone[3] = data_noise;
                    }
                }

            }
        }
        //Update tone counters #[0-2]
        for (unsigned int i = 0; i < 3; i++){
            --psg::state::tone_counter[i];
            if (psg::state::tone_counter[i] == 0){
                psg::state::channel_polarity[i] = psg::state::channel_polarity[i] ?  0 : 1;
                psg::state::tone_counter[i] = psg::reg::tone[i];
            }
        }

        //Update the noise channel (#3)
        --psg::state::tone_counter[3];
        ++psg::state::lfsr_state;
        if (psg::state::tone_counter[3] == 0){
            psg::state::channel_polarity[3] = psg::state::channel_polarity[3] ? 0 : 1;
            switch (psg::reg::tone[3] & 0x3){
            case 0:
                psg::state::tone_counter[3] = 0x10;
                break;
            case 1:
                psg::state::tone_counter[3] = 0x20;
                break;
            case 2:
                psg::state::tone_counter[3] = 0x40;
                break;
            case 3:
                psg::state::tone_counter[3] = psg::reg::tone[2];
            }
        }

        //Update clock state
        ++psg::state::cycle;
        psg::state::clock_current_ratio += 128;

        //Generate sample, if needed
        if (psg::state::clock_current_ratio >= psg::state::clock_ratio){
            psg::state::clock_current_ratio = (psg::state::clock_current_ratio & 127) + (psg::state::clock_ratio & 127);
            psg::state::next_sample = make_sample();
            return true;
        }
        return false;
    }

    //Configure sample rate (see .h)
    void set_sample_rate(uint_fast32_t rate){
        psg::cfg::sample_rate = rate;
        psg::state::clock_current_ratio = 0;
        psg::state::clock_ratio = (cfg::psg_clock << 7) / rate;
    }

    //Preform a clock cycle (with clock divider) (see .h)
    bool clock(){
        ++psg::state::divider_state;
        if (psg::state::divider_state == 16){
            psg::state::divider_state = 0;
            return tick();
        }
        return false;
    }
}