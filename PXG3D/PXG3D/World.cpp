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
	void World::AddLight(std::shared_ptr<LightComponent> light)
	{
		lights.emplace_front(light);
		
	}
	std::list<std::shared_ptr<LightComponent>> World::GetLights()
	{
		return lights;
	}
	int World::GetLightCount() const
	{
		return lights.size();
	}
}

