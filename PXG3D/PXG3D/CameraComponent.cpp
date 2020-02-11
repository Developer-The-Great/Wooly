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
		//glm::ortho(0.0f, 800.0f,0.0f,0.0f, 0.1f, 100.0f);
		//glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
		projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
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

		if (auto worldPtr = owner->GetWorld().lock())
		{
			worldPtr->SetCamera(shared_from_this());
		}
	}
	int CameraComponent::GetUseCount()
	{
		return owner.use_count();
	}
}
