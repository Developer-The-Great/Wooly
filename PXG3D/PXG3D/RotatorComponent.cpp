#include "RotatorComponent.h"

namespace PXG
{

	RotatorComponent::RotatorComponent(Vector3 axis, float speed):Component()
	{
		this->speed = speed;
		this->RotationAxis = axis;


	}
	void RotatorComponent::Start()
	{
	}
	void RotatorComponent::FixedUpdate(float tick)
	{


	}
}

