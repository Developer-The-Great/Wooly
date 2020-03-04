#include "TriggerComponent.h"
#include "AbstractEventComponent.h"
namespace PXG
{
	void TriggerComponent::raiseTrigger(Node* currentNode, Node* targetNode)
	{
		if (component)
		{
			component->Execute(currentNode, targetNode);
		}
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

	void TriggerComponent::setNodePos(Vector3 pos)
	{
		nodePos = pos;
	}

	void TriggerComponent::onNotify(subject_base * subjectBase, subject_base::event_t event)
	{
		static MapMovementComponent* mapComponent = nullptr;
		static RayCastHitHandler* rayCast = nullptr;
		//movement notification
		if (mapComponent == subjectBase && mapComponent != nullptr ||
			(mapComponent == nullptr && (mapComponent = dynamic_cast<MapMovementComponent*>(subjectBase))))
		{
			switch (event)
			{
			case MapMovementComponent::ON_MOVE_FINISHED:
			{

				if (GetComponent()->isMoving())
				{
					Vector3 oldNodePos = nodePos;
					Vector3 deltaOffset = mapComponent->getTempNodePos() - mapComponent->getOffset();

					Vector3 pos = GetOwner()->GetTransform()->GetPosition() - Vector3(50, 0, 50);
					pos = pos * 0.01f;
					pos.y = 0;
					pos = pos + mapComponent->getTempNodePos();

					for (auto node : nodeGraph->GetNodes())
					{

						if (node->getPos() == pos)
						{
							Debug::Log("chaning new node, pos:");
							Debug::Log(node->getPos().ToString());
							SetNodePos(pos);
							node->SetNodeWeight(2000);
						}
						if (node->getPos() == oldNodePos)
						{
							Debug::Log("chaning old node");
							node->SetNodeWeight(1);
						}
					}
					Vector3 thisPos = this->getNodePos();
					Vector3 targetPos = pos;
					Vector3 delta = targetPos - oldNodePos;
					float angle = thisPos.getAngle(delta, this->GetOwnerTransform()->GetForward());
					oldAngle = angle;
					
				}
				break;
			}
			case MapMovementComponent::ON_MOVE_START:
			{
				Debug::Log("SETTING TEMP FORWARD!!!");
				//tempForward =this->GetOwner()->GetTransform()->GetForward();
				break;
			}
			default:
				break;
			}
		}


		//ray cast activation
		if (rayCast == subjectBase && rayCast != nullptr ||
			(rayCast == nullptr && (rayCast = dynamic_cast<RayCastHitHandler*>(subjectBase))))
		{
			switch (event)
			{
			case RayCastHitHandler::RAY_CAST_INTERACTIVE_HIT:
			{

				Node* node = nullptr;
				raiseTrigger(NULL, NULL);
				Rotate(rayCast->getLastTarget()->getPos());
				break;
			}
			default:
				break;
			}
		}
	}

	void TriggerComponent::SetNodeGraph(std::shared_ptr<NodeGraph> newNodeGraph)
	{
		nodeGraph = newNodeGraph;
	}

	void TriggerComponent::Rotate(Vector3 otherPos)
	{
		Vector3 thisPos = this->getNodePos();
		Vector3 targetPos = otherPos;
		Vector3 delta = targetPos - thisPos;
		float angle = thisPos.getAngle(delta, this->GetOwnerTransform()->GetForward());
		tempForward = this->GetOwnerTransform()->GetForward();
		Quaternion quat = glm::angleAxis(glm::radians(angle), glm::vec3(0, 1, 0));
		this->GetOwnerTransform()->SetRotation(quat);
		this->oldAngle = angle;
	}

}