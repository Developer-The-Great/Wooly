#include "Component.h"
#include "GameObject.h"

namespace PXG
{
	Component::Component(GameObject * owner)
	{
		this->owner = owner;
	}

}
