#include "PXGNode.h"
#include "Mathf.h"
#include "GameObject.h"
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

	NodeType Node::GetNodeType() const
	{
		return nodeType;
	}

	bool Node::IsPositionValidConnection(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ, NodeType  otherNodeType)
	{

		if (positionCheckFuncPtr)
		{
			return (this->*positionCheckFuncPtr)(nodeX,nodeY,nodeZ,otherNodeX,otherNodeY,otherNodeZ,otherNodeType);
		}
		return false;
	}

	void Node::SetNodeTypeTo(NodeType newType)
	{
		nodeType = newType;

		switch (newType)
		{
			case NodeType::FlatTile:
				positionCheckFuncPtr = &Node::rampNodeCheck;
				break;

			case NodeType::Ladder:
				positionCheckFuncPtr = &Node::ladderNodeCheck;
				break;

			case NodeType::Ramp:
				positionCheckFuncPtr = &Node::rampNodeCheck;
				break;

			default:
				break;
		
		}
		
	}

	bool Node::flatTileNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ, NodeType  otherNodeType)
	{
		bool hasSameVerticalHeight = nodeY == otherNodeY;

		//if the nodes are exactly one step left or right and have the same z direction
		if (Mathf::Abs(otherNodeX - nodeX) == 1 && otherNodeZ == nodeZ && hasSameVerticalHeight)
		{
			return true;
		}
		//if the nodes are exactly one step forward or backward and have the same x direction
		if (Mathf::Abs(otherNodeZ - nodeZ) == 1 && otherNodeX == nodeX && hasSameVerticalHeight)
		{
			return true;
		}

		//if node is below otherNode and otherNode is of type ladder
		if ((nodeY - otherNodeY == -1) && (otherNodeType == NodeType::Ladder))
		{
			
			return true;
		}

		return false;
	}

	bool Node::rampNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ, NodeType  otherNodeType)
	{
		Debug::Log(Verbosity::Error, "rampNodeCheck Not implemented!");
		return false;
	}

	bool Node::ladderNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ, NodeType  otherNodeType)
	{
		bool isInSameXZPosition = (nodeX == otherNodeX) && (nodeZ == otherNodeZ);

		//if it is directly bellow the ladder
		if (nodeY - otherNodeY == 1 && isInSameXZPosition)
		{
			return true;
		}

		//if its above the ladder and its of type ladder
		if (nodeY - otherNodeY == -1 && isInSameXZPosition && nodeType == NodeType::Ladder)
		{
			return true;
		}

		//if its one step 
		int targetLadderNodeX = nodeX;
		int targetLadderNodeY = nodeY;
		int targetLadderNodeZ = nodeZ;

		addNodeToDirection(targetLadderNodeX, targetLadderNodeY, targetLadderNodeZ, connectionDirection);

		if ((otherNodeX == targetLadderNodeX) && (otherNodeY == targetLadderNodeY) && (otherNodeZ == targetLadderNodeZ))
		{
			return true;
		}

		return false;

	}

	void Node::addNodeToDirection(int & x, int & y, int & z, NodeConnectionDirection direction)
	{
		switch (direction)
		{
		case NodeConnectionDirection::XNeg:
			x--;
			break;

		case NodeConnectionDirection::XPos:
			x++;
			break;

		case NodeConnectionDirection::YPos:
			y++;
			break;

		case NodeConnectionDirection::YNeg:
			y--;
			break;

		case NodeConnectionDirection::ZPos:
			z++;
			break;

		case NodeConnectionDirection::ZNeg:
			z--;
			break;

		default:
			break;
		}

	}

	void Node::AddConnection(Node * newNode)
	{
		connectedNodes.push_back(newNode);
	}

	void Node::AddConnection(Vector3 newNode)
	{
		connectedNodesPosition.push_back(newNode);
	}

}