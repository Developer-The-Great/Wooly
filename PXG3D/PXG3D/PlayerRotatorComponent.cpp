#include "PlayerRotatorComponent.h"
namespace PXG
{
	void PlayerRotatorComponent::onNotify(subject_base * subject_base, subject_base::event_t event)
	{
		static MapMovementComponent* mapComponent = nullptr;
		//movement notification
		if (mapComponent == subject_base && mapComponent != nullptr ||
			(mapComponent == nullptr && (mapComponent = dynamic_cast<MapMovementComponent*>(subject_base))))
		{
			switch (event)
			{
			case MapMovementComponent::ON_MOVE_FINISHED:
			{

				break;
			}
			case MapMovementComponent::ON_MOVE_START:
			{

				break;
			}case MapMovementComponent::ON_MOVE:
			{
				if (mapComponent->currentDir != lastDir)
				{
					Vector3 otherDir = mapComponent->currentDir;
					rotate(otherDir);
					lastDir = otherDir;
				}
				
				break;
			};
			default:
				break;
			}
		}
	}
	void PlayerRotatorComponent::setInitForward()
	{
		forward = GetOwner()->GetTransform()->GetForward();
		forward = Vector3(1, 0, 0);
	}
	void PlayerRotatorComponent::rotate(Vector3 rotate)
	{
		Debug::Log("ROTATING PLAYER");
		rotate = rotate * -1;
		Quaternion originalRot = GetOwner()->GetTransform()->GetRotation();
		//originalRot = otherObj->GetComponent<TriggerComponent>()->oldRot;
		Debug::Log("forward" + forward.ToString());
		Debug::Log("dir " + rotate.ToString());
		float angle = rotate.getAngle(forward, rotate);
		//target quat 
		Debug::Log("Angle " + std::to_string(angle));
		Quaternion quat = glm::angleAxis(glm::radians(angle), glm::vec3(0, 1, 0));
		//Quaternion newRot = Mathf::Slerp(originalRot, quat, factor);
		GetOwner()->GetTransform()->SetRotation(quat);
		if (angle == 360 || angle == -360) GetOwner()->GetTransform()->rotate(Vector3(0, 1, 0), 180);
	}
}



