#include "RayCastShooter.h"
#include "PhysicsEngine.h"
#include "Debug.h"
#include "Transform.h"
#include "GameObject.h"
#include "HitInfo.h"
#include "Input.h"
#include "CameraComponent.h"

namespace PXG
{
	void RayCastShooter::Start()
	{
	}
	void RayCastShooter::FixedUpdate(float tick)
	{

		auto transform = GetOwnerTransform();
		Vector3 forward = transform->GetForward();

		HitInfo info;


		float x = Input::GetMouseX();
		float y = Input::GetMouseY();

		Vector3 position = PhysicsEngine::GetOrthographicCameraWorldPosition(x, y, 800.0f, 600.0f, GetOwner());

		PhysicsEngine::DetailedRaycast(position, forward, info, GetOwner()->GetWorld().lock());
		

		if (info.RayHit) 
		{ 
			Debug::Log("raycast hit {0} ", info.GameObjectHit->name);
		}
		else
		{
			Debug::Log("raycast did not hit");
		}

	}
}

