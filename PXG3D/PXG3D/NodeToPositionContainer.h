#pragma once

#include "PXGNode.h"
#include <memory>

namespace PXG
{
	struct NodeToPositionContainer
	{
		NodeToPositionContainer(Vector3 v,std::shared_ptr<Node> node)
		{
			x = static_cast<int>(v.x);
			y = static_cast<int>(v.y);
			z = static_cast<int>(v.z);
			this->node = node;
		}
		
		int x, y, z;
		std::shared_ptr<Node> node;


	};
}
