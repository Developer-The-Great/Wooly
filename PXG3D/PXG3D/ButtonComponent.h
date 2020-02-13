#pragma once
#include "Component.h"
#include "Input.h"
#include "Vector3.h"
#include "Transform.h"
#include "KeyCode.h"
#include "Subject.h"
namespace PXG
{


	class ButtonComponent : public Component,public subject_base
	{
	public:

		enum buttonEvent: event_t
		{
			ON_CLICK
		};

		ButtonComponent();
		
		~ButtonComponent() override;

		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;


	private:
		
		buttonEvent onClickEvent = ON_CLICK;

	};

}

