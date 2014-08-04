#include <cstdio>
#include "../psg/psg.h"

using namespace std;
//Generate a couple seconds of sound
int main(int argc, char *argv[]) {
    unsigned long sample_count = 0;
    unsigned long cycle_count = 0;
    const unsigned long rate = 44100;

    psg::reg::vol[0] = 0;
    psg::reg::vol[1] = 0;
    psg::reg::vol[2] = 0;
    psg::reg::tone[0] = 0xFE - 12;
    psg::reg::tone[1] = 0xFE - 6;
    psg::reg::tone[2] = 0xFE;

    psg::set_sample_rate(rate);

    while (sample_count < (rate * 10)){
        ++cycle_count;
        if (psg::tick()){
            ++sample_count;
            putchar(*((char*)&(psg::state::next_sample) + 1));
            putchar(*(char*)&(psg::state::next_sample));
        }
    }
}
