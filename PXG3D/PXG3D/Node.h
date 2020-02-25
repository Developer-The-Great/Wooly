#pragma once
#include <vector>
#include "Vector3.h"
#include "Component.h"

namespace PXG
{
	class Node : public Component
	{
	public:
		Node();
		~Node();
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
		

	private:
		void AddConnection(Node* newNode);
		void AddConnection(Vector3 newNode);
		int nodeWeight;
		std::vector<Node*> connectedNodes;
		std::vector<Vector3> connectedNodesPosition;
		Vector3 gridPos;
	};


}
