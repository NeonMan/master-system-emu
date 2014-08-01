#include "psg.h"
#include <stdint.h>

// ------------------------------------------
// --- Software implementation of SN79489 ---
// ------------------------------------------
#define VOLUME_L(x) psg::cfg::volume_table[psg::reg::vol[x]]
using namespace std;
namespace psg{
    ///PSG emulator configuration
    namespace cfg{
        uint_fast32_t sample_rate = 22050;            ///<-- Output sample rate, in Hz
        const uint32_t psg_clock_ntsc = 3579545 / 16; ///<-- PSG clock (pal NTSC version)
        const uint32_t psg_clock_pal  = 3546893 / 16; ///<-- PSG clock (pal PAL version)
        uint_fast32_t psg_clock = psg_clock_ntsc;     ///<-- PSG clock
        const  int volume_table[16] = {
            32767, 26028, 20675, 16422, 13045, 10362, 8231, 6568,
            5193, 4125, 3277, 2603, 2067, 1642, 1304, 0
        };
    }
    
    ///PSG emulator state
    namespace state{
        uint_fast32_t cycle = 0; ///<-- Current clock cycle
        int16_t next_sample = 0; ///<-- Generated sample

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
    }

    ///PSG writeable registers
    namespace reg{
        int_fast8_t  vol[4] = { 15, 15, 15, 15 }; //Silent
        uint_fast16_t tone[4] = { 0x3ff, 0x3ff, 0x3ff, 0x7 }; //Largest period
    }

    ///Generate a sample from the registers (Signed 16bit)
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
            const bool duty = psg::state::channel_polarity[3];
            if (duty){
                sample += psg::lfsr[psg::state::lfsr_state % sizeof(psg::lfsr)] ? VOLUME_L(3) : -VOLUME_L(3);
            }
            else{

            }
        }
        sample = sample / 8;
        return (int16_t)(sample); //Dividing by two (lose one bit) improves sound quality.
    }

    
    ///Add a clock cycle, return true if there's a new sample ready.
    bool tick(){
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
}