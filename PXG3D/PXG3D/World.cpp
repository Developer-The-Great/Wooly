#include "World.h"
#include "CameraComponent.h"

namespace PXG
{
	World::World():GameObject()
	{

	}
	void World::SetCamera(std::shared_ptr<CameraComponent>  cameraComponent)
	{
		this->cameraComponent = cameraComponent;
	}

	std::shared_ptr<CameraComponent> World::GetCamera()
	{
		return cameraComponent;
	}
}

