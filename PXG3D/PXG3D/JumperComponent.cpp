#include "JumperComponent.h"
#include "Vector3.h"
#include "Time.h"
//#include "PhysicsEngine.h"
#include "Mathf.h"
#include "GameObject.h"
#include "Input.h"
#include "KeyCode.h"

namespace PXG
{
	void JumperComponent::FixedUpdate(float tick)
	{
		if (Input::GetKeyDown(KeyCode::Z))
		{
			currentSpeed = speed;
			Jump();
			initialheight = GetOwner()->GetTransform()->GetLocalPosition().y;

		}
	
		if (isJumping)
		{
			
			timePassed += tick;
			Debug::Log("timePassed {0}",timePassed);
			Debug::Log("speed {0} ", currentSpeed);


			GetOwner()->GetTransform()->translate(Vector3(0, currentSpeed * tick * 10, 0));

			currentSpeed += PhysicsEngine::GetGravity() * tick * 10;


			if (currentSpeed < 0)
			{
				Debug::Log("time {0}", Time::GetTime());
				Debug::Log("height max reached {0}", GetOwner()->GetTransform()->GetLocalPosition().ToString());
				Debug::Log("");

			}

			if (GetOwner()->GetTransform()->GetLocalPosition().y <  initialheight)
			{
				
				Vector3 position = GetOwner()->GetTransform()->GetLocalPosition();
				GetOwner()->GetTransform()->SetLocalPosition(Vector3(position.x, initialheight, position.z));

				timePassed = 0;
				isJumping = false;
			}


		}

		
		

		
	}
	void JumperComponent::SetJumpSpeed(float newSpeed)
	{
		speed = newSpeed;
	}
	void JumperComponent::Jump()
	{
		if (!isJumping)
		{
			
			isJumping = true;


			//speed = (jumpHeight/2.0f - (0.5f * PhysicsEngine::GetGravity() * Mathf::Pow(airTimeGiven, 2.0f))) /airTimeGiven;

			airTime = 2* airTimeGiven;
		}

	}
}

