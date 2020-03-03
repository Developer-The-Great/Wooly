#pragma once
#include "Component.h"
#include "PXGNode.h"
#include "Debug.h"
namespace PXG
{
	class AbstractEventComponent : public Component
	{
	public:

		virtual void Execute(Node* currentNode, Node* targetNode) = 0;
		void ChangeMove();
		bool isMoving() { return move; }
		void setMove(bool b) { move = b; }

	protected:
		bool move = false;
	};


}