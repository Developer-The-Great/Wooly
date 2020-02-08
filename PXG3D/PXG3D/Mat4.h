#pragma once
#include "GLMHeaders.h"
#include "Vector3.h"

namespace PXG
{
	struct Mat4
	{
		glm::mat4 Matrix;

		Mat4(glm::mat4 initMat)
		{
			Matrix = initMat;
		}

		Mat4()
		{
			Matrix = glm::mat4(1.0f);
		}

		Mat4 operator*(const Mat4& otherMat4)
		{
			return Mat4( otherMat4.Matrix  * Matrix);
		}

		Mat4 operator*(const Vector3 vec3)
		{
			glm::mat3 x(Matrix);
			
			return Matrix ;
		}

		Mat4& operator=(const Mat4& otherMat4)
		{
			Matrix = otherMat4.Matrix;

			return *this;
		}

		Mat4& operator=(const glm::mat4& otherMat4)
		{
			Matrix = otherMat4;

			return *this;
		}

		std::string ToString()
		{
			return glm::to_string(Matrix);
		}

		glm::mat4 ToGLM()
		{
			return Matrix;
		}

	};
}
