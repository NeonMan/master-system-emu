#ifndef __PSG_H
#define __PSG_H

#include <stdint.h>

namespace psg{
    namespace cfg{
        extern uint_fast32_t sample_rate;
        extern const uint32_t psg_clock_ntsc;
        extern const uint32_t psg_clock_pal;
        extern uint_fast32_t psg_clock;
    }
    namespace state{
        extern uint_fast32_t cycle;
        extern int16_t next_sample;
        extern uint_fast32_t clock_ratio;
        extern uint_fast32_t clock_current_ratio;
        extern uint_fast16_t tone_counter[4];
        extern uint_fast8_t channel_polarity[4];
    }
    namespace reg{
        extern int_fast8_t  vol[4];
        extern uint_fast16_t tone[4];
    }
    bool tick();
};

#endif