#include "AbstractEventComponent.h"

void PXG::AbstractEventComponent::ChangeMove()
{
	move = !move;
	Debug::Log("moving:");
	Debug::Log(std::to_string(move));
}
