#include "Mathf.h"
#include <iostream>

const float Mathf::Deg2Rad = 0.0174532f;
const float Mathf::Epsilon = FLT_EPSILON;
const float Mathf::Rad2Deg = 57.295779f;

//TODO replace GLM functions with custom functions

bool Mathf::FloatCompare(float a, float b)
{
	return glm::abs(a-b) < Epsilon;
}

float Mathf::Abs(float x)
{
	return glm::abs(x);
}

float Mathf::Clamp(float value, float min, float max)
{
	if (value < min) { return min; }
	if (value > max) { return max; }

	return value;
}

float Mathf::Cos(float radians)
{
	return glm::cos(radians);
}

Vector3 Mathf::Cross(Vector3 a, Vector3 b)
{
	return glm::cross(glm::vec3(a.x, a.y, a.z), glm::vec3(b.x, b.y, b.z));
}

float Mathf::Dot(Vector3 a, Vector3 b)
{
	return glm::dot(glm::vec3(a.x,a.y,a.z),glm::vec3(b.x,b.y,b.z));
}

float Mathf::Lerp(float a, float b, float t)
{
	return a + (b-a) * t;
}

Vector3 Mathf::Lerp(Vector3 a, Vector3 b, float t)
{
	return a * (1-t) + b * t;
}

Vector3 Mathf::Slerp(Vector3 a, Vector3 b, float t)
{
	return Vector3();
}

Quaternion Mathf::Slerp(Quaternion a, Quaternion b, float t)
{
	glm::quat A(a.w, a.x, a.y, a.z);
	glm::quat B(b.y, b.x, b.y, b.z);

	return glm::slerp(A, B, t);
}

//TODO implement quat angle difference
float Mathf::GetQuaternionAngleDifference(Quaternion a, Quaternion b)
{
	//get axis angle representation of a and b

	//get vector c, a vector perpendicular to the forward of a and b

	//construct 2 new axis angle orientations. one orientation (A') where  c is forward and a is up and another one (B')
	//where c is forward and b is up

	//return the angle difference between A' and B'
	return 0;
}

Quaternion Mathf::LookRot(Vector3 Forward, Vector3 Up)
{
	
	Vector3 right = Cross(Up,Forward ).Normalized();

	glm::mat3 GLMMat3(right.ToGLMVec3(), Up.ToGLMVec3(), Forward.ToGLMVec3());

	Quaternion Result = glm::normalize(glm::toQuat(GLMMat3));

	return Result;
}

float Mathf::Map(float value, float min1, float max1, float min2, float max2)
{
	return min2 + ((value - min1)/(max1 - min1)) * (max2-min2);
}

float Mathf::Pow(float value, float pow)
{
	return glm::pow(value,pow);
}

float Mathf::Sin(float radians)
{
	return glm::sin(radians);
}
