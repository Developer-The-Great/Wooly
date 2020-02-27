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
		void raiseTrigger( Node* currentNode, Node* targetNode);
		//add new components to execute
		void SetComponent(std::shared_ptr<AbstractEventComponent> newComponent);
		std::shared_ptr<AbstractEventComponent> GetComponent();

	private:

	std::shared_ptr<AbstractEventComponent> component;
	};
}


