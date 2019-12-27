#pragma once
#include "GLMHeaders.h"

struct AxisAngle
{
	float angle;
	float x;
	float y;
	float z;

	AxisAngle()
	{
		angle = 0;
		x = 0;
		y = 0;
		z = 1;
	}

	AxisAngle(float aAngle, float ax, float ay, float az)
	{
		angle = aAngle;
		x = ax;
		y = ay;
		z = az;
	}

	/*AxisAngle(glm::angleAxis axisAngle)
	{
		
	}*/

	

};
