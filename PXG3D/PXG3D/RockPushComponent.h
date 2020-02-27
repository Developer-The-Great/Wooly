#pragma once
#include "AbstractEventComponent.h"
namespace PXG
{
	class RockPushComponent : public AbstractEventComponent
	{
	public:
	
		virtual void Execute(Node* currentNode, Node* targetNode) override;

	private:

	};


}
