#include "engine/Timer.h"

#include "MiniFB.h"

float deltaTime = 0.f;
int frames = 0;
float timeStack[200];
mfb_timer* timer;

namespace Timer
{
	void Init()
	{
		timer = mfb_timer_create();
		mfb_timer_reset(timer);
	}

	void Update()
	{
        if (frames == 200)
        {
            for (int i = 0; i < 200; i++)
            {
                if (i == 199)
                {
                    timeStack[i] = deltaTime;
                }
                else
                {
                    timeStack[i] = timeStack[i + 1];
                }
            }
        }

        if (frames < 200)
        {
            frames++;
        }

		deltaTime = mfb_timer_delta(timer);
        timeStack[frames - 1] = deltaTime;
	}

	float GetSmoothDeltaTime()
	{
		auto sum = 0.f;

        for (auto i : timeStack)
        {
            sum += i;
        }

        return sum / frames;
	}

    float GetDeltaTime()
    {
        return deltaTime;
    }
}
