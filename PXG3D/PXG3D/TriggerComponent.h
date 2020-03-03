#pragma once
#include "Component.h"
#include "Game.h"
#include "PXGNode.h"
#include "FollowPlayerComponent.h"
#include <vector>
#include "Subscriber.h"
#include "MapMovementComponent.h"
#include "NodeGraph.h"
#include "RayCastHitHandler.h"
namespace PXG
{
	class TriggerComponent :public Component, public subscriber_base
	{
	public:
		//triggers all stored components
		void raiseTrigger(Node* currentNode, Node* targetNode);
		//add new components to execute
		void SetComponent(std::shared_ptr<AbstractEventComponent> newComponent);
		std::shared_ptr<AbstractEventComponent> GetComponent();
		void SetNodePos(Vector3 newPos);
		Vector3 getNodePos();
		void moveNodePos(Vector3 dir);
		void setNodePos(Vector3 pos);
		virtual void onNotify(subject_base * subject_base, subject_base::event_t event) override;
		void SetNodeGraph(std::shared_ptr<NodeGraph> newNodeGraph);

	private:

		std::shared_ptr<AbstractEventComponent> component;
		std::shared_ptr<NodeGraph> nodeGraph;
		Vector3 nodePos;
	};
}


