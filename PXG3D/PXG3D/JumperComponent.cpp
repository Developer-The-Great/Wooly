#include "JumperComponent.h"
#include "Vector3.h"
#include "Time.h"
//#include "PhysicsEngine.h"
#include "Mathf.h"
#include "GameObject.h"
#include "Input.h"
#include "KeyCode.h"
#include "MapMovementComponent.h"

namespace PXG
{
	void JumperComponent::FixedUpdate(float tick)
	{
		if (Input::GetKeyDown(KeyCode::Z))
		{
			

		}
	
		if (isJumping)
		{
			
			timePassed += tick;

			if(!isStatic)
			{
				GetOwner()->GetTransform()->translate(Vector3(0, currentSpeed * tick * 100, 0));

			}

			currentSpeed += PhysicsEngine::GetGravity() * tick * 10;

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
			//currentSpeed = speed;
			airTime = 2* airTimeGiven;
		}

	}
	void JumperComponent::onNotify(subject_base * subject_base, subject_base::event_t event)
	{

		if(event==MapMovementComponent::ON_MOVE_START)
		{
			currentSpeed = speed;
			Jump();
			initialheight = GetOwner()->GetTransform()->GetLocalPosition().y;
		}
	}
}

