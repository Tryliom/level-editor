#include "engine/AudioManager.h"

#include <cassert>
#include <map>
#include <iostream>

#define SOKOL_IMPL
#include "sokol_log.h"
#include "sokol_audio.h"

#include "stb_vorbis.c"
#include "stb_vorbis.h"

float volume = 1.f;
std::vector<Sample> audioBuffers = {};
std::map<AudioType, Sample> audioSamples = {};

void audioCallback(float* buffer, int numFrames, int numChannels)
{
    assert(numChannels == 1);

    for (auto frame = 0; frame < numFrames; frame++)
    {
        float currentSample = 0.f;

        for (auto i = 0; i < audioBuffers.size(); i++)
        {
            auto& audioBuffer = audioBuffers[i];

            if (!audioBuffer.IsPlaying) continue;

            currentSample += audioBuffer.Data[audioBuffer.FrameCount] * audioBuffer.Volume;
            audioBuffer.FrameCount++;

            if (audioBuffer.FrameCount >= audioBuffer.Data.size())
            {
                if (audioBuffer.Repeat)
                {
                    audioBuffer.FrameCount = 0;
                }
                else
                {
                    // Remove audio buffer
                    audioBuffers.erase(audioBuffers.begin() + i);
                    i--;
                }
            }
        }

        buffer[frame] = currentSample * volume;
    }
}

float* getSamples(stb_vorbis* vorbis, int channels, int sampleCount)
{
    auto* samples = new float[sampleCount];
    auto sampleIndex = 0;

    while (true)
    {
        auto readSamples = stb_vorbis_get_samples_float_interleaved(vorbis, channels, samples + sampleIndex, sampleCount - sampleIndex);
        sampleIndex += readSamples * channels;

        // Add all channels together
        for (auto i = 0; i < readSamples; i++)
        {
            auto sampleValue = 0.f;
            for (auto channel = 0; channel < channels; channel++)
            {
                sampleValue += samples[i * channels + channel];
            }
            samples[i] = sampleValue / (float) channels;
        }

        if (readSamples == 0) break;
    }

    return samples;
}

Sample loadSample(const char* path)
{
    // Get sample data from ogg file
    stb_vorbis* vorbis = stb_vorbis_open_filename(path, nullptr, nullptr);
    if (vorbis == nullptr)
    {
        std::cout << "Failed to load sample: " << path << std::endl;
        return {};
    }

    stb_vorbis_info info = stb_vorbis_get_info(vorbis);
    auto channels = info.channels;
    auto sampleCount = (int) stb_vorbis_stream_length_in_samples(vorbis) * channels;
    auto* samples = getSamples(vorbis, channels, sampleCount);

    stb_vorbis_close(vorbis);

    return { std::vector<float>(samples, samples + sampleCount) };
}

namespace AudioManager
{
    void Init()
    {
        saudio_setup((saudio_desc)
        {
            .stream_cb = audioCallback,
            .logger =
            {
                .func = slog_func
            }
        });

        // Load samples
    }

    void Play(AudioType audioType, bool repeat, float volume)
    {
        audioBuffers.push_back(audioSamples[audioType]);

        auto& audioBuffer = audioBuffers[audioBuffers.size() - 1];

        audioBuffer.Repeat = repeat;
        audioBuffer.Volume = volume;
        audioBuffer.IsPlaying = true;
    }

    void Stop(AudioType audioType)
    {
        for (auto i = 0; i < audioBuffers.size(); i++)
        {
            auto& audioBuffer = audioBuffers[i];
            if (audioBuffer.Data == audioSamples[audioType].Data)
            {
                audioBuffers.erase(audioBuffers.begin() + i);
                i--;
            }
        }
    }

    void Pause(AudioType audioType)
    {
        for (auto & audioBuffer : audioBuffers)
        {
            if (audioBuffer.Data == audioSamples[audioType].Data)
            {
                audioBuffer.IsPlaying = false;
            }
        }
    }

    void Resume(AudioType audioType)
    {
        for (auto & audioBuffer : audioBuffers)
        {
            if (audioBuffer.Data == audioSamples[audioType].Data)
            {
                audioBuffer.IsPlaying = true;
            }
        }
    }

	void SetGlobalVolume(float volume)
	{
		volume = volume;
	}
}