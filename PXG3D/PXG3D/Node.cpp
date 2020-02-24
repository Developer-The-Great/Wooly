#pragma once
#include "Node.h"
namespace PXG
{
	Node::Node()
	{
	}

	Node::~Node()
	{
	}

	int Node::GetNodeWheight()
	{
		return nodeWeight;
	}

	void Node::SetNodeWeight(int newWeight)
	{
		nodeWeight = newWeight;
	}

}