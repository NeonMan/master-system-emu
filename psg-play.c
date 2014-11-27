#include <stdio.h>
#include <time.h>

#include "z80/fake_z80.h"
#include "psg/psg.h"

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
    FILE* out_f = fopen("psg-play.wav", "wb");

    //Write WAV header
    fwrite("RIFF", 4, 1, out_f);
    fputc(0xbc, out_f);
    fputc(0x4a, out_f);
    fputc(0x43, out_f);
    fputc(0x00, out_f); //File size - 8.
    fwrite("WAVE", 4, 1, out_f);
    //Format chunk
    fwrite("fmt ", 4, 1, out_f); //ID
    fputc(0x10, out_f);
    fputc(0x00, out_f);
    fputc(0x00, out_f);
    fputc(0x00, out_f); //Size
    fputc(0x01, out_f);
    fputc(0x00, out_f); //Compression, PCM
    fputc(0x01, out_f);
    fputc(0x00, out_f); //Channels, 1
    fputc(0x22, out_f);
    fputc(0x56, out_f);
    fputc(0x00, out_f);
    fputc(0x00, out_f);//sample rate
    fputc(0x44, out_f);
    fputc(0xAC, out_f);
    fputc(0x00, out_f);
    fputc(0x00, out_f);//bitrate
    fputc(0x02, out_f);
    fputc(0x00, out_f);//bytes per sample
    fputc(0x10, out_f);
    fputc(0x00, out_f);//significant bits

    //All the samples in one huge chunk
    fwrite("data", 4, 1, out_f);
    fputc(0x90, out_f);
    fputc(0x4a, out_f);
    fputc(0x43, out_f);
    fputc(0x00, out_f);//size

    while (sample_count < (rate * 100)){
        ++cycle_count;
        if (psg_tick()){
            ++sample_count;
            fputc(*(char*)&(psg_next_sample), out_f);
            fputc(*((char*)&(psg_next_sample)+1), out_f);
        }
    }
    fclose(out_f);
    time_t end_time;
    time(&end_time);
    fprintf(stderr, "\r\n\r\n%lu samples in %ld ms\r\n\r\n", sample_count, (int)(end_time - init_time));
}
