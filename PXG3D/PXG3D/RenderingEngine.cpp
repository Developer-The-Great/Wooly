#include "core/RenderingEngine.h"
#include "World.h"
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
	void RenderingEngine::SetWorld(std::shared_ptr<World> world)
	{
		this->world = world;
	}
}

