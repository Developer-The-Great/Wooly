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
				Debug::Log("-----------------------------------------------");
				Debug::Log("getting ray Cast");
				Debug::Log("-----------------------------------------------");
				HitInfo info = raycaster->GetLastHit();
				auto hitObject = info.GameObjectHit;
				//check if target has a node
				if (hitObject->GetComponent<Node>() && nodeGraph)
				{
					auto endNode = hitObject->GetComponent<Node>().get();
					Vector3 playerPos = mapMovement->getOffset();
					Debug::Log("nodeGraph Length");
					Debug::Log(std::to_string(nodeGraph->GetNodes().size()));
					Debug::Log("player pos:");
					Debug::Log(playerPos.ToString());
					//look for current player node
					
					for (auto startNode : nodeGraph->GetNodes())
					{
						Debug::Log("node pos:");
						Debug::Log(startNode->getPos().ToString());
						//compare position of player with map movement
						if (playerPos == startNode->getPos())
						{
							Debug::Log("found player node");
						
							Debug::Log("target Node");
							Debug::Log(endNode->getPos().ToString());
							Debug::Log("Connecntions of target:");
							for (auto otherNode : endNode->GetConnectedNodes())
							{
								Debug::Log(otherNode->getPos().ToString());
							}
							//gen path
							auto result = FindPath(translatedGraph, startNode, endNode);
							//check if path is found
							if (result.first)
							{
								//check if node is interactable  and trigger; path length ==2 -> node is next to player position 
								if (result.second->size() == 2 && endNode->GetNodeWheight() >= 2000)
								{
									//find gameobject at node position
									for (auto otherObject : nodeGraph->GetObjects())
									{
										Vector3 comparePos = otherObject->GetTransform()->GetPosition();
										//scale position to unit length
										comparePos = comparePos * 0.01f;
										comparePos.y -= 1;
										//if has trigger and position is same raise the trigger
										if (comparePos == endNode->getPos() && otherObject->HasComponent<TriggerComponent>())
										{
											auto trigger = otherObject->GetComponent < TriggerComponent>();
											trigger->raiseTrigger(startNode, endNode);
											break;
										}
									}
								}
								//move 
								else
								{
									mapMovement->Move(translatePath(result.second.get()));
									break;
								}
							}
							else
							{
								Debug::Log("no path found");
								break;
							}
						}
						else
						{
							Debug::Log("next node");
						}
					}
					Debug::Log("could not find player node");

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
	std::vector<Node*>* RayCastHitHandler::translatePath(std::vector<PathFindingNode*>* oldPath)
	{
		Debug::Log("path:");

		int index = 0;
		std::vector<Node*>* newPath = new std::vector<Node*>;
		for (auto oldNode : *oldPath)
		{
			Debug::Log(oldNode->GetRealNode()->getPos().ToString());
			//ignore first node to exclude the start node from the vecotr
			/*if(index!=0)
			{
			}*/
			newPath->push_back(oldNode->GetRealNode());
			index++;
		}
		return newPath;
	}
}