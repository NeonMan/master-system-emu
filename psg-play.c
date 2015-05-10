// Copyright 2015 Juan Luis Álvarez Martínez
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "z80/fake_z80.h"
#include "psg/psg.h"

#define SAMPLE_RATE  22050
#define SAMPLE_COUNT (SAMPLE_RATE * 1)
#define SAMPLES_PER_CHUNK 1024

volatile int playing;
int sample_count = 0;

//stream: Where the samples are written.
//   len: Stream size in bytes.
void audio_callbackf(void *userdata, Uint8 * stream, int len){
    //Fill the stream buffer. In samples:
    //    len / bytes_per_sample
    //bytes_per_sample is 2.
    for (int i = 0; i < len; i+=2){
        while (!psg_tick()){}
        *((int16_t*)(stream + i)) = psg_next_sample;
    }

    sample_count += SAMPLES_PER_CHUNK;
    if (sample_count > SAMPLE_COUNT)
        playing = 0;
}

int main(int argc, char** argv){
    // --- Initializing ---
    const unsigned long rate = SAMPLE_RATE;

    printf("Available audio drivers.\n");
    for (int i = 0; i < SDL_GetNumAudioDrivers(); i++){
        printf("%d: %s\n", i, SDL_GetAudioDriver(i));
    }

    SDL_Init(SDL_INIT_AUDIO);
    //SDL_AudioInit("disk"); //Uncomment to dump raw samples to sdlaudio.raw

    SDL_AudioSpec audio_want, audio_set;
    SDL_zero(audio_want);
    audio_want.freq = rate;
    audio_want.channels = 1;
    audio_want.format = AUDIO_S16SYS;
    audio_want.samples = SAMPLES_PER_CHUNK;
    audio_want.callback = audio_callbackf;
    SDL_AudioDeviceID audio_dev;
    audio_dev = SDL_OpenAudioDevice(NULL, 0, &audio_want, &audio_set, 0);
    if (audio_dev == 0){
        printf("Unable to configure audio device.\n");
        exit(-1);
    }
    playing = 1;

    // --- Setup PSG ---
    psg_set_rate(SAMPLE_RATE);
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

    // --- Playing ---
    SDL_PauseAudioDevice(audio_dev, 0);

    while (playing)
        SDL_Delay(100);

    // --- Cleanup --.
    SDL_CloseAudioDevice(audio_dev);
    SDL_Quit();
    return 0;
}
