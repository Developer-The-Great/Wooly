#pragma once
#include "Component.h"
#include "Input.h"
#include "Vector3.h"
#include "Transform.h"
#include "KeyCode.h"
namespace PXG
{


	class ButtonComponent : public Component
	{
	public:

		ButtonComponent();
		
		~ButtonComponent() override;

		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;


	private:


	};

}

