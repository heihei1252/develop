#include <stdio.h>
#include <unistd.h>

#include "WaveGenerator.h"

#define DEFAULT_STRING "01016403e803e801f401f401f401f401f401f401f401f401f401f401f403e801f401f401f401f401f403e801f403e801f401f401f403e801f401f401f401f401f401f401f401f401f401f401f403e801f401f401f403e801f401f401f401f401f403e801f401f401f403e801f401f401f401f401f403e801f403e801f401f401f403e801f401f401f401f401f401f401f403e801f403e801f401f401f401f401f403e801f401f401f401f401f401f401f401f401f403e801f403e801f401f401f403e801f401f401f403e801f403e801f403e801f403e801f403e801f401f401f403e801f401f401f403e801f403e801f403e801f401f401f401f401f401f401f403e801f401f401f403e801f401f401f403e801f401f401f403e801f401f401f403e801f401f401f403e801f401f401f403e801f401f401f401f401f401f401f403e801f401f401f403e801f403e801f401f401f401f401f403e801f401f401f403e801f401f401f401f401f401f401f403e801f401f401f403e801f401f401f403e801f401f401f401f401f403e801f401f401f401f401f403e801f403e801f401f401f401f401f403e801f401f401f401f401f403e801f401f401f401f401f401f401f401f401f403e801f401f401f401f401f401f401f403e801f403e801f401f401f401f401f401f401f403e801f401f401f401f401f403e801f403e801f401f401f401f401f403e801f403e801f401f401f401f401f403e801f403e801f401f401f401f401f401f401f401f401f403e801f401f401f403e801f403e801f401f401f401f401f403e801f401f401f403e801f401f401f403e801f403e801f401f401f401f401f401f401f403e801f403e801f401f401f403e801f403e801f401f401f401f401f403e801f403e801f403e801f401f401f403e801f403e801f401f401f401f401f401f401f401f401f401f401f401f401f401f401f401f401f401f401f401f407081388"

#define DEFAULT_FILENAME "/tmp/tmpWave.wav"

int main(int argc, char *argv[])
{
    WaveGenerator wg(DEFAULT_STRING, 20000/*20KHZ*/);
    std::vector<uint8_t> wavVec;
    FILE *fp = NULL;
    
    wavVec.clear();
    wavVec = wg.getWaveData();
    
    fp = fopen(DEFAULT_FILENAME, "wb+");
    if (fp == NULL){
        printf("fopen error.\n");
        return -1;
    }

    for(vector<uint8_t>::iterator iter = wavVec.begin(); iter != wavVec.end(); iter++) {
        uint8_t data = *iter;
        fwrite(&data, 1, 1, fp);
    }

    fflush(fp);
    fclose(fp);
    return 0;
}
