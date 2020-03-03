#pragma once
#include "NodeGraphDistTrigger.h"
#include "OnClickTrigger.hpp"
#include "Trigger.h"

namespace PXG
{
	using CompoundDistanceOnClickTrigger = TriggerAdder<OnClickTrigger,NodeGraphDistTrigger>;
}
