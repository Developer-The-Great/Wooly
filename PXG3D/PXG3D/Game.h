#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"

typedef std::shared_ptr<PXG::GameObject> GameObj;

namespace PXG
{
	class Game
	{


	public:

		virtual void Start() = 0;

		virtual void Update() = 0;

		virtual void FixedUpdate() = 0;

		void SetCamera();

		void SetPhysicsEngine(std::shared_ptr<PhysicsEngine> physicsEngine);

		void SetRenderingEngine(std::shared_ptr<RenderingEngine> renderingEngine);

		GameObj Instantiate();




	private:

		GameObject rootObj;

		std::shared_ptr<PhysicsEngine> physicsEngine;
		std::shared_ptr<RenderingEngine> renderingEngine;

	};


}
