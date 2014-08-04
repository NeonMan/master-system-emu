#include <cstdio>
#include "../psg/psg.h"

using namespace std;
//Generate a couple seconds of sound
int main(int argc, char *argv[]) {
    unsigned long sample_count = 0;
    unsigned long cycle_count = 0;
    const unsigned long rate = 22050;

    psg::set_sample_rate(rate);

    //Write 0 to the three first volume registers
    //Pull n_we low
    psg::bus::n_we = 0;
    for (int i = 0; i < 3; i++){
        psg::bus::data =
            (1 << 7) |  //<-- Set byte type to latch/data
            (i << 5) |  //<-- Select the register #
            (1 << 4) |  //<-- Select volume register
            (0);        //<-- The data (4-bit)
        psg::tick();
    }
    //Write the tone registers
    for (int i = 0; i < 3; i++){
        psg::bus::data =
            (1 << 7) | //<-- Set the type to latch/data (0 << 7)
            (i << 5) |  //<-- Select the register #
            //<-- Select tone/noise register (0 << 4)
            ((0xFE - 6*i) & 0xF); //<-- Write the low 4 bits.
        psg::tick();
        psg::bus::data = ((0xFE - 6*i)>>4) & 0x3F;
        psg::tick();
    }
    //Pull n_we back up
    psg::bus::n_we = 1;

    while (sample_count < (rate * 10)){
        ++cycle_count;
        if (psg::tick()){
            ++sample_count;
            putchar(*((char*)&(psg::state::next_sample) + 1));
            putchar(*(char*)&(psg::state::next_sample));
        }
    }
}
