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
		world->AddToChildren(newGameObject);
		newGameObject->SetParent(world);

		//due to SetOwner() only accepting a Shared Pointer of a gameObject,
		//SetOwner for these components are done here.
		//MeshComponent * mesh = newGameObject->GetMeshComponent().get();
		newGameObject->GetMeshComponent()->SetOwner(newGameObject);
		newGameObject->GetPhysicsComponent()->SetOwner(newGameObject);
		newGameObject->SetWorld(world);
		return newGameObject;
	}
}

