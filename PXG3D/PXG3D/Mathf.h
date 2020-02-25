#pragma once
#include <float.h>




namespace PXG
{
	struct Quaternion;
	struct Vector3;
	struct AxisAngle;

	//Contains commonly used mathematical functions. Also contains 3D math related functions
	class Mathf
	{
	public:

		static const float Deg2Rad;
		static const float Epsilon;
		static const float Rad2Deg;

		static float Abs(float x);

		static float ACos(float x);

		static float Clamp(float value, float min, float max);

		static float Cos(float radians);

		static Vector3 Cross(Vector3 a, Vector3 b);

		static float Dot(Vector3 a, Vector3 b);

		static bool FloatCompare(float a, float b);

		static float GetQuaternionAngleDifference(Quaternion a, Quaternion b);

		static float Lerp(float a, float b, float t);

		static Vector3 Lerp(Vector3 a, Vector3 b, float t);

		//Warning : Assumes that given forward and up are orthonormal
		static Quaternion LookRot(Vector3 Forward, Vector3 Up);

		static float Map(float value, float min1, float max1, float min2, float max2);

		static float Pow(float value, float pow);

		static Vector3 RotateVectorAroundAxis(Vector3 toRotate, Vector3 axis, float angle);

		static float Sin(float radians);

		static Vector3 Slerp(Vector3 a, Vector3 b, float t);

		static Quaternion Slerp(Quaternion a, Quaternion b, float t);

		static float Sqrt(float x);

		static Quaternion ToQuaternion(AxisAngle axisAngle);
	};

}
