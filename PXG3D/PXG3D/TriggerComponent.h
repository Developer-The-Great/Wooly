#pragma once
#include "Component.h"
#include "Game.h"
#include "PXGNode.h"
#include "FollowPlayerComponent.h"
#include <vector>

namespace PXG
{
	class TriggerComponent :public Component
	{
	public:
		//triggers all stored components
		void raiseTrigger(GameObj target, Node* currentNode, Node* targetNode);
		//add new components to execute
		void AddComponent(std::shared_ptr<AbstractEventComponent> newComponent);


	private:

		std::vector< std::shared_ptr<AbstractEventComponent>> components;
	};
}


