#include "ButtonComponent.h"

namespace PXG
{

	ButtonComponent::ButtonComponent() :Component(), subject_base()
	{
		Debug::Log("created button comp");
	}
	void ButtonComponent::FixedUpdate(float tick)
	{
		if (Input::GetKeyDown(KeyCode::LeftMouse))
		{
			Transform* transform = GetOwnerTransform();
			Vector3 position = transform->GetPosition();
			Vector3 scale = transform->getScale();
			scale = scale * 2;
			float mouseX = Input::GetMouseX();
			float mouseY = Input::GetMouseY();
			mouseY = -mouseY + 600;
		
			Vector3 pScale = this->GetOwnerTransform()->getScale();
			float leftBorder = position.x - pScale.x + 100;
			float botborder = position.y- pScale.z + 100;
			if (mouseX > leftBorder
				&& mouseX < leftBorder+ scale.x &&
				mouseY > botborder &&mouseY < botborder + scale.z)
			{
				notify(ON_CLICK);
			}
		}
	}
}

