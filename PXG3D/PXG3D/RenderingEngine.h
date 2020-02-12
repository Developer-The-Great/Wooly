#pragma once
#include <memory>
#include "MeshComponent.h"

namespace PXG
{
	class World;
	class Canvas;
	class RenderingEngine
	{
	public:

		RenderingEngine();

		void AddRenderingComponent(std::shared_ptr<MeshComponent> meshComponent);

		void RenderCurrentlySetWorld();
		void RenderCanvas();


		void SetWorld(std::shared_ptr<World> world);

		void SetCanvas(std::shared_ptr<Canvas> canvas);


	private:


		std::shared_ptr<World> world = nullptr;	
		std::shared_ptr<Canvas> canvas = nullptr;


	};
}


