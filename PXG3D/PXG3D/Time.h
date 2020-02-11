#pragma once

#include <list>
#include <GLFW/glfw3.h>

namespace PXG
{
	class Time
	{
	public:

		Time(int frameTickStored,float firstFPSEstimate);

		~Time();

		float GetTime();

		float GetAverageDeltaTime();

		void UpdateTimePassed();

		float GetCurrentDeltaTime();

		void UpdateAverageTime();

		float GetFPS();

	private:

		float averageDeltaTick;

		float timePassed;

		std::list<float> averageTimes;

		int frameTickStoredCount;

		void calculateAverageDeltaTick();
	};

}

