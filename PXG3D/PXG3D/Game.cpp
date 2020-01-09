#include "Game.h"

namespace PXG
{
	void Game::SetCamera()
	{
	}
	void Game::SetPhysicsEngine(std::shared_ptr<PhysicsEngine> physicsEngine)
	{
		this->physicsEngine = physicsEngine;
	}
	void Game::SetRenderingEngine(std::shared_ptr<RenderingEngine> renderingEngine)
	{
		this->renderingEngine = renderingEngine;
	}
	GameObj Game::Instantiate()
	{
		GameObj newGameObject = std::make_shared<GameObject>();
		rootObj.AddToChildren(newGameObject);

		newGameObject->AddToPhysicsEngine(physicsEngine);
		newGameObject->AddToRenderingEngine(renderingEngine);


		return newGameObject;
	}
}

