#pragma once
#include "GameObject.h"


namespace PXG
{
	class CameraComponent;

	class World : public GameObject
	{

	public:

		World();

		void SetCamera(std::shared_ptr<CameraComponent> cameraComponent);
		std::shared_ptr<CameraComponent> GetCamera();

	protected:

	private:

		std::shared_ptr<CameraComponent> cameraComponent;

	};
}


