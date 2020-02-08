#include "CameraComponent.h"
#include "GameObject.h"
#include <memory>
#include "World.h"

namespace PXG
{
	CameraComponent::CameraComponent()
	{

		bool hasOwner = !owner.expired();
		Debug::Log("owner use {0}", owner.use_count());
		projection =  glm::perspective(glm::radians(80.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
	}
	CameraComponent::~CameraComponent()
	{
		Debug::Log("CAM DESTROYED");
	}
	void CameraComponent::Start()
	{
	}
	void CameraComponent::FixedUpdate(float tick)
	{
	}
	Mat4 CameraComponent::GetView()
	{


		bool hasOwner = !(owner.expired());
		if (hasOwner)
		{

			auto ownerPtr = owner.lock();
			
			//if(auto)
			auto x = ownerPtr->GetTransform();
			assert(x);
			return glm::inverse(x->GetWorldTransform().ToGLM());
		}
		
		return Mat4();
	}
	Mat4 CameraComponent::GetProjection()
	{
		return projection;
	}
	void CameraComponent::SetOwner(std::shared_ptr<GameObject> owner)
	{
		Component::SetOwner(owner);
	}
	int CameraComponent::GetUseCount()
	{
		return owner.use_count();
	}
}
