#include "ButtonComponent.h"

namespace PXG
{

	ButtonComponent::ButtonComponent() :Component()
	{
		Debug::Log("created button comp");
	}
	ButtonComponent::~ButtonComponent()
	{
	}
	void ButtonComponent::Start()
	{
	}
	void ButtonComponent::FixedUpdate(float tick)
	{

		if (Input::GetKeyDown(KeyCode::LeftMouse))
		{
			//	Debug::Log("mouse click");
			Transform* transform = GetOwnerTransform();
			Vector3 position = transform->GetPosition();
			Vector3 scale = transform->getScale();
			scale = scale * 2;
			float mouseX = Input::GetMouseX();
			float mouseY = Input::GetMouseY();
			mouseY = -mouseY + 600;
			Debug::Log("{}", mouseY);
			Debug::Log(position.ToString());
			if (mouseX > position.x && mouseX < position.x + scale.x &&
				mouseY > position.y &&mouseY < position.y + scale.z)
			{
				Debug::Log("Button hit");
			}


		}


	}
}

