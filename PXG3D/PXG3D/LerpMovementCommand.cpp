#include "LerpMovementCommand.h"
#include "Mathf.h"
#include "GameObject.h"

namespace PXG
{
	LerpMovementCommand::LerpMovementCommand(Component * requestorComponent) : AbstractMovementCommand(requestorComponent)
	{

	}

	void LerpMovementCommand::Execute()
	{
		//gameObject->GetTransform()->GetPosition();
		Vector3 position = gameObject->GetTransform()->GetLocalPosition();

		Vector3 newPosition = Mathf::Lerp(position, target, t);

		gameObject->GetTransform()->SetLocalPosition(newPosition);

	}

	void LerpMovementCommand::UnExecute()
	{
		Vector3 position = gameObject->GetTransform()->GetLocalPosition();

		Vector3 newPosition = Mathf::Lerp(position, start, t);

		gameObject->GetTransform()->SetLocalPosition(newPosition);


	}

	void LerpMovementCommand::SetTarget(Vector3 newTarget)
	{
		target = newTarget;
	}

	void LerpMovementCommand::SetStart(Vector3 newStart)
	{
		start = newStart;
	}

}
