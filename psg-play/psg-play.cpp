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
    *psg::bus::n_we = 0;
    for (int i = 0; i < 3; i++){
        *psg::bus::data = psg::make_latch_data(i, 1, 0);
        psg::tick();
    }
    //Write the tone registers
    for (int i = 0; i < 3; i++){
        *psg::bus::data = psg::make_latch_data(i, 0, 0xFE - 6 * i);
        psg::tick();
        *psg::bus::data = psg::make_data(((0xFE - 6 * i) >> 4));
        psg::tick();
    }
    //Pull n_we back up
    *psg::bus::n_we = 1;

    while (sample_count < (rate * 10)){
        ++cycle_count;
        if (psg::tick()){
            ++sample_count;
            putchar(*((char*)&(psg::state::next_sample) + 1));
            putchar(*(char*)&(psg::state::next_sample));
        }
    }
}
