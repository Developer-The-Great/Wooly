#include "MapMovementComponent.h"
#include "Input.h"
#include "GameObject.h"
#include "KeyCode.h"
#include "Debug.h"
#include "Tile.h"
#include "TriggerComponent.h"
namespace PXG
{



	void MapMovementComponent::Start()
	{
	}

	void MapMovementComponent::FixedUpdate(float tick)
	{
		static float timer = 0;
		if (!commandQueue.empty())
		{

			timer += tick * 5;

			const auto back = commandQueue.back();
			static bool restart = true;

			if (restart)
			{
				notify(ON_MOVE_START);
				//restart = false;
			}

			bool result = false;
			switch (back)
			{
			case FORWARD:	result = move({ 0, 0, 1 }, restart, timer, tick); break;
			case BACKWARD:  result = move({ 0 ,0,-1 }, restart, timer, tick); break;
			case UP:		result = move({ 0, 1, 0 }, restart, timer, tick); break;
			case DOWN:		result = move({ 0,-1, 0 }, restart, timer, tick); break;
			case LEFT:		result = move({ 1, 0, 0 }, restart, timer, tick); break;
			case RIGHT:		result = move({ -1, 0, 0 }, restart, timer, tick); break;
			default:;
			}
			restart = result;
			if (result) {
				restart = true;
				timer = 0;


				//change offset last after notifying


				tempNodePos = tempNodePos - tempOffset;
		
				notify(ON_MOVE_FINISHED);

				oldOffset = offset;
		
				commandQueue.pop_back();


			}
		}
	}

	void MapMovementComponent::SetMap(std::shared_ptr<GameObject> newMap)
	{
		this->map = newMap;
	}

	void MapMovementComponent::Reset() const
	{
		Transform* mapTransform = map->GetTransform();
		mapTransform->SetLocalPosition({ 0,0,0 });
	}

	void MapMovementComponent::AddOtherObjectToMove(std::shared_ptr<GameObject> newObject)
	{
		otherObjectsToMove.push_back(newObject);
	}

	bool MapMovementComponent::move(PXG::Vector3 direction, bool restart, float factor, float tick)
	{
		if (factor > 1)
			factor = 1;

		static Vector3 mapIntialPosition = map->GetTransform()->GetPosition();
		if (restart)
		{
			mapIntialPosition = map->GetTransform()->GetPosition();
		}


		Transform* mapTransform = map->GetTransform();
		direction = direction * 100;
		mapTransform->SetLocalPosition(Mathf::Lerp(mapIntialPosition, mapIntialPosition + direction, factor));

		//move other objects than tiles 
		//iterate over objects if it has eventcomponent check if it should be moving
		for (auto otherObj : otherObjectsToMove)
		{
			static std::unordered_map<std::shared_ptr<GameObject>, Vector3> initialPositions;
			if (restart)
			{
				initialPositions[otherObj] = otherObj->GetTransform()->GetPosition();
			}

			if (!otherObj->HasComponent<TriggerComponent>() || !otherObj->GetComponent<TriggerComponent>()->GetComponent()->isMoving())
			{
				otherObj->SetLocalPosition(Mathf::Lerp(initialPositions[otherObj], initialPositions[otherObj] + direction, factor));
			}
			else
			{
				Vector3 pos = initialPositions[otherObj];
				pos = otherObj->GetComponent<TriggerComponent>()->getNodePos();
				Vector3 playerPos = tempNodePos;
				Vector3 delta = playerPos - pos;
	
				//Vector3 YOffset = Vector3{ 0,0,0 };
				//if (otherObj->HasComponent<JumperComponent>())
				//{
				//	auto jumper = otherObj->GetComponent<JumperComponent>();
				//	float speed = jumper->GetCurrentSpeed();

				//	//	GetOwner()->GetTransform()->translate(Vector3(0, currentSpeed * tick * 100, 0));
				//	YOffset = Vector3(0, speed*tick * 100, 0);
				//}
				otherObj->SetLocalPosition(Mathf::Lerp(initialPositions[otherObj], initialPositions[otherObj] + delta * 100 + direction + YOffset, factor));

			}

		}
		notify(ON_MOVE);
		tempOffset = direction * 0.01f;
		if (factor == 1) return true;
		return false;

	}

	void MapMovementComponent::onNotify(subject_base* subject_base, subject_base::event_t event)
	{
	}


}
