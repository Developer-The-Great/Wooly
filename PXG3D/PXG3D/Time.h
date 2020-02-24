#pragma once

#include <list>
#include <GLFW/glfw3.h>

namespace PXG
{
	class Time
	{
	public:

		Time(int,float);

		float GetTime();

		float GetAverageDeltaTime() const;

		void UpdateTimePassed();

		float GetCurrentDeltaTime() const;

		void UpdateAverageTime();

		float GetFPS() const;

	private:

		float averageDeltaTick;

		float timePassed;

		std::list<float> averageTimes;

		int frameTickStoredCount;

		void calculateAverageDeltaTick();
	};

}

