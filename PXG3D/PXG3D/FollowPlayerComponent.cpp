#pragma once 
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
			Debug::Log("Following");
			//Do player follow stuff
		}
		else
		{
			Debug::Log("Not Following");

		}
	}

	void FollowPlayerComponent::Execute(Node * currentNode, Node * targetNode)
	{
		if (following) following = false;
		else following = true;
		Debug::Log("Execute");


	}
}