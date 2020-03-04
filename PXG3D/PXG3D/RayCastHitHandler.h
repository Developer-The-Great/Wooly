#pragma once
#include "RayCastShooter.h"
#include "Component.h"
#include "Subscriber.h"
#include "NodeGraph.h"
#include "PathFinder.hpp"
#include <vector>
#include "MapMovementComponent.h"
#include "TriggerComponent.h"
namespace PXG
{
	class RayCastHitHandler : public Component, public subscriber_base, public subject_base
	{
	public:
		enum event : event_t
		{
			RAY_CAST_INTERACTIVE_HIT
		};
		RayCastHitHandler();
		~RayCastHitHandler();

		void onNotify(subject_base* subjectBase, subject_base::event_t event) override;
		void setNodeGraph(std::shared_ptr<NodeGraph> newGraph);
		void setMapMovement(std::shared_ptr<MapMovementComponent> newMap);

		Node* getLastTarget() { return lastTargetNode; };

		class NodeGraphAccessor
		{
		protected:
			static std::vector<PathFindingNode>& GetTranslatedGraph(RayCastHitHandler& hh);
		};


	private:
		friend class NodeGraphAccessor;

		void handleResult(std::pair < bool, std::shared_ptr<std::vector<PathFindingNode*>>> result, Node* endNode);
		Node* getStartNode(Vector3 playerPos);
		std::shared_ptr<NodeGraph> nodeGraph;
		std::vector<PathFindingNode> translatedGraph;
		std::shared_ptr<MapMovementComponent> mapMovement;
		std::vector<Node*>* translatePath(std::vector<PathFindingNode*>* oldPath);
		Node* lastTargetNode;
	};

}