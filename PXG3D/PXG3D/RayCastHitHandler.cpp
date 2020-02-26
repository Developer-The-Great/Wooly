#include "RayCastHitHandler.h"
namespace PXG
{

	RayCastHitHandler::RayCastHitHandler()
	{
	}

	RayCastHitHandler::~RayCastHitHandler()
	{
	}
	void RayCastHitHandler::onNotify(subject_base * subjectBase, subject_base::event_t event)
	{
		RayCastShooter* raycaster = nullptr;
		if (raycaster == subjectBase && raycaster != nullptr || (raycaster == nullptr && (raycaster = dynamic_cast<RayCastShooter*>(subjectBase))))
		{
			switch (event)
			{
			case RayCastShooter::ON_RAYCAST_HIT:
			{
				Debug::Log("getting ray Cast");
				HitInfo info = raycaster->GetLastHit();

				if (info.GameObjectHit->GetComponent<Node>())
				{
					Debug::Log("found attached node");
					if (nodeGraph != nullptr)
					{
						Vector3 playerPos = mapMovement->getOffset();
						Debug::Log("PLayer position:");
						Debug::Log(playerPos.ToString());
						for (auto it : nodeGraph->GetNodes())
						{
							if (playerPos == it->getPos())
							{
								Debug::Log("found playerNode");
								Debug::Log(std::to_string(info.GameObjectHit->GetComponent<Node>()->GetConnectedNodes().capacity()));

							}
						}
						//	auto result = FindPath()
						//	Debug::Log("got node graph");
					}
					else
					{
						Debug::Log("node graph not set");
					}
				}
			}

			default:
				break;
			}
		}
	}
	void RayCastHitHandler::setNodeGraph(std::shared_ptr<NodeGraph> newGraph)
	{
		nodeGraph = newGraph;

		auto graph = nodeGraph->GetNodes();


		translatedGraph = TranslateNodeGraph(graph);

		//auto result = FindPath(translatedGraph, startNode, endNode);*/
	}
	void RayCastHitHandler::setMapMovement(std::shared_ptr<MapMovementComponent> newMap)
	{
		mapMovement = newMap;
	}
}