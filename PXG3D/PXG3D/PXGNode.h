#pragma once
#include <vector>
#include "Vector3.h"
#include "Component.h"
#include <functional>

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

		bool Processed = false;

		bool IsPositionValidConnection(int nodeX,int nodeY,int nodeZ,int otherNodeX,int otherNodeY,int otherNodeZ);

		void SetNodeAsRampNode();

	private:
		bool flatTileNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ);
		bool rampNodeCheck(int nodeX, int nodeY, int nodeZ, int otherNodeX, int otherNodeY, int otherNodeZ);

		void AddConnection(Node* newNode);
		void AddConnection(Vector3 newNode);

		int nodeWeight;
		std::vector<Node*> connectedNodes;
		std::vector<Vector3> connectedNodesPosition;
		Vector3 gridPos;

		bool (Node::*positionCheckFuncPtr)(int, int, int, int, int,int);


	};


}
