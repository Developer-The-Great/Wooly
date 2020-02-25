#pragma once
#include "NodeGraph.h"
#include <algorithm>

namespace PXG
{

	void NodeGraph::Clear() 
	{
		nodes.clear();
	}
	std::vector<Node*> NodeGraph::GetNodes()
	{
		return nodes;
	}
	void NodeGraph::generateConnections()
	{

		for (Node* OuterNode : nodes)
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
		}
	}
	void NodeGraph::AddNode(Node * newNode)
	{
		nodes.push_back(newNode);
	}

	void NodeGraph::Start()
	{
	}

	void NodeGraph::FixedUpdate(float tick)
	{
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