#pragma once
#include "PXGNode.h"
#include "Component.h"
#include "World.h"
namespace PXG
{
	struct NodeToPositionContainer;

	class NodeGraph : public Component
	{
	public:

		void AddNewNode(Node* newNode);
		void AddNewInteractiveObj(std::shared_ptr<GameObject> newObj);
		void Clear();
		std::vector<Node*>& GetNodes();
		std::vector<std::shared_ptr<GameObject>>& GetObjects();
		void generateConnections(std::vector<NodeToPositionContainer>& nodeToPositionContainers);
	private:

		void AddNode(Node* newNode);

		std::vector<Node*> nodes;
		std::vector<std::shared_ptr<GameObject>> interactiveObjects;
		
	};

	
}
