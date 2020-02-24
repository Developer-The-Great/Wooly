#pragma once
#include <vector>
#include "Debug.h"
#include "Vector2.h"
#include "Vector3.h"
namespace PXG
{
	class Node
	{
	public:
		Node();
		~Node();
		void initNode(Vector3 newPos, int weight=1);
		int GetNodeWheight();
		void SetNodeWeight(int newWeight);
		std::vector<Node*>& getConnectedNodes();
		void AddNewConnection(Node* newNode);
		void AddNewConnection(Vector3 otherNodePosition);
		std::vector<Vector3>& getConnectionsPositions();
		Vector3 getPos();

	private:
		void AddConnection(Node* newNode);
		void AddConnection(Vector3 newNode);
		int nodeWeight;
		std::vector<Node*> connectedNodes;
		std::vector<Vector3> connectedNodesPosition;
		Vector3 gridPos;
	};


}