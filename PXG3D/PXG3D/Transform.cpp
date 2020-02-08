#include "Transform.h"
#include "Debug.h"

namespace PXG
{
	Transform::Transform(Quaternion rotation, Vector3 position, Vector3 scale)
	{
		this->rotation = rotation;
		this->position = position;
	}
	Transform::Transform()
	{
		scale = Vector3(1.0f, 1.0f,1.0f);
	}
	inline Quaternion Transform::GetLocalRotation() const
	{
		return rotation;
	}
	Vector3 Transform::GetLocalPosition() const
	{
		return position;
		
	}

	void Transform::SetLocalPosition(Vector3 localPosition)
	{
		position = localPosition;
	}

	Vector3 Transform::GetPosition()
	{
		if (parentTransform)
		{
			Vector3 result(parentTransform->GetWorldTransform().Matrix * glm::vec4(position.ToGLMVec3(), 1));

			return result;
		}
		return position;
	}

	void Transform::translate(Vector3 translation)
	{
		position = position + translation;
	}

	Quaternion Transform::GetRotation()
	{
		if (parentTransform)
		{
			return parentTransform->GetRotation() * GetLocalRotation();
		}
		return GetLocalRotation();
	}

	Mat4 Transform::GetLocalTransform() 
	{
		glm::mat4 mat4Scale(
			glm::vec4(scale.x, 0, 0, 0),
			glm::vec4(0, scale.y, 0, 0),
			glm::vec4(0, 0, scale.z, 0),
			glm::vec4(0, 0, 0, 1)
		);

		glm::quat Quaternion = rotation.ToGLMQuat();

		glm::mat4 mat4Rotation = glm::toMat4(Quaternion);

		glm::mat4 mat4Position(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(position.ToGLMVec3(), 1)
		);
		

		return Mat4(mat4Scale * mat4Rotation * mat4Position);
	}

	Mat4 Transform::GetWorldTransform()
	{
		if (parentTransform)
		{
			return parentTransform->GetWorldTransform() * GetLocalTransform();
		}
		
		return GetLocalTransform();
	}

	void Transform::SetParent(Transform * transform)
	{
		parentTransform = transform;
	}

	/*inline void Transform::SetParent(Transform * transform)
	{
		parentTransform = transform;
	}*/

}

