#pragma once
#include "Node.h"
#include "Component.h"
namespace PXG
{
	struct NodeToPositionContainer;

	class NodeGraph : public Component
	{
	public:

		void AddNewNode(Node* newNode);
		void Clear();
		std::vector<Node*>& GetNodes();
		void generateConnections(std::vector<NodeToPositionContainer>& nodeToPositionContainers);
	private:

		void AddNode(Node* newNode);

		std::vector<Node*> nodes;
		
	};

	
}
