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
    }

    ///PSG writeable registers
    namespace reg{
        int_fast8_t  vol[4] = { 15, 15, 0, 0 }; //Full volume.
        uint_fast16_t tone[4] = { 0, 0, 0xfe, 0xFE }; //440 Hz
    }

    ///Generate a sample from the registers (Signed 16bit)
    int16_t make_sample(){
        int_fast32_t sample = 0;
        sample += psg::state::channel_polarity[0] ? VOLUME_L(0) : -VOLUME_L(0);
        sample += psg::state::channel_polarity[1] ? VOLUME_L(1) : -VOLUME_L(1);
        sample += psg::state::channel_polarity[2] ? VOLUME_L(2) : -VOLUME_L(2);
        sample += psg::state::channel_polarity[3] ? VOLUME_L(3) : -VOLUME_L(3);
        sample = sample / 8;
        return (int16_t)(sample); //Dividing by two (lose one bit) improves sound quality.
    }

    
    ///Add a clock cycle, return true if there's a new sample ready.
    bool tick(){
        //Update tone counters #[1-3]
        for (unsigned int i = 1; i < 4; i++){
            --psg::state::tone_counter[i];
            if (psg::state::tone_counter[i] == 0){
                psg::state::channel_polarity[i] = psg::state::channel_polarity[i] ?  0 : 1;
                psg::state::tone_counter[i] = psg::reg::tone[i];
            }
        }

        //Update the noise channel (#0)
        //ToDo

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