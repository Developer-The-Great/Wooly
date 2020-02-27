#include "AbstractEventComponent.h"

void PXG::AbstractEventComponent::ChangeMove()
{
	if (move) move = false;
	else move = true;
}
