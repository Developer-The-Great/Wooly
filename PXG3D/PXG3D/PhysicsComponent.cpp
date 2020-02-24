#include "PhysicsComponent.h"
#include "Mat4.h"
#include "ColorMaterial.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Mesh.h"
namespace PXG
{
	PhysicsComponent::PhysicsComponent() : Component()
	{
		physicsRenderingMaterial = std::make_shared<ColorMaterial>(Vector3(1,1,1));
	}

	void PhysicsComponent::SetPhysicsDrawingMaterial(std::shared_ptr<AbstractMaterial> newPhysicsDrawingMaterial)
	{
		physicsRenderingMaterial = newPhysicsDrawingMaterial;
	}

	void PhysicsComponent::Start()
	{

	}

	void PhysicsComponent::FixedUpdate(float tick)
	{
	}

	void PhysicsComponent::ConstructPhysicsRepresentationFromMeshComponent()
	{
		
		auto meshComponentMeshes = GetOwner()->GetMeshComponent()->GetMeshes();
		meshes = meshComponentMeshes;
	}

	//TODO this looks pretty similar to how mesh component draws things, find a way to refactor it
	void PhysicsComponent::DrawPhysicsRepresentation(Mat4 parentTransform, Mat4 view, Mat4 projection)
	{

		auto owner = GetOwner();
		//set uniforms for material

		Mat4 currentTransform = owner->GetTransform()->GetLocalTransform() * parentTransform;

		if (physicsRenderingMaterial)
		{
			physicsRenderingMaterial->SendUniforms(owner->GetWorld(), currentTransform, view, projection);

			for (auto const& mesh : meshes)
			{
				mesh->Draw(physicsRenderingMaterial->GetShader());
			}
		}


		for (std::shared_ptr<GameObject> const& child : owner->GetChildren())
		{
			child->GetPhysicsComponent()->DrawPhysicsRepresentation(currentTransform, view, projection);
		}



	}
	void PhysicsComponent::ConstructCollisionCube()
	{



	}
}

