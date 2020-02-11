#include "FreeMovementComponent.h"
#include "Input.h"
#include "GameObject.h"
#include "KeyCode.h"
#include "Debug.h"

namespace PXG
{
	void FreeMovementComponent::Start()
	{


	}

	void FreeMovementComponent::FixedUpdate(float tick)
	{

		Debug::Log("update");

		if (!owner.expired())
		{
			auto ownerPtr = owner.lock();
			auto transform = ownerPtr->GetTransform();
			Debug::Log("position {0}", transform->GetPosition().ToString());
			float speed = 0.1f;

			if (Input::GetKey(KeyCode::W))
			{
				transform->translate(Vector3(0, 0, -speed));
			}

			if (Input::GetKey(KeyCode::A))
			{
				transform->translate(Vector3(-speed, 0, 0));
			}
			
			if (Input::GetKey(KeyCode::S))
			{
				transform->translate(Vector3(0, 0, speed));
			}
			
			if (Input::GetKey(KeyCode::D))
			{
				transform->translate(Vector3(speed, 0, 0));
			}
			
			if (Input::GetKey(KeyCode::Q))
			{
				transform->translate(Vector3(0, speed, 0));
			}
			
			if (Input::GetKey(KeyCode::E))
			{
				transform->translate(Vector3(0, -speed, 0));
			}
			
			
				
		}

	}
}

