#include "Game.h"
#include "World.h"
#include "PhysicsComponent.h"

namespace PXG
{
	Game::Game()
	{
		world = std::make_shared<World>();

		
	}

	std::shared_ptr<World> Game::GetWorld() const
	{
		return world;
	}

	void Game::SetCamera()
	{
	}

	void Game::Initialize()
	{


	}
	

	GameObj Game::Instantiate()
	{
		GameObj newGameObject = std::make_shared<GameObject>();
	
		newGameObject->SetWorld(world);
		return newGameObject;
	}
}

