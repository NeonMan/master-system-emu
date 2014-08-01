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
        uint_fast32_t psg_clock = psg_clock_pal;      ///<-- PSG clock
    }
    
    ///PSG emulator state
    namespace state{
        uint_fast32_t cycle = 0; ///<-- Current clock cycle
        int16_t next_sample = 128; ///<-- Generated sample
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
        uint_fast8_t  vol[4] = { 15, 15, 15, 15 }; //Full volume.
        uint_fast16_t tone[4] = { 0xFE, 0xFE, 0xFE, 0xFE }; //440 Hz
    }

    ///Generate a sample from the registers
    inline int16_t make_sample(){
        const int16_t sample = (
            //state::channel_polarity[0] ? reg::vol[0] : -reg::vol[0] +
            state::channel_polarity[1] ? reg::vol[1] : -reg::vol[1] +
            state::channel_polarity[2] ? reg::vol[2] : -reg::vol[2] +
            state::channel_polarity[3] ? reg::vol[3] : -reg::vol[3]
            );
        return sample;
    }
    
    ///Add a clock cycle, return true if there's a new sample ready.
    bool tick(){
        //Update tone counters
        for (unsigned int i = 1; i < 4; i++){
            --state::tone_counter[i];
            if (state::tone_counter[i] == 0){
                state::channel_polarity[i] ^= 1;
                state::tone_counter[i] = reg::tone[i];
            }
        }

        //Update the noise channel
        //ToDo

        //Update clock state
        ++state::cycle;
        state::clock_current_ratio += 128;

        //Generate sample, if needed
        if (state::clock_current_ratio >= state::clock_ratio){
            state::clock_current_ratio = (state::clock_current_ratio & 127) + (state::clock_ratio & 127);
            state::next_sample = make_sample();
            return true;
        }
        return false;
    }
}