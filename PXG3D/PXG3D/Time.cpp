#include "Time.h"
#include "Debug.h"

namespace PXG
{


	Time::Time(int frameTickStoredCount, float firstFrameEstimate)
	{
		averageDeltaTick = 0;

		this->frameTickStoredCount = frameTickStoredCount;

		timePassed = glfwGetTime();

		float estimatedTick = 1.0f / firstFrameEstimate;

		for (int i = 0; i < frameTickStoredCount; i++)
		{
			averageTimes.push_front(estimatedTick);
		}


		calculateAverageDeltaTick();

	}

	Time::~Time()
	{
	}

	float Time::GetTime()
	{
		return glfwGetTime();
	}



	float Time::GetAverageDeltaTime()
	{
		return averageDeltaTick;
	}

	void Time::UpdateTimePassed()
	{
		timePassed = glfwGetTime();
	}
	float Time::GetCurrentDeltaTime()
	{
		return glfwGetTime() - timePassed;
	}
	void Time::UpdateAverageTime()
	{
		float frameDelta = GetCurrentDeltaTime();

		averageTimes.push_front(frameDelta);
		averageTimes.pop_back();

		averageDeltaTick = 0.0f;


		calculateAverageDeltaTick();


	}

	float Time::GetFPS()
	{
		return  1.0f / averageDeltaTick;
	}

	void Time::calculateAverageDeltaTick()
	{
		for (auto const& tick : averageTimes)
		{
			averageDeltaTick += tick;
		}

		averageDeltaTick /= frameTickStoredCount;
	}

}

