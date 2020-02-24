#include "RenderingEngine.h"
#include "World.h"
#include "Canvas.h"
#include "PhysicsComponent.h"
#include "CameraComponent.h"
#include "Mat4.h"
#include "Mesh.h"
#include "RasterizationMode.h"
namespace PXG
{
	RenderingEngine::RenderingEngine()
	{

	}
	void RenderingEngine::AddRenderingComponent(std::shared_ptr<MeshComponent> meshComponent)
	{

	}
	//TODO [Medium Priority] refactor RenderCurrentlySetWorld() and RenderDebugDrawingForSetWorld()
	void RenderingEngine::RenderCurrentlySetWorld()
	{
		Mesh::SetRasterizationMode(RasterizationMode::FILL);
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

	void RenderingEngine::RenderDebugDrawingForSetWorld()
	{
		Mesh::SetRasterizationMode(RasterizationMode::LINE);
		//render physics components if necessary
		if (world)
		{
			bool needToDrawPhysicsComponent = world->IsDrawPhysicsComponentMeshNeeded();
			auto camera = world->GetCamera();

			if (needToDrawPhysicsComponent && camera)
			{
				world->GetPhysicsComponent()->DrawPhysicsRepresentation(Mat4(), camera->GetView(), camera->GetProjection());
			}

		}

		//render all objects send for debug

	}

	void RenderingEngine::RenderCanvas()
	{
		Mesh::SetRasterizationMode(RasterizationMode::FILL);
		const glm::vec3 red = { 0.2f,0.2f,0.2f };

		
		std::shared_ptr<CameraComponent>  camera = canvas->GetCamera();

		if (camera)
		{

			std::shared_ptr<MeshComponent> meshComponent = canvas->GetMeshComponent();

			if (meshComponent)
			{

				meshComponent->Draw(Mat4(), camera->GetView(), camera->GetProjection());
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (fontRenderer)
				fontRenderer->draw(camera->GetProjection().ToGLM(), red);
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

