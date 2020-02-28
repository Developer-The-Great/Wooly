#pragma once
#include "Component.h"
#include "Subscriber.h"
namespace PXG
{
	struct Vector3;

	class JumperComponent :
		public Component, public subscriber_base
	{
	public:

		virtual void FixedUpdate(float tick) override;

		void SetJumpSpeed(float newSpeed);

		void Jump();


	private:

		double timePassed;

		float initialheight = 100.0f;

		double speed = 40.0f;
		float currentSpeed;

		float jumpHeight = 40.0f;

		float airTimeGiven = 1.5f;
		float airTime;

		bool isJumping = false;;

		virtual void onNotify(subject_base * subject_base, subject_base::event_t event) override;

	};
}


