#include "GameTime.h"


GameTime::GameTime()
{
}
GameTime::GameTime(int rate, bool lockfps)
{
	Update(rate, lockfps);
}


GameTime::~GameTime()
{
}
void GameTime::StartTimer()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	countsPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	CounterStart = frequencyCount.QuadPart;
}

double GameTime::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart - CounterStart) / countsPerSecond;
}

double GameTime::GetFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - frameTimeOld;
	frameTimeOld = currentTime.QuadPart;

	if (tickCount < 0.0f)
		tickCount = 0.0f;

	return float(tickCount) / countsPerSecond;
}
float GameTime::getFrameRate()
{
	float f;
	if (lockFrameRate)
		f = frameCount / (timeThreshold * 10000)*frameRate;
	else
		f = frameCount ;
	return f;
}
void GameTime::Update(int rate, bool lockfps)
{
	lockFrameRate = lockfps;

	if (lockFrameRate)
	{
		if (rate == 30)
		{
			timeThreshold = 0.03333f;
			frameRate = 0.5f;
		}

		else if (rate == 60)
		{
			timeThreshold = 0.0166666f;
			frameRate = 1.0f;
		}

		else if (rate == 120)
		{
			timeThreshold = 0.00833f;
			frameRate = 2;
		}

		else
		{
			lockFrameRate = false;
			timeThreshold = 0;
		}

	}
	else
	{
		timeThreshold = 0;
	}

}