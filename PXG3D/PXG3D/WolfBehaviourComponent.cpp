#include "WolfBehaviourComponent.h"
#include "GameObject.h"
#include "RayCastHitHandler.h"
#include "World.h"
#include "CameraComponent.h"
#include "NodeGraph.h"
#include "Tile.h"

namespace PXG
{
	WolfBehaviourComponent::WolfBehaviourComponent( Vector3 gridPos , std::shared_ptr<MapMovementComponent> mapMovement)
	{
		wolfGridPos = gridPos;
		wolfGridPos.y -= 1.0f;


		this->mapMovement = mapMovement;
	}

	void WolfBehaviourComponent::Start()
	{
		//im sry for this I need the raycast hit handler
		raycastHitHandler = GetOwner()->GetWorld().lock()->GetCamera()->GetOwner()->GetComponent<RayCastHitHandler>();

		//------get all nodes in front of wolf--------//

		Vector3 front = Vector3(0, 0, 1);
		Vector3 back = Vector3(0, 0, -1);
		Vector3 right = Vector3(1, 0, 0);
		Vector3 left = Vector3(-1, 0, 0);

		Vector3 wolfForward = GetOwner()->GetTransform()->GetForward();

		Vector3 forwardUsed;
		float dotProductResult = -FLT_MAX;
		
		if (float dot = Mathf::Dot(wolfForward, front) > dotProductResult)
		{
			forwardUsed = front;
			dotProductResult = dot;

		}
		
		if (float dot = Mathf::Dot(wolfForward, back) > dotProductResult)
		{
			forwardUsed = back;
			dotProductResult = dot;

		}

		if (float dot = Mathf::Dot(wolfForward, right) > dotProductResult)
		{
			forwardUsed = right;
			dotProductResult = dot;

		}

		if (float dot = Mathf::Dot(wolfForward, left) > dotProductResult)
		{
			forwardUsed = left;
			dotProductResult = dot;

		}

		step = forwardUsed;

		Node* wolfNode =  raycastHitHandler->getStartNode(wolfGridPos);
		
		if (!wolfNode) { return; }

		Debug::Log("forward used {0}", forwardUsed.ToString());

		
		SetNodesTracked();



		for (const auto& node : nodesToCheck)
		{
			Debug::Log("tracking node at {0}", node->getPos().ToString());
		}

	}

	void WolfBehaviourComponent::FixedUpdate(float tick)
	{

		if (isMoving)
		{
			Vector3 currentPosition = GetOwner()->GetTransform()->GetPosition();

			float YOffset = 100.0f;

			Vector3 worldTargetWithOffsets = worldTarget - 
				(mapMovement->getOffset() * Tile::SIZE) 
				+ Vector3(50, YOffset, 50);

			currentPosition.x = Mathf::Lerp(currentPosition.x, worldTargetWithOffsets.x, 0.1f);
			currentPosition.z = Mathf::Lerp(currentPosition.z, worldTargetWithOffsets.z, 0.1f);
			

			GetOwner()->GetTransform()->SetLocalPosition(currentPosition + Vector3(00.0f,0,00.0f));

			if (Vector3::Distance(currentPosition, worldTargetWithOffsets) < distanceTolerance)
			{
				isMoving = false;
				currentPosition = worldTargetWithOffsets;
			}

		}


	}

	void WolfBehaviourComponent::Execute(Node * currentNode, Node * targetNode)
	{
	}

	void WolfBehaviourComponent::onNotify(subject_base * subject_base, subject_base::event_t event)
	{

		if (event == MapMovementComponent::ON_MOVE_FINISHED)
		{
			Debug::Log("On move called on Wolf");

			//get player grid position
			
			
			//get player node
			Node * playerNode = raycastHitHandler->getStartNode(mapMovement->getOffset());

			//if(playerNode)
			bool isPlayerInRange = false;
			for (auto const& node : nodesToCheck)
			{
				if (node == playerNode)
				{
					Debug::Log("Player detected");
					isPlayerInRange = true;
					break;
				}
			}

			if (isPlayerInRange)
			{
				

				Node * previousNode = GetCurrentWolfNode();

				if (!previousNode) { return; }

				previousNode->SetNodeWeight(1);

				wolfGridPos = wolfGridPos + step;

				Node * afterNode = GetCurrentWolfNode();
				if (!afterNode) 
				{ 
					wolfGridPos = wolfGridPos - step;
					return; 
				}

				afterNode->SetNodeWeight(IMPOSSIBLE_REALM(1));


				worldTarget.x = wolfGridPos.x * Tile::SIZE;
				worldTarget.y = wolfGridPos.y * Tile::SIZE;
				worldTarget.z = wolfGridPos.z * Tile::SIZE;

				isMoving = true;


				if (afterNode == playerNode)
				{
					Debug::Log("LOST!");
				}

				SetNodesTracked();
			}

			//check if 


		}
	}

	void WolfBehaviourComponent::SetNodesTracked()
	{
		nodesToCheck.clear();
		Vector3 positionToCheck = wolfGridPos;
		for (int i = 0; i < wolfMoveRange; i++)
		{

			//check position
			Node* newNode = raycastHitHandler->getStartNode(positionToCheck);

			//if there is a node there
			if (newNode)
			{
				nodesToCheck.push_back(newNode);
			}
			else
			{
				break;
			}

			//add the position
			positionToCheck = positionToCheck + step;
		}
	}

	Node * WolfBehaviourComponent::GetCurrentWolfNode()
	{
		return raycastHitHandler->getStartNode(wolfGridPos);
	}
}

