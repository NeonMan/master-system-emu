/**
 *  @file psg.h
 *  @brief PSG functions and variables.
 */
#ifndef __PSG_H
#define __PSG_H

#include <stdint.h>
#include "../z80/z80_externs.h"

#ifdef __cplusplus
extern "C" {
#endif

    //Variables

    //PSG IO ports.
    extern uint8_t psg_ready;   ///<-- Data read Ready (Output, open collector)

    //PSG emulator state
    extern int16_t psg_next_sample; ///<-- Generated sample

    //Functions
    /**
    * @brief Perform a clock cycle, return true if there's a new sample ready.
    *        This function bypasses the clock divider.
    *
    * @returns true if a new sample is ready.
    */
    uint8_t psg_tick();

    /**
     * @brief update PSG registers but don't increment timer towards a new sample. 
     */
    void psg_io();

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
    uint8_t make_latch_data(uint8_t channel, uint8_t volume_ntone, uint8_t data);

    /**
    * @brief Generate a data byte 
    *
    * @param data Data (6 bits)
    *
    * @return A byte ready to send to the PSG
    */
    uint8_t make_data(uint8_t data);

#ifdef __cplusplus
}
#endif
#endif