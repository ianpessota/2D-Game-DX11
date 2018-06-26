#ifndef _GAME_TIME_H_
#define _GAME_TIME_H_



#include "Windows.h"
class GameTime
{
public:
	GameTime();
	GameTime(int,bool);
	void Update(int, bool);

	double countsPerSecond = 0.0;
	__int64 CounterStart = 0;

	int frameCount = 0;
	float fps = 0;
	float frameRate = 0;

	//Frame Rate Lock 60(0.1666..) 30(0.333..), 100(0.01)
	float timeThreshold = 0;


	__int64 frameTimeOld = 0;
	double frameTime;

	void StartTimer();
	double GetTime();
	double GetFrameTime();
	float getFrameRate();

	bool lockFrameRate = false;

	~GameTime();
};

#endif // !_GAME_TIME_H_