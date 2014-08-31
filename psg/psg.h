#ifndef __PSG_H
#define __PSG_H

#include "psg_constants.h"
#include <stdint.h>

#ifdef NDEBUG
#pragma comment(lib, "../Release/psg.lib")
#else
#pragma comment(lib, "../Debug/psg.lib")
#endif

    //Variables
    //PSG IO ports.
    //@Note since many circuits may share the data bus and/or other lines,
    //a pointer to the shared bus variable might be a better option.
    extern uint8_t  z80_data;    ///<-- Data bus, 8 bit wide (Input)
    extern uint16_t z80_address; ///<-- Address bus (bit 7 up selects PSG)
    extern bool     z80_n_wr;    ///<-- !Write enable (Input)
    extern bool     psg_ready;   ///<-- Data read Ready (Output, open collector)

    //PSG emulator state
    extern int16_t psg_next_sample; ///<-- Generated sample

    ///PSG writeable registers
    //extern int8_t  vol[4];   ///<-- Atenuation (volume) registers
    //extern uint16_t tone[4]; ///<-- Tone registers. #3 is the Noise register

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
    void set_sample_rate(uint32_t rate);

    /**
    * @brief Generates a latch/data byte
    * 
    * @param channel      Which channel to latch [0-3]
    * @param volume_ntone If true, latch volume. Otherwise latch tone.
    * @param data         Data (4 bits)
    *
    * @return A byte ready to send to the PSG
    */
    inline uint8_t make_latch_data(uint8_t channel, uint8_t volume_ntone, uint8_t data){
        uint8_t rv = 1 << 7;
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
    inline uint8_t make_data(uint8_t data){
        return (data & 0x3F);
    }

#endif