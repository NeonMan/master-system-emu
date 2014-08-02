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
    //Variables

    ///PSG IO ports
    namespace bus{
        extern uint_fast8_t data;  ///<-- Data bus, 8 bit wide (Input)
        extern uint_fast8_t n_oe;  ///<-- ¬Output enable (Input)
        extern uint_fast8_t n_we;  ///<-- ¬Write enable (Input)
        extern uint_fast8_t ready; ///<-- Data read Ready (Output, open collector)
    }

    ///PSG emulator configuration
    namespace cfg{
    }

    ///PSG emulator state
    namespace state{
        extern int16_t next_sample; ///<-- Generated sample
    }

    ///PSG writeable registers
    namespace reg{
        extern int_fast8_t  vol[4];   ///<-- Atenuation (volume) registers
        extern uint_fast16_t tone[4]; ///<-- Tone registers. #3 is the Noise register
    }

    //Functions
    /**
    * @brief Add a clock cycle, return true if there's a new sample ready.
    *
    * @returns true if a new sample is ready.
    */
    bool tick();

    /**
    * @brief Configures the psg emulator for a sample rate.
    *
    * @param rate Sample rate in Hz
    */
    void set_sample_rate(uint_fast32_t rate);
};

#endif