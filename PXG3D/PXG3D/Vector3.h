#pragma once
#include "GLMHeaders.h"
#include <string>


struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float v3x , float v3y ,float v3z)
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
		Vector3 normVector = *this;

		return normVector/Length();
	}

	Vector3 operator+ (const Vector3 &vec3)
	{
		Vector3 result;

		result.x = x + vec3.x;
		result.y = y + vec3.y;
		result.z = z + vec3.z;

		return result;
	}

	Vector3 operator- (const Vector3 &vec3)
	{
		Vector3 result;

		result.x = x - vec3.x;
		result.y = y - vec3.y;
		result.z = z - vec3.z;

		return result;
	}

	Vector3 operator* (const float &scalar)
	{
		Vector3 result;

		result.x = x * scalar;
		result.y = y * scalar;
		result.z = z * scalar;

		return result;
	}

	Vector3 operator/ (const float &scalar)
	{
		Vector3 result;

		result.x = x / scalar;
		result.y = y / scalar;
		result.z = z / scalar;

		return result;
	}

	Vector3 operator= (const Vector3 &vec3)
	{
		Vector3 result;

		result.x = vec3.x;
		result.y = vec3.y;
		result.z = vec3.z;

		return result;
	}

	Vector3 operator= (const glm::vec3& vec3)
	{
		Vector3 result;

		result.x = vec3.x;
		result.y = vec3.y;
		result.z = vec3.z;

		return result;
	}

	float Length()
	{
		return glm::length(glm::vec3(x,y,z));
	}

	std::string ToString()
	{
		return glm::to_string(glm::vec3(x, y, z));
	}

	glm::vec3 ToGLMVec3()
	{
		return glm::vec3(x, y, z);
	}

}; 

