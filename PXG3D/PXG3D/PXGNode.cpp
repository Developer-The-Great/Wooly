#include "PXGNode.h"
#include "Mathf.h"
namespace PXG
{
	void Node::initNode(Vector3 newPos, int weight)
	{
		positionCheckFuncPtr = &Node::flatTileNodeCheck;
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

	bool Node::IsPositionValidConnection(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ)
	{
		if (positionCheckFuncPtr)
		{
			return (this->*positionCheckFuncPtr)(nodeX,nodeY,nodeZ,otherNodeX,otherNodeY,otherNodeZ);
		}
		return false;
	}

	void Node::SetNodeAsRampNode()
	{
		positionCheckFuncPtr = &Node::rampNodeCheck;
	}

	bool Node::flatTileNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ)
	{
		if (Mathf::Abs(otherNodeX - nodeX) == 1 && otherNodeZ == nodeZ)
		{
			return true;
		}

		if (Mathf::Abs(otherNodeZ - nodeZ) == 1 && otherNodeX == nodeX)
		{
			return true;
		}

		return false;
	}

	bool Node::rampNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ)
	{
		Debug::Log(Verbosity::Error, "rampNodeCheck Not implemented!");
		return false;
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