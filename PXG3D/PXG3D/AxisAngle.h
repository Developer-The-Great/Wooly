#pragma once
#include "GLMHeaders.h"
#include <string>
#include "Mathf.h"
#include "Vector3.h"
namespace PXG
{
	//struct Quaternion;

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

		//Quaternion ToQuaternion()
		//{
		//	Vector3 axis(x, y, z);
		//	axis.Normalize();
		//	//axis.no
		//	float qx = axis.x * Mathf::Sin(glm::radians(angle/2.0f));
		//	float qy = axis.y * Mathf::Sin(glm::radians(angle / 2.0f));
		//	float qz = axis.z * Mathf::Sin(glm::radians(angle / 2.0f));
		//	float qw = Mathf::Cos(glm::radians(angle / 2.0f));

		//	return Quaternion(qw, qx, qy, qz).Normalized;
		//}

		std::string ToString()
		{
			return "AxisAngle(Angle: " + std::to_string(angle) + " ,X: " + std::to_string(x) + " ,Y: " + std::to_string(y) + " ,Z: " + std::to_string(z);
		}


	};

}

