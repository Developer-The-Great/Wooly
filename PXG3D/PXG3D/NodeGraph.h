#pragma once
#include "PXGNode.h"
#include "Component.h"
namespace PXG
{
	class NodeGraph : public Component
	{
	public:

		void AddNewNode(Node* newNode);
		void Clear();
		std::vector<Node*>& GetNodes();
		void generateConnections();
	private:

		void AddNode(Node* newNode);

		std::vector<Node*> nodes;
		
	};

	
}
