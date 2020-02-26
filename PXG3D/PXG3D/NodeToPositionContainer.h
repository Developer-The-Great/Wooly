#pragma once

#include "PXGNode.h"
#include <memory>

namespace PXG
{
	struct NodeToPositionContainer
	{
		int x, y, z;
		std::shared_ptr<Node> node;


	};
}
