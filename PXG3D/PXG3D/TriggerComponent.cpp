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

	void TriggerComponent::SetNodePos(Vector3 newPos)
	{
		nodePos = newPos;
	}

	Vector3 TriggerComponent::getNodePos()
	{
		return nodePos;
	}

	void TriggerComponent::moveNodePos(Vector3 dir)
	{
		nodePos = nodePos + dir;
	}

	void TriggerComponent::onNotify(subject_base * subjectBase, subject_base::event_t event)
	{
		MapMovementComponent* mapComponent = nullptr;
		switch (event)
		{
		case MapMovementComponent::ON_MOVE:
		{
			if (mapComponent == subjectBase && mapComponent != nullptr ||
				(mapComponent == nullptr && (mapComponent = dynamic_cast<MapMovementComponent*>(subjectBase)))
				&& GetComponent()->isMoving())
			{
				Debug::Log("trigger on move notification");
				Vector3 oldNodePos = nodePos;
				Vector3 deltaOffset = mapComponent->getOffset() - mapComponent->getOldOffset();
				nodePos = nodePos + deltaOffset;

				for (auto node : nodeGraph->GetNodes())
				{
					
					if(node->getPos() == nodePos)
					{
						Debug::Log("chaning node");
						node->SetNodeWeight(2000);
					}
					if(node->getPos()==oldNodePos)
					{
						Debug::Log("chaning node");
						node->SetNodeWeight(1);
					}
				}
			}
		}
		default:
			break;
		}
	}

	void TriggerComponent::SetNodeGraph(std::shared_ptr<NodeGraph> newNodeGraph)
	{
		nodeGraph = newNodeGraph;
	}

}