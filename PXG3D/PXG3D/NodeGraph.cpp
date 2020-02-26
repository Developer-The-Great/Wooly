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
	void NodeGraph::generateConnections(std::vector<NodeToPositionContainer>& nodeToPositionContainers)
	{
		/*for (Node* OuterNode : nodes)
		{
			for (Node* InnerNode : nodes) {

				auto match = std::find_if(OuterNode->getConnectionsPositions().begin(), OuterNode->getConnectionsPositions().end(), [&](auto pos) {
					return pos == InnerNode->getPos();
				});

				if (match != OuterNode->getConnectionsPositions().end())
				{
					OuterNode->AddNewConnection(InnerNode);
				}
			}
		}*/

		//for each node 
		for (int i = 0; i < nodes.size()-1; i++)
		{
			NodeToPositionContainer firstContainer = nodeToPositionContainers.at(i);

			for (int j=i+1; j < nodes.size();j++)
			{
				
				NodeToPositionContainer secondContainer = nodeToPositionContainers.at(j);

				bool secondContainerValidConnection = firstContainer.node->IsPositionValidConnection(
					firstContainer.x, firstContainer.y, firstContainer.z,
					secondContainer.x, secondContainer.y, secondContainer.z
				);

				if (secondContainerValidConnection)
				{


					firstContainer.node->AddNewConnection(secondContainer.node.get());
					secondContainer.node->AddNewConnection(firstContainer.node.get());
				}

				
			}
		}

			//find node at left 

			//find node at right 

			//find node at forward

			//find node at backward
			



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

}