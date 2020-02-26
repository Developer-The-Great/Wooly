#include "TriggerComponent.h"
#include "AbstractEventComponent.h"
namespace PXG
{
	void TriggerComponent::raiseTrigger(GameObj target,Node* currentNode, Node* targetNode)
	{
		for (auto it: components)
		{
			it->Execute(currentNode,targetNode);
		}
	}

	void TriggerComponent::AddComponent(std::shared_ptr<AbstractEventComponent> newComponent)
	{
		components.push_back(newComponent);
	}
}