#pragma once
#include "Component.h"
#include "Subscriber.h"
#include "MapMovementComponent.h"
#include "Debug.h"
namespace PXG
{
	class PlayerRotatorComponent : public Component, public subscriber_base
	{
	public:
		virtual void onNotify(subject_base * subject_base, subject_base::event_t event) override;
		void setInitForward();
	private:
		Vector3 lastDir = Vector3{ 0,0,0 };
		void rotate(Vector3 dir);
		Vector3 forward = Vector3{ 0,0,0 };

	};



}