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
	class RayCastHitHandler : public Component, public subscriber_base
	{
	public:
		RayCastHitHandler();
		~RayCastHitHandler();

		void onNotify(subject_base* subjectBase, subject_base::event_t event) override;
		void setNodeGraph(std::shared_ptr<NodeGraph> newGraph);
		void setMapMovement(std::shared_ptr<MapMovementComponent> newMap);
	private:
		std::shared_ptr<NodeGraph> nodeGraph;
		std::vector<PathFindingNode> translatedGraph;
		std::shared_ptr<MapMovementComponent> mapMovement;
	};

}