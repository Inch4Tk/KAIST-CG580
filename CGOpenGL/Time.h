#pragma once

#include "StandardIncludes.h"

class Time
{
	friend class AppManager;
public:

	void NextFrame();

	uint64_t GetFramecount() const;
	double GetDelta() const;
	double GetFps() const;
	double GetFpsLast() const;
	double GetFpsRunning() const;

private:
	Time();
	~Time();

	double firstFrameT = -1.0;
	double lastFrameT = 0.0;
	double curFrameT = 0.0;
	double deltaFrameT = 0.0;
	double sinceStartT = 0.0;

	uint64_t framecount = 0;
	double fps = 0.0; // Over all frames
	double fpsLast = 0.0; // Just last frame
	double fpsRunning = 0.0; // Weights the last frame 0.5 of all frames
};

