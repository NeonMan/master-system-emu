#ifndef __PSG_H
#define __PSG_H

#include "psg_constants.h"
#include <stdint.h>

#ifdef _NDEBUG
#pragma comment(lib, "../Release/psg.lib")
#else
#pragma comment(lib, "../Debug/psg.lib")
#endif

namespace psg{
    namespace cfg{
    }
    namespace state{
        extern int16_t next_sample;
    }
    namespace reg{
        extern int_fast8_t  vol[4];
        extern uint_fast16_t tone[4];
    }
    bool tick();
    void set_sample_rate(uint_fast32_t rate);
};

#endif