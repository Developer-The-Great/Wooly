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

			if (mouseX > position.x && mouseX < position.x + scale.x &&
				mouseY > position.y &&mouseY < position.y + scale.z)
			{
				notify(ON_CLICK);
			}
		}
	}
}

