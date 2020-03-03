#include"FollowPlayerComponent.h"
namespace PXG
{

	FollowPlayerComponent::FollowPlayerComponent()
	{
	}

	FollowPlayerComponent::~FollowPlayerComponent()
	{
	}
	void FollowPlayerComponent::FixedUpdate(float pStep)
	{
		if (following)
		{
		}
		else
		{
		}
	}

	void FollowPlayerComponent::Execute(Node * currentNode, Node * targetNode)
	{
	//	this->isMoving = !this->isMoving;
		ChangeMove();
		Debug::Log("Execute");
	}
}