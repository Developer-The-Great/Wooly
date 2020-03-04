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

		WolfBehaviourComponent(Vector3 gridPos, std::shared_ptr<MapMovementComponent> mapMovement);

		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;

		virtual void Execute(Node* currentNode, Node* targetNode) override;

		virtual void onNotify(subject_base * subject_base, subject_base::event_t event) override;



	private:

		void SetNodesTracked();

		Node* GetCurrentWolfNode();

		int wolfMoveRange = 5;

		Vector3 wolfGridPos;

		Vector3 step;
		Vector3 worldTarget;
		float distanceTolerance = 1.1f;

		std::shared_ptr<MapMovementComponent> mapMovement;
		std::shared_ptr<RayCastHitHandler> raycastHitHandler;
	

		std::vector<Node*> nodesToCheck;


		bool isMoving = false;

	};
}


