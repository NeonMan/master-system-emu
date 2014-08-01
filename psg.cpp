#include "psg.h"
#include <stdint.h>

// ------------------------------------------
// --- Software implementation of SN79489 ---
// ------------------------------------------
using namespace std;
namespace psg{
    ///PSG emulator configuration
    namespace cfg{
        uint_fast32_t sample_rate = 22050;            ///<-- Output sample rate, in Hz
        const uint32_t psg_clock_ntsc = 3579545 / 16; ///<-- PSG clock (pal NTSC version)
        const uint32_t psg_clock_pal  = 3546893 / 16; ///<-- PSG clock (pal PAL version)
        uint_fast32_t psg_clock = psg_clock_ntsc;     ///<-- PSG clock
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
        int_fast8_t  vol[4] = { 0, 15, 15, 15 }; //Full volume.
        uint_fast16_t tone[4] = { 0xFE, 0xBE, 0xDE, 0xFE }; //440 Hz
    }

    ///Generate a sample from the registers (Signed 16bit)
    int16_t make_sample(){
        int16_t sample = 0;
        sample += state::channel_polarity[0] ? reg::vol[0] : -reg::vol[0];
        sample += psg::state::channel_polarity[1] ? psg::reg::vol[1] : -psg::reg::vol[1];
        sample += psg::state::channel_polarity[2] ? psg::reg::vol[2] : -psg::reg::vol[2];
        sample += psg::state::channel_polarity[3] ? psg::reg::vol[3] : -psg::reg::vol[3];
        return sample*128; //4bit volume would be too low, we multiply to make it audible
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