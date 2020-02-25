#pragma once
#include "Component.h"
#include "Node.h"
namespace PXG
{
	class AbstractEventComponent : public Component
	{
	public:

		virtual void Execute(Node* currentNode, Node* targetNode) = 0;
	private:

	};


}