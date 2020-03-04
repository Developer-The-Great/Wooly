#pragma once
#include "Component.h"
#include "Subscriber.h"
#include "MapMovementComponent.h"
#include "RayCastHitHandler.h"
#include "NodeGraph.h"
#include "Subject.h"
namespace PXG
{


	class brokenCube : public Component, public subscriber_base, public subject_base
	{
	public:
		enum events : event_t
		{
			ON_CUBE_BROKEN
		};
		virtual void FixedUpdate(float tick) override;
	//	std::shared_ptr<RayCastHitHandler> ray;
		std::shared_ptr<MapMovementComponent> map;
		std::shared_ptr<NodeGraph> graph;

	private:


		virtual void onNotify(subject_base * subject_base, subject_base::event_t event) override;
		bool isTriggered = false;
		bool secondTrigger = false;
		GameObj objectOnTop;

	};

	inline void brokenCube::FixedUpdate(float tick)
	{
		if (secondTrigger)
		{
			Vector3 offset = Vector3(0, -15, 0);
			this->GetOwnerTransform()->translate(offset);
		}
	}
	inline void brokenCube::onNotify(subject_base * subject_base, subject_base::event_t event)
	{
		if (event == MapMovementComponent::LATE_ON_MOVE_FINISHED)
		{
			Vector3 thisPos = GetOwnerTransform()->GetPosition()*0.01f;

			if (!isTriggered)
			{
				Vector3 playerPos = map->getOffset();
				if (thisPos == playerPos)
				{
					isTriggered = true;
				}
			}
			else
			{
				for (Node* otherNode : graph->GetNodes())
				{
					if (otherNode->getPos() == thisPos)
					{
						otherNode->SetNodeWeight(IMPOSSIBLE_REALM(1));
						Debug::Log("second step");

					}
				}
				notify(ON_CUBE_BROKEN);
				secondTrigger = true;
			}

		}
	}
}