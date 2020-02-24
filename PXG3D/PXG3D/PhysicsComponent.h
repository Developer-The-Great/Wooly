#pragma once
#include "Component.h"
#include <memory>
#include <vector>

namespace PXG
{
	struct Mat4;
	class AbstractMaterial;
	class Mesh;
	//struct Vector3;

	class PhysicsComponent : public Component
	{
	public:
		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;

		//creates a physics representation by copying the meshes from the mesh component
		void ConstructPhysicsRepresentationFromMeshComponent();

		//creates a physics representation by creating collision cube that expands all the vertices of a given mesh
		void ConstructCollisionCube();


		PhysicsComponent();

		void SetPhysicsDrawingMaterial(std::shared_ptr<AbstractMaterial> newPhysicsDrawingMaterial);
		
		void DrawPhysicsRepresentation(Mat4 parentTransform, Mat4 view, Mat4 projection);

	private:

		

		std::shared_ptr<AbstractMaterial> physicsRenderingMaterial;

		std::vector<std::shared_ptr<Mesh>> meshes;

	};
}


