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
			case FORWARD:	result = move({ 0, 0, 1 }, restart, timer); break;
			case BACKWARD:  result = move({ 0 ,0,-1 }, restart, timer); break;
			case UP:		result = move({ 0, 1, 0 }, restart, timer); break;
			case DOWN:		result = move({ 0,-1, 0 }, restart, timer); break;
			case LEFT:		result = move({ 1, 0, 0 }, restart, timer); break;
			case RIGHT:		result = move({ -1, 0, 0 }, restart, timer); break;
			default:;
			}
			restart = result;
			if (result) {
				restart = true;
				timer = 0;
				Debug::Log("----------------------------------------");
				Debug::Log("done moving");
				Debug::Log("----------------------------------------");

				//change offset last after notifying

				/*Debug::Log("offset");
				Debug::Log(offset.ToString());*/
				tempNodePos = tempNodePos - tempOffset;
			/*	Debug::Log("temp node pos");
				Debug::Log(tempNodePos.ToString());*/
				notify(ON_MOVE_FINISHED);

				oldOffset = offset;
			/*	Debug::Log("oldoffset");
				Debug::Log(tempOffset.ToString());*/

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

	bool MapMovementComponent::move(PXG::Vector3 direction, bool restart, float factor)
	{
		if (factor > 1)
			factor = 1;
		Debug::Log("moving");
		Debug::Log("offset / player pos:");
		Debug::Log(offset.ToString());

		static Vector3 mapIntialPosition = map->GetTransform()->GetPosition();
		if (restart)
		{
			mapIntialPosition = map->GetTransform()->GetPosition();
		}


		Transform* mapTransform = map->GetTransform();
		direction = direction * 100;
		mapTransform->SetLocalPosition(Mathf::Lerp(mapIntialPosition, mapIntialPosition + direction, factor));
		//Vector3 otherDir

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


				//Debug::Log("dir");
				//Debug::Log(direction.ToString());
				//Vector3 otherDir = (direction);
				//Debug::Log("dir pos");
				////Debug::Log(otherObj->GetComponent<TriggerComponent>()->getNodePos().ToString());
				//Debug::Log(initialPositions[otherObj].ToString());
				Vector3 pos = initialPositions[otherObj];
				pos = otherObj->GetComponent<TriggerComponent>()->getNodePos();
			/*	Debug::Log("direction");
				Debug::Log(direction.ToString());
				Debug::Log("pos");
				Debug::Log(pos.ToString());*/
				/*Vector3 playerPos = oldOffset;
				playerPos = tempNodePos;*/
				Vector3 playerPos = tempNodePos;
				/*Debug::Log("player pos");
				Debug::Log(playerPos.ToString());
				Debug::Log("delta");*/
				Vector3 delta = playerPos - pos;
				//Debug::Log((playerPos - pos).ToString());
				//delta.y += 1;
				/*Debug::Log("old pos");
				Debug::Log(otherObj->GetTransform()->GetPosition().ToString());*/
				otherObj->SetLocalPosition(Mathf::Lerp(initialPositions[otherObj], initialPositions[otherObj] + delta * 100 + direction, factor));
			/*	Debug::Log("new pos");
				Debug::Log(otherObj->GetTransform()->GetPosition().ToString());*/
			}

		}
		notify(ON_MOVE);
		tempOffset = direction*0.01f;
		if (factor == 1) return true;
		return false;

	}

	void MapMovementComponent::onNotify(subject_base* subject_base, subject_base::event_t event)
	{
	}


}
