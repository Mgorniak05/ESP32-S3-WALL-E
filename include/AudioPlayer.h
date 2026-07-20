#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <Arduino.h>
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceLittleFS.h"

class AudioPlayer {
    private:
        int pinBCLK;
        int pinLRC;
        int pinDIN;
        AudioGeneratorMP3 *mp3;
        AudioOutputI2S *out;
        AudioFileSourceLittleFS *file;

        public:
        AudioPlayer(int blck, int lrc, int din);
        void begin();
        void play(const char* filename);
        void loop();
        void stop();
        bool isPlaying();

};

#endif