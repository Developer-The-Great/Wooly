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
				//getting ray cast info
				Debug::Log("getting ray Cast");
				HitInfo info = raycaster->GetLastHit();
				auto hitObject = info.GameObjectHit;
				//check if target has a node
				if (info.GameObjectHit->GetComponent<Node>())
				{
					Debug::Log("found attached node");
					//make sure nodegraph exists
					if (nodeGraph != nullptr)
					{
						Vector3 playerPos = mapMovement->getOffset();
						//look for current player node
						for (auto startNode : nodeGraph->GetNodes())
						{
							if (playerPos == startNode->getPos())
							{
								Debug::Log("found playerNode");
								//gen path
								auto result = FindPath(translatedGraph, startNode, hitObject->GetComponent<Node>().get());
								if (result.first)
								{
									if (result.second->size() == 1)
									{
										if (hitObject->HasComponent<TriggerComponent>())
										{
											Debug::Log("ray cast target has trigger");

										}
										else
										{
											Debug::Log("ray cast target has no trigger");

										}
									}
									Debug::Log("found path");
								}
								else
								{
									Debug::Log("no path found");

								}
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