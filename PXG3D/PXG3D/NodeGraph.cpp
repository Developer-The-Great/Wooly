#include "NodeGraph.h"
#include <algorithm>
#include "NodeToPositionContainer.h"

namespace PXG
{

	void NodeGraph::Clear() 
	{
		nodes.clear();
	}
	std::vector<Node*>& NodeGraph::GetNodes()
	{
		return nodes;
	}
	std::vector<std::shared_ptr<GameObject>>& NodeGraph::GetObjects()
	{
		return interactiveObjects;
	}
	void NodeGraph::generateConnections(std::vector<NodeToPositionContainer>& nodeToPositionContainers)
	{

		for (int i = 0; i < nodes.size()-1; i++)
		{
			NodeToPositionContainer firstContainer = nodeToPositionContainers.at(i);

			for (int j=i+1; j < nodes.size();j++)
			{
				NodeToPositionContainer secondContainer = nodeToPositionContainers.at(j);

				NodeType secondNodeType = secondContainer.node->GetNodeType();
				NodeType firstNodeType = firstContainer.node->GetNodeType();

				bool  firstContainerValidConnection = firstContainer.node->IsPositionValidConnection(
					firstContainer.x, firstContainer.y, firstContainer.z,
					secondContainer.x, secondContainer.y, secondContainer.z, 
					secondNodeType
				);

				bool  secondContainerValidConnection = secondContainer.node->IsPositionValidConnection(
					secondContainer.x, secondContainer.y, secondContainer.z,
					firstContainer.x, firstContainer.y, firstContainer.z, 
					firstNodeType
				);

				if (secondContainerValidConnection && firstContainerValidConnection)
				{
					firstContainer.node->AddNewConnection(secondContainer.node.get());
					secondContainer.node->AddNewConnection(firstContainer.node.get());
				}
			}
		}
	}
	void NodeGraph::AddNode(Node * newNode)
	{
		nodes.push_back(newNode);
	}

	
	void NodeGraph::AddNewNode(Node * newNode)
	{
		if (!nodes.empty())
		{
			if (std::find(nodes.begin(), nodes.end(), newNode) != nodes.end())
			{
				Debug::Log("NodeGraph already contains node trying to add");
			}
			else
			{
				Debug::Log("New Node added");
				AddNode(newNode);
			}
		}
		else
		{
			AddNode(newNode);
		}

	}

	void NodeGraph::AddNewInteractiveObj(std::shared_ptr<GameObject > newObj)
	{
		if (!interactiveObjects.empty())
		{
			if (std::find(interactiveObjects.begin(), interactiveObjects.end(), newObj) != interactiveObjects.end())
			{
				Debug::Log("NodeGraph already contains interactive obj trying to add");
			}
			else
			{
				Debug::Log("New interactive obj added");
				interactiveObjects.push_back(newObj);
			}
		}
		else
		{
			interactiveObjects.push_back(newObj);
		}
	}

}