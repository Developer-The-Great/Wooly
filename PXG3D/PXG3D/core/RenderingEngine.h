#pragma once
#include <memory>
#include "MeshComponent.h"


namespace PXG
{
	class World;

	class RenderingEngine
	{
	public:

		RenderingEngine();

		void AddRenderingComponent(std::shared_ptr<MeshComponent> meshComponent);

		void RenderCurrentlySetWorld();

		void SetWorld(std::shared_ptr<World> world);

		

	private:


		std::shared_ptr<World> world = nullptr;

	};
}


