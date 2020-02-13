#pragma once
#include "Quaternion.h"
#include "Vector3.h"
#include "Mat4.h"

namespace PXG
{

	class Transform
	{
	public:

		Transform(Quaternion rotation,Vector3 position,Vector3 scale = Vector3(1, 1, 1));

		Transform();

		Quaternion GetLocalRotation() const;

		Vector3 GetLocalPosition() const;

		void SetLocalPosition(Vector3 localPosition);

		Vector3 GetPosition();

		void translate(Vector3 translation);

		void rotate(Vector3 axis, float angle);

		void Scale(Vector3 newScale);

		Quaternion GetRotation();

		Mat4 GetLocalTransform() const;

		Mat4 GetWorldTransform() const;

		void SetParent(Transform * transform);

		Transform* GetParent() const;

		Vector3 getScale() const;

		Vector3 GetForward() const;

		Vector3 GetRight() const;

		Vector3 GetUp() const;


	private:

		Quaternion rotation;
		Vector3 position;
		Vector3 scale;


		Transform* parentTransform = nullptr;



	};
}


