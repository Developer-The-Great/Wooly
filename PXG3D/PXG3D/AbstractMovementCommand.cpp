#include "AbstractMovementCommand.h"

#include "Component.h"

namespace PXG
{

	AbstractMovementCommand::AbstractMovementCommand(Component * movementRequestor)
	{
		if (movementRequestor)
		{
			gameObject = movementRequestor->GetOwner();
		}

	}
	void AbstractMovementCommand::SetDirection(Vector3 dir)
	{
		direction = dir;
	}
}

