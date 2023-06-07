#pragma once

#include <vector>

struct Sample
{
    std::vector<float> Data;
    int FrameCount;
    bool Repeat;
    bool IsPlaying;
    float Volume;
};

enum class AudioType
{

};

namespace AudioManager
{
    void Init();
    void Play(AudioType audioType, bool repeat = false, float volume = 1.f);
    void Stop(AudioType audioType);
    void Pause(AudioType audioType);
    void Resume(AudioType audioType);
	void SetGlobalVolume(float volume);
}