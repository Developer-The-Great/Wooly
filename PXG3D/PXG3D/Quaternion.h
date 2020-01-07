#pragma once
#include "GLMHeaders.h"
#include "AxisAngle.h"

namespace PXG
{
	
	struct  Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		Quaternion(float qw, float qx, float qy, float qz)
		{
			w = qw;
			x = qx;
			y = qy;
			z = qz;
		}

		Quaternion()
		{
			w = 0;
			x = 0;
			y = 0;
			z = 0;
		}

		Quaternion(glm::quat Quat)
		{
			x = Quat.x;
			y = Quat.y;
			z = Quat.z;
			w = Quat.w;
		}

		Quaternion operator/(const float scalar)
		{
			Quaternion result;

			result.w = w / scalar;
			result.x = x / scalar;
			result.y = y / scalar;
			result.z = z / scalar;

			return result;
		}

		Quaternion operator*(const float& scalar)
		{
			Quaternion result;

			result.w = w * scalar;
			result.x = x * scalar;
			result.y = y * scalar;
			result.z = z * scalar;

			return result;
		}

		Quaternion operator=(const glm::quat& quat)
		{
			Quaternion result;

			result.w = quat.w;
			result.x = quat.x;
			result.y = quat.y;
			result.z = quat.z;

			return result;
		}

		float Length()
		{
			return glm::length(glm::quat(w, x, y, z));
		}

		Quaternion Normalized()
		{
			return glm::normalize(glm::quat(w, x, y, z));
		}

		AxisAngle ToAxisAngle()
		{

			AxisAngle Result;
			Result.angle = 2 * Mathf::ACos(w);
			Result.x = x / Mathf::Sqrt(1 - w * w);
			Result.y = y / Mathf::Sqrt(1 - w * w);
			Result.z = z / Mathf::Sqrt(1 - w * w);

			return Result;
		}

		std::string ToString()
		{
			return glm::to_string(glm::quat(w, x, y, z));
		}
	};
}
