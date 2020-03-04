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
			//getting ray cast info
			HitInfo info = raycaster->GetLastHit();
			auto hitObject = info.GameObjectHit;
			Vector3 playerPos = mapMovement->getOffset();
			//check if target has a node
			if (hitObject->GetComponent<Node>() && nodeGraph)
			{
				if (Node* startNode = getStartNode(playerPos))
				{
					auto endNode = hitObject->GetComponent<Node>().get();
					//gen path
					auto result = FindPath(translatedGraph, startNode, endNode);
					//check if path is found
					handleResult(result, endNode);
				}
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
	void RayCastHitHandler::handleResult(std::pair<bool, std::shared_ptr<std::vector<PathFindingNode*>>> result, Node* endNode)
	{
		//check for path found
		if (result.first)
		{
			//check if node is interactable  and trigger; path length ==2 -> node is next to player position 
			if (result.second->size() == 2 && endNode->GetNodeWheight() >= 2000)
			{
				//find gameobject at node position
				for (auto otherObject : nodeGraph->GetObjects())
				{
					if (otherObject->HasComponent<TriggerComponent>())
					{
						Vector3 comparePos = otherObject->GetComponent<TriggerComponent>()->getNodePos();
						//if has trigger and position is same raise the trigger
						if (comparePos == endNode->getPos())
						{
							auto trigger = otherObject->GetComponent < TriggerComponent>();
							trigger->onNotify(this, RAY_CAST_INTERACTIVE_HIT);
							continue;
						}
					}
				}
			}
			//move 
			else
			{
				mapMovement->Move(translatePath(result.second.get()));
			}
		}
	}
	Node * RayCastHitHandler::getStartNode(Vector3 position)
	{
		for (auto startNode : nodeGraph->GetNodes())
		{
			Debug::Log("getPos {0} ", startNode->getPos().ToString());
			Debug::Log("position {0} ", position.ToString());

			if (Mathf::FloatVectorCompare(startNode->getPos() ,position))
			{
				return startNode;
			}
		}
		return nullptr;
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