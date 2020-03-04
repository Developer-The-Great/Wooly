#pragma once
#include <vector>
#include "Vector3.h"
#include "Component.h"
#include <functional>

namespace PXG
{
	enum class NodeType
	{
		Ramp,
		Ladder,
		FlatTile
	};

	enum class NodeConnectionDirection
	{
		XPos,
		XNeg,
		YPos,
		YNeg,
		ZPos,
		ZNeg
	};
}

namespace PXG
{
	class Node : public Component
	{
	public:

		void initNode(Vector3 newPos, int weight=1);
		int GetNodeWheight();
		void SetNodeWeight(int newWeight);
		std::vector<Node*>& GetConnectedNodes();
		void AddNewConnection(Node* newNode);
		void AddNewConnection(Vector3 otherNodePosition);
		std::vector<Vector3>& getConnectionsPositions();
		Vector3 getPos();

		[[nodiscard]] std::shared_ptr<GameObject> GetGameObject() const
		{
			return GetOwner();
		}

		void SetLadderConnectionDirection(Vector3 direction);

		NodeType GetNodeType() const;

		bool Processed = false;
		//returns true if the nodeX and otherNodeX are valid connections 
		bool IsPositionValidConnection(int nodeX,int nodeY,int nodeZ,int otherNodeX,int otherNodeY,int otherNodeZ, NodeType  otherNodeType);
		
		void SetNodeTypeTo(NodeType newType);

	private:
		//returns true if the a given nodeX and otherNode are next to each other in either the X direction of the Y Direction
		bool flatTileNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ, NodeType  otherNodeType);
		//returns true if the given nodeX and otherNode are either 1 step above or below each other and their positions create a vector that is equal 
		//to rampForward
		bool rampNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ, NodeType otherNodeType);

		bool ladderNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ, NodeType  otherNodeType);

		void addNodeToDirection(int& x, int& y, int& z, NodeConnectionDirection direction);

		void AddConnection(Node* newNode);
		void AddConnection(Vector3 newNode);

		int nodeWeight;
		std::vector<Node*> connectedNodes;
		std::vector<Vector3> connectedNodesPosition;
		Vector3 gridPos;

		NodeType nodeType = NodeType::FlatTile;
		NodeConnectionDirection connectionDirection = NodeConnectionDirection::XNeg;

		bool (Node::*positionCheckFuncPtr)(int, int, int, int, int,int, NodeType otherNodeType);


	};


}
