#include <cstdio>
#include "psg.h"

using namespace std;
//Generate a couple seconds of sound
int main(int argc, char *argv[]) {
    unsigned long sample_count = 0;
    unsigned long cycle_count = 0;
    while (sample_count < (44100 * 10)){
        ++cycle_count;
        if (psg::tick()){
            ++sample_count;
            putchar(*((char*)&(psg::state::next_sample) + 1));
            putchar(*(char*)&(psg::state::next_sample));
        }
    }
}
