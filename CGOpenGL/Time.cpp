#include "Time.h"


Time::Time()
{
}


Time::~Time()
{
}

/// <summary>
/// Takes the frame measurements for the next frames begin time
/// </summary>
void Time::NextFrame()
{
	++framecount;

	// Get the time from glfw
	lastFrameT = curFrameT;
	curFrameT = glfwGetTime();
	// Quick check for the first frame
	if( firstFrameT < 0 )
	{
		firstFrameT = curFrameT;
		lastFrameT = curFrameT;
	}

	// Update other time variables
	deltaFrameT = curFrameT - lastFrameT;
	sinceStartT = curFrameT - firstFrameT;
	fps = static_cast<double>(framecount) / sinceStartT;
	if( deltaFrameT > 0 )
	{
		fpsLast = 1.0 / deltaFrameT;
		fpsRunning = 0.9 * fpsRunning + 0.1 * (1.0 / deltaFrameT);
	}
}

/// <summary>
/// Gets the framecount.
/// </summary>
uint64_t Time::GetFramecount() const
{
	return framecount;
}

/// <summary>
/// Gets the delta.
/// </summary>
double Time::GetDelta() const
{
	return deltaFrameT;
}


/// <summary>
/// Gets the FPS over all frames.
/// </summary>
double Time::GetFps() const
{
	return fps;
}

/// <summary>
/// Gets the FPS running.
/// </summary>
double Time::GetFpsRunning() const
{
	return fpsRunning;
}

/// <summary>
/// Gets the FPS of only the last frame.
/// </summary>
/// <returns></returns>
double Time::GetFpsLast() const
{
	return fpsLast;
}
