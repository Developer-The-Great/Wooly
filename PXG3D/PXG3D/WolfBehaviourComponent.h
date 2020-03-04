#pragma once
#include "AbstractEventComponent.h"
#include "Subscriber.h"
#include <memory>
#include <vector>

namespace PXG
{
	class MapMovementComponent;
	class RayCastHitHandler;
	class NodeGraph;

	class WolfBehaviourComponent :public AbstractEventComponent, public subscriber_base
	{
	public:

		WolfBehaviourComponent(std::shared_ptr<NodeGraph> nodeGraph,Vector3 gridPos);

		virtual void Start() override;

		virtual void Execute(Node* currentNode, Node* targetNode) override;

		virtual void onNotify(subject_base * subject_base, subject_base::event_t event) override;

	private:

		int wolfMoveRange = 5;

		Vector3 wolfGridPos;

		std::shared_ptr<MapMovementComponent> mapMovement;
		std::shared_ptr<RayCastHitHandler> raycastHitHandler;
		std::shared_ptr<NodeGraph> nodeGraph;

		std::vector<Node*> nodesToCheck;

	};
}


