#pragma once
#include "GameObject.h"
#include <list>

namespace PXG
{
	class CameraComponent;
	class LightComponent;

	class World : public GameObject
	{

	public:

		World();

		void SetCamera(std::shared_ptr<CameraComponent> cameraComponent);
		std::shared_ptr<CameraComponent> GetCamera();

		void AddLight(std::shared_ptr<LightComponent> light);
		std::list<std::shared_ptr<LightComponent>>  GetLights();

		int GetLightCount() const;

	protected:

	private:

		std::shared_ptr<CameraComponent> cameraComponent;

		std::list<std::shared_ptr<LightComponent>> lights;

	};
}


