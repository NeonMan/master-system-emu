#include <stdio.h>
#include <time.h>

#include "../z80/fake_z80.h"
#include "../psg/psg.h"

//Generate a couple seconds of sound
int main(int argc, char *argv[]) {
    unsigned long sample_count = 0;
    unsigned long cycle_count = 0;
    const unsigned long rate = 22050;

    set_sample_rate(rate);

    //Write 0 to the three first volume registers
    //Pull n_we low
    z80_n_wr = 0;
    //Pull IOREQ down
    z80_n_ioreq = 0;
    //Select the correct address (bit 7 high)
    z80_address = 1 << 6;

    for (int i = 0; i < 3; i++){
        z80_data = make_latch_data(i, 1, 0);
        psg_tick();
    }
    //Write the tone registers
    for (int i = 0; i < 3; i++){
        z80_data = make_latch_data(i, 0, 0xFE - 6 * i);
        psg_tick();
        z80_data = make_data(((0xFE - 6 * i) >> 4));
        psg_tick();
    }
    //Pull n_we back up
    z80_n_wr = 1;


    //Starting time
    time_t init_time;
    time(&init_time);
    while (sample_count < (rate * 100)){
        ++cycle_count;
        if (psg_tick()){
            ++sample_count;
            putchar(*((char*)&(psg_next_sample) + 1));
            putchar(*(char*)&(psg_next_sample));
        }
    }
    time_t end_time;
    time(&end_time);
    fprintf(stderr, "\r\n\r\n%d samples in %d ms\r\n\r\n", sample_count, end_time - init_time);
}
