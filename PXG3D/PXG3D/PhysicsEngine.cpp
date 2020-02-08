#include "core/PhysicsEngine.h"
#include "PhysicsComponent.h"
#include "World.h"
#include "Mathf.h"

namespace PXG
{
	void PhysicsEngine::AddPhysicsComponent(std::shared_ptr<PhysicsComponent> physicsComponent)
	{

	}

	void PhysicsEngine::SetWorld(std::shared_ptr<World> world)
	{
		this->world = world;
	}

	float PhysicsEngine::GetTickRate()
	{
		return tickTime;
	}

	float PhysicsEngine::GetCurrentTickRemaining()
	{
		return tickTimeRemaining;
	}

	void PhysicsEngine::SetTickRemaining(float tick)
	{
		tickTimeRemaining = tick;
	}

	bool PhysicsEngine::IsTicking()
	{
		if (tickTimeRemaining > 0.0f)
		{
			return true;
		}
		else
		{
			tickTimeRemaining = 0.0f;
			return false;
		}

	}

	float PhysicsEngine::DecreaseRemainingTickTime()
	{
		if (tickTimeRemaining > tickTime)
		{
			tickTimeRemaining -= tickTime;
			return tickTime;
		}
		else
		{
			float decreasedTime = tickTimeRemaining;

			tickTimeRemaining = 0;
			
			return  decreasedTime;
		}



	}

}

