#include "GrandpaComponent.h"
namespace PXG
{

	void GrandpaComponent::Execute(Node * currentNode, Node * targetNode)
	{
	}
	void GrandpaComponent::onNotify(subject_base * subject_base, subject_base::event_t event)
	{
		switch (event)
		{
		case MapMovementComponent::ON_MOVE_FINISHED:
		{
			Vector3 thisPos = Vector3(0, 0, 0);
			if(this->GetOwner()->HasComponent<TriggerComponent>())
			{
				thisPos = this->GetOwner()->GetComponent<TriggerComponent>()->getNodePos();
			}
			for (auto sheep : sheeps)
			{
				if (sheep->HasComponent<TriggerComponent>())
				{
					Vector3 sheepPos = sheep->GetComponent<TriggerComponent>()->getNodePos();
					if (Mathf::Abs(thisPos.x - sheepPos.x) ==0 || Mathf::Abs(thisPos.z - sheepPos.z) ==0)
					{
						Debug::Log("WON");
					}
				}
			}
			break;
		}
		default:
			break;
		}
	}
}