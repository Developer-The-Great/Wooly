#include "WolfBehaviourComponent.h"
#include "GameObject.h"
#include "RayCastHitHandler.h"
#include "World.h"
#include "CameraComponent.h"
#include "NodeGraph.h"

namespace PXG
{
	WolfBehaviourComponent::WolfBehaviourComponent(std::shared_ptr<NodeGraph> nodeGraph, Vector3 gridPos)
	{
		wolfGridPos = gridPos;
		wolfGridPos.y -= 1.0f;


		this->nodeGraph = nodeGraph;
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

		

		Node* wolfNode =  raycastHitHandler->getStartNode(wolfGridPos);
		
		if (!wolfNode) { return; }


		
		
		Debug::Log("forward used {0}", forwardUsed.ToString());

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

			//add the position
			positionToCheck = positionToCheck + forwardUsed;
		}



		for (const auto& node : nodesToCheck)
		{
			Debug::Log("tracking node at {0}", node->getPos().ToString());
		}

	}

	void WolfBehaviourComponent::Execute(Node * currentNode, Node * targetNode)
	{
	}

	void WolfBehaviourComponent::onNotify(subject_base * subject_base, subject_base::event_t event)
	{

		//if(event == )
		//if player is in selected nodes
			//move in direction
		if (event == MapMovementComponent::ON_MOVE_FINISHED)
		{
			Debug::Log("On move called on Wolf");

			//get player grid position

			//get player node


			//check if 


		}
	}
}

