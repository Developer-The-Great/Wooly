#pragma once
#include "GameObject.h"
#include <memory>
#include "Vector3.h"

namespace PXG
{
	struct HitInfo
	{
		Vector3 Position;
		std::shared_ptr<GameObject> GameObjectHit = nullptr;
		Vector3 Normal;
		float DistanceFromRayPosition;
		float T;
		bool RayHit;

	};
}
