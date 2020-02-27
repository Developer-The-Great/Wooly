#pragma once
#include "AbstractCommand.h"
#include <memory>
#include "Vector3.h"
namespace PXG
{
	class Component;
	class GameObject;


	class AbstractMovementCommand : public AbstractCommand
	{
	public:

		AbstractMovementCommand(Component* movementRequestor);

		void SetDirection(Vector3 dir);


		virtual void Execute() = 0;
		virtual void UnExecute() = 0;

	protected:

		std::shared_ptr<GameObject> gameObject;

		Vector3 direction = Vector3(0,1,0);


	};
}


