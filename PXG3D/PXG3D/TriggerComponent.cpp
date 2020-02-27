#include "TriggerComponent.h"
#include "AbstractEventComponent.h"
namespace PXG
{
	void TriggerComponent::raiseTrigger(Node* currentNode, Node* targetNode)
	{

		component->Execute(currentNode, targetNode);

	}

	void TriggerComponent::SetComponent(std::shared_ptr<AbstractEventComponent> newComponent)
	{
		component = newComponent;
	}
	std::shared_ptr<AbstractEventComponent> TriggerComponent::GetComponent()
	{
		return component;
	}

}