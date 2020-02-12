#include "RenderingEngine.h"
#include "World.h"
#include "Canvas.h"
#include "CameraComponent.h"

namespace PXG
{
	RenderingEngine::RenderingEngine()
	{

	}
	void RenderingEngine::AddRenderingComponent(std::shared_ptr<MeshComponent> meshComponent)
	{

	}
	void RenderingEngine::RenderCurrentlySetWorld()
	{

		std::shared_ptr<CameraComponent>  camera = world->GetCamera();
		
		if (camera)
		{

			std::shared_ptr<MeshComponent> meshComponent = world->GetMeshComponent();

			if (meshComponent)
			{
				meshComponent->Draw(Mat4(), camera->GetView(), camera->GetProjection());
			}

		}
		

	}
	void RenderingEngine::RenderCanvas()
	{
		std::shared_ptr<CameraComponent>  camera = canvas->GetCamera();

		if (camera)
		{

			std::shared_ptr<MeshComponent> meshComponent = canvas->GetMeshComponent();

			if (meshComponent)
			{

				meshComponent->Draw(Mat4(), camera->GetView(), camera->GetProjection());
			}

		}
	}
	void RenderingEngine::SetWorld(std::shared_ptr<World> world)
	{
		this->world = world;
	}
	void RenderingEngine::SetCanvas(std::shared_ptr<Canvas> canvas)
	{
		this->canvas = canvas;
	}
}

