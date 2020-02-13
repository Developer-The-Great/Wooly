#pragma once
#include "Component.h"

namespace PXG
{
	

	class PhysicsComponent : public Component
	{
	public:
		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;

		PhysicsComponent();
	private:

		//creates a collision cube that expands all the vertices of a given mesh
		void ConstructCollisionCube();


	};
}


