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
			case FORWARD:	result = move({ 0, 0, 1 }, restart, timer, tick, FORWARD); break;
			case BACKWARD:  result = move({ 0 ,0,-1 }, restart, timer, tick, BACKWARD); break;
			case UP:		result = move({ 0, 1, 0 }, restart, timer, tick, UP); break;
			case DOWN:		result = move({ 0,-1, 0 }, restart, timer, tick, DOWN); break;
			case LEFT:		result = move({ 1, 0, 0 }, restart, timer, tick, LEFT); break;
			case RIGHT:		result = move({ -1, 0, 0 }, restart, timer, tick, RIGHT); break;
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

	void MapMovementComponent::AddSheep(std::shared_ptr<GameObject> newObject)
	{
		sheeps.push_back(newObject);
	}

	bool MapMovementComponent::move(PXG::Vector3 direction, bool restart, float factor, float tick, MovementCommands command)
	{
		currentDir = direction;
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
		int sheepIndex = 0;
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
				Vector3 pos = otherObj->GetComponent<TriggerComponent>()->getNodePos();
				Vector3 playerPos = Vector3{ 0,0,0 };
				if (sheepIndex == 0)
				{
					playerPos = tempNodePos;
				}
				else
				{
					playerPos = lastSheepPos;
				}

				Vector3 delta = playerPos - pos;

				float Ydelta = 0;
				if (otherObj->HasComponent<JumperComponent>())
				{
					auto jumper = otherObj->GetComponent<JumperComponent>();
					float speed = jumper->GetCurrentSpeed();
					Ydelta = speed * tick * 2 * 100;
					jumper->height += Ydelta;
					jumper->height = Mathf::Lerp(jumper->height, 0, factor);
					Ydelta = jumper->height;
				}
				float Xdelta = Mathf::Lerp(initialPositions[otherObj].x, initialPositions[otherObj].x + delta.x * 100 + direction.x, factor);
				float Zdelta = Mathf::Lerp(initialPositions[otherObj].z, initialPositions[otherObj].z + delta.z * 100 + direction.z, factor);



				//Vector3 offSet = Vector3(Xdelta, 0, Zdelta);

				Vector3 forward = otherObj->GetComponent<TriggerComponent>()->getForward();



				Vector3 dir;
				switch (command)
				{
				case FORWARD: {dir = Vector3(0, 0, -1); break; }
				case UP: {dir = Vector3(0, 0, 0); break; }
				case DOWN: {dir = Vector3(0, 0, 0); break; }
				case LEFT: {dir = Vector3(1, 0, 0); break; }
				case RIGHT: {dir = Vector3(-1, 0, 0); break; }
				case BACKWARD: {dir = Vector3(0, 0, 1); break; }
				default:
					break;
				}
				Quaternion originalRot = otherObj->GetTransform()->GetRotation();
				//originalRot = otherObj->GetComponent<TriggerComponent>()->oldRot;

				float angle = dir.getAngle(dir, forward);
				//target quat 
				Quaternion quat = glm::angleAxis(glm::radians(angle), glm::vec3(0, 1, 0));
				//Quaternion newRot = Mathf::Slerp(originalRot, quat, factor);
				otherObj->GetTransform()->SetRotation(quat);

				otherObj->SetLocalPosition(Vector3(Xdelta, initialPositions[otherObj].y + Ydelta, Zdelta));

				sheepIndex++;
				lastSheepPos = otherObj->GetComponent<TriggerComponent>()->getNodePos();
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
