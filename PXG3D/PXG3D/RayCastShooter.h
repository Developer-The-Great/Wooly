#pragma once
#include "Component.h"
namespace PXG
{
	class RayCastShooter :public Component
	{
	public:

		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;

	private:




	};
}


