#pragma once
#include"Component.h"
#include "Debug.h"
#include "AbstractEventComponent.h"
#include "PXGNode.h"
namespace PXG
{
	class FollowPlayerComponent : public AbstractEventComponent
	{
	public:
		FollowPlayerComponent();
		~FollowPlayerComponent();
		void FixedUpdate(float pStep)override;
		virtual void Execute(Node* currentNode, Node* targetNode) override;
	private:
		bool following = false;
	};

}
