#pragma once
#include "GLMHeaders.h"
#include "Debug.h"
#include <string>

namespace PXG
{
	struct Vector3
	{
		float x;
		float y;
		float z;

		Vector3(float v3x, float v3y, float v3z)
		{
			x = v3x;
			y = v3y;
			z = v3z;
		}

		Vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		Vector3(glm::vec3 GLMVec3)
		{
			x = GLMVec3.x;
			y = GLMVec3.y;
			z = GLMVec3.z;
		}

		Vector3 Normalized()
		{
			return Vector3(glm::normalize(glm::vec3(x,y,z)));
		}

		Vector3& Normalize()
		{
			*this = Normalized();

			return *this;
		}

		Vector3 operator+ (const Vector3 &vec3)
		{
			float newX = x + vec3.x;
			float newY = y + vec3.y;
			float newZ = z + vec3.z;

			return Vector3(newX, newY, newZ);
		}

		Vector3 operator- (const Vector3 &vec3)
		{
			float newX = x - vec3.x;
			float newY = y - vec3.y;
			float newZ = z - vec3.z;

			return Vector3(newX, newY, newZ);
		}

		Vector3 operator* (const float &scalar)
		{
			float newX = x * scalar;
			float newY = y * scalar;
			float newZ = z * scalar;

			return Vector3(newX, newY, newZ);
		}

		Vector3 operator/ (const float &scalar) 
		{
			float newX = x / scalar;
			float newY = y / scalar;
			float newZ = z / scalar;

			return Vector3(newX,newY,newZ);
		}

		Vector3& operator= (const Vector3 &vec3) 
		{

			x = vec3.x;
			y = vec3.y;
			z = vec3.z;
			
			return *this;
		}

		Vector3& operator= (const glm::vec3& vec3) 
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;

			return *this;
		}

		float Length()
		{
			return glm::length(glm::vec3(x, y, z));
		}

		std::string ToString()
		{
			return glm::to_string(glm::vec3(x, y, z));
		}

		glm::vec3 ToGLMVec3() const
		{
			return glm::vec3(x, y, z);
		}

	};
}


