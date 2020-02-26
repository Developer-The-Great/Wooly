#include "Node.h"
namespace PXG
{
	Node::Node()
	{
	}

	Node::~Node()
	{
	}

	void Node::initNode(Vector3 newPos, int weight)
	{
		gridPos = newPos;
		nodeWeight = weight;
	}

	int Node::GetNodeWheight()
	{
		return nodeWeight;
	}

	void Node::SetNodeWeight(int newWeight)
	{
		nodeWeight = newWeight;
	}

	std::vector<Node*>& Node::GetConnectedNodes()
	{
		return connectedNodes;

	}

	void Node::AddNewConnection(Node * newNode)
	{
		Debug::Log("NEW CONNECTION");
		if(!connectedNodes.empty())
		{
			if(std::find(connectedNodes.begin(), connectedNodes.end(), newNode) !=connectedNodes.end())
			{
				Debug::Log("Node already contains Node trying to add");
			}
			else
			{

				AddConnection(newNode);
			}
		}
		else
		{
			AddConnection(newNode);
		}

	}

	void Node::AddNewConnection(Vector3  otherNodePosition)
	{

		if (!connectedNodesPosition.empty())
		{
			if (std::find(connectedNodesPosition.begin(), connectedNodesPosition.end(), otherNodePosition) != connectedNodesPosition.end())
			{
				Debug::Log("Node already contains Node trying to add");
			}
			else
			{
				AddConnection(otherNodePosition);
			}
		}
		else
		{
			AddConnection(otherNodePosition);
		}
	}

	std::vector<Vector3>& Node::getConnectionsPositions()
	{
		return connectedNodesPosition;
	}

	Vector3 Node::getPos()
	{
		return gridPos;
	}

	void Node::AddConnection(Node * newNode)
	{
		Debug::Log("adding Connection");

		connectedNodes.push_back(newNode);
	}

	void Node::AddConnection(Vector3 newNode)
	{
		connectedNodesPosition.push_back(newNode);
	}

}