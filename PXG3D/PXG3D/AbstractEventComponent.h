#pragma once
#include "Component.h"
#include "PXGNode.h"
namespace PXG
{
	class AbstractEventComponent : public Component
	{
	public:

		virtual void Execute(Node* currentNode, Node* targetNode) = 0;
		void ChangeMove();
		bool isMoving() { return move; }
	private:
		bool move = false;
	};


}