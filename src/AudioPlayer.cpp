#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(int blck, int lrc, int din){
    pinBCLK = blck;
    pinLRC = lrc;
    pinDIN = din;
    mp3 = new AudioGeneratorMP3();
    out = new AudioOutputI2S();
    file = nullptr;
}

void AudioPlayer::begin(){
    out->SetPinout(pinBCLK, pinLRC, pinDIN);
    out->SetGain(0.15);
}

void AudioPlayer::play(const char* filename){
    stop();
    file = new AudioFileSourceLittleFS(filename);
    mp3->begin(file, out);
}

void AudioPlayer::loop(){
    if (isPlaying()) {
        if (!mp3->loop())
            stop();
    }
}

void AudioPlayer::stop() {
    if (mp3 && mp3->isRunning()) {
        mp3->stop();
    }
    if (file) {
        delete file;
        file = nullptr;
    }
}

bool AudioPlayer::isPlaying(){
    return mp3 && mp3->isRunning();
}
