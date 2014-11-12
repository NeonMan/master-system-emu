#include "vdp.h"
#include <string.h>

struct vdp_s vdp;

void vdp_init(){
    memset(&vdp, 0, sizeof(struct vdp_s));
}

void vdp_io(){
    //Check the IO ports (Address/data/IOreq)

    //If not IO, just return
    if (z80_n_ioreq)
        return;

    //Test bits 7,6 and 0
    switch (z80_address & ((3 << 6) | 1)){
    // --- If Address' bit 7 is set and 6 is not. Address is in range [0x80-0xBF]
    case ((1 << 7) | 1) : //Odd address is control port
        return;
    case ((1 << 7) | 0) : //Even address is data port
        return;
    // --- If Address' bit 6 is set and 7 is not. Address is in range [0x40-0x7F]
    case ((1 << 6) | 1): //Odd address H counter (read)
        if (z80_n_rd) //Only allow reads
            return;
        z80_data = vdp.h;
        return;
    case ((1 << 6) | 0) : //Even address V counter (read)
        if (z80_n_rd) //Only allow reads
            return;
        z80_data = vdp.v;
        return;
    // --- Otherwise, ignore.
    default:
        return;
    }
}

void vdp_tick(){
    vdp_io(); //Perform read/writes if needed
}