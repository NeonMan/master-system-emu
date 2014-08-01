#include <cstdio>
#include "../psg/psg.h"

using namespace std;
//Generate a couple seconds of sound
int main(int argc, char *argv[]) {
    unsigned long sample_count = 0;
    unsigned long cycle_count = 0;
    const unsigned long rate = 44100;

    psg::reg::vol[3] = 0;
    psg::reg::tone[3] = 3;

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
