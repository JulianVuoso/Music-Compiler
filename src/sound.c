#include <math.h>
#include "sound.h"

void generateWav(Score score) {
    long duration = getTotalDuration(score);
    long nSamples = getSampleNumber(duration);
    
    // Create a mono (1), 16-bit sound and set the duration
    Wave mySound = makeWave((int)SAMPLE_RATE, CHANNEL_NUM, BITS_SAMPLE);
    waveSetDuration(&mySound, duration);

    // Add all of the data
    addScore(&mySound, score);

    // Write it to a file and clean up when done
    waveToFile(&mySound, WAV_FILE_NAME);
    waveDestroy(&mySound);
}

void addScore(Wave * mySound, Score score) {
    for (int i = 0; i < score.size; i++) {
        addChord(mySound, score.sets[i].chord, getSampleNumber(score.sets[i].time));
    }
}

void addChord(Wave * mySound, Chord chord, long nSamples) {
    float multiplier = M_PI * 2.0 / SAMPLE_RATE;
    float frameData[CHANNEL_NUM];
    int i, j, k;
    for(i = 0; i < nSamples; i++){
        for (j = 0; j < chord.size; j++) {
            for (k = 0; k < CHANNEL_NUM; k++) {
                frameData[k] = cos(chord.freq[j] * (float)i * multiplier) / chord.size;
            }
        }
        waveAddSample(mySound, frameData);
    }
}

long getTotalDuration(Score score) {
    long rta = 0;
    for (int i = 0; i < score.size; i++) {
        rta += score.sets[i].time;
    }
    return rta;
}

long getSampleNumber(long miliseconds) {
    return (long) ((miliseconds / 1000.0) * SAMPLE_RATE);
}