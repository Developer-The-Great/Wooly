#pragma once
#include <memory>



namespace PXG
{
	class PhysicsComponent;
	class World;
	class PhysicsEngine

	{
	public:

		void AddPhysicsComponent(std::shared_ptr<PhysicsComponent> physicsComponent);

		void SetWorld(std::shared_ptr<World> world);

		float GetTickRate();

		float GetCurrentTickRemaining();

		void SetTickRemaining(float tick);

		bool IsTicking();

		float DecreaseRemainingTickTime();

	private:

		const float tickTime = 0.02f;

		float tickTimeRemaining;

		std::shared_ptr<World> world;


	};

}


