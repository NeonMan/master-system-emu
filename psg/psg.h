#ifndef __PSG_H
#define __PSG_H

#include "psg_constants.h"
#include <stdint.h>

#ifdef NDEBUG
#pragma comment(lib, "../Release/psg.lib")
#else
#pragma comment(lib, "../Debug/psg.lib")
#endif

namespace psg{
    //Variables
    ///PSG IO ports.
    ///@Note since many circuits may share the data bus and/or other lines,
    ///a pointer to the shared bus variable might be a better option.
    namespace bus{
        extern uint_fast8_t* data;// = &bus_default::data;  ///<-- Data bus, 8 bit wide (Input)
        extern uint_fast8_t* n_oe;// = &bus_default::n_oe;  ///<-- ¬Chip enable (Input)
        extern uint_fast8_t* n_we;// = &bus_default::n_we;  ///<-- ¬Write enable (Input)
        extern uint_fast8_t* ready;// = &bus_default::ready; ///<-- Data read Ready (Output, open collector)
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
    * @brief Perform a clock cycle, return true if there's a new sample ready.
    *        This function bypasses the clock divider.
    *
    * @returns true if a new sample is ready.
    */
    bool tick();

    /**
    * @brief Perform a clock cycle, return true if a sample is ready.
    *
    * @returns true if a sample is ready.
    */
    bool clock();

    /**
    * @brief Configures the psg emulator for a sample rate.
    *
    * @param rate Sample rate in Hz
    */
    void set_sample_rate(uint_fast32_t rate);

    /**
    * @brief Generates a latch/data byte
    * 
    * @param channel      Which channel to latch [0-3]
    * @param volume_ntone If true, latch volume. Otherwise latch tone.
    * @param data         Data (4 bits)
    *
    * @return A byte ready to send to the PSG
    */
    inline uint_fast8_t make_latch_data(uint_fast8_t channel, uint_fast8_t volume_ntone, uint_fast8_t data){
        uint_fast8_t rv = 1 << 7;
        rv |= (channel & 0x3) << 5;
        rv |= volume_ntone ? (1 << 4) : 0;
        rv |= (data & 0x0F);
        return rv;
    }

    /**
    * @brief Generate a data byte 
    *
    * @param data Data (6 bits)
    *
    * @return A byte ready to send to the PSG
    */
    inline uint_fast8_t make_data(uint_fast8_t data){
        return (data & 0x3F);
    }
};

#endif