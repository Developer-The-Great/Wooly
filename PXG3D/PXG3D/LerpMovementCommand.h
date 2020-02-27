#pragma once
#include "AbstractMovementCommand.h"

namespace PXG
{
	class LerpMovementCommand : public AbstractMovementCommand
	{
	public:

		LerpMovementCommand(Component* requestorComponent);

		virtual void Execute() override;

		virtual void UnExecute() override;

		void SetTarget(Vector3 newTarget);

		void SetStart(Vector3 newStart);

	private:
		
		Vector3 target;
		Vector3 start;

		float t;


	};

}

