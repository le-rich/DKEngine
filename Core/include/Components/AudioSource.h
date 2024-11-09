#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H


class AudioSource
{

public:
    AudioSource(const char* soundName)
    {
        soundName = soundName;
    }

    void setIsPlaying(bool isPlaying)
    {
        isPlaying = isPlaying;
    }


private:
    char* soundName;
    bool isMute;
    int volume;
    int pitch;
    bool isPlaying;
    bool loop;    
};