#pragma once
#include "Input.h"
#include "Trigger.h"
#include "KeyCode.h"

namespace PXG
{
	class OnClickTrigger : public Trigger
	{

	public:
		void SetSubject(GameObj subj,std::shared_ptr<RayCastShooter> ray_cast)
		{
			subject = subj;
			shooter = ray_cast;
		}

		
	protected:
		bool triggerCondition() override
		{
			if(subject)
			{
				if(shooter->GetLastHit().GameObjectHit == subject)
				{
					return Input::GetKeyUp(KeyCode::LeftMouse);
				}
				return false;
			}

			return Input::GetKeyUp(KeyCode::LeftMouse);
		}

	private:
		GameObj subject;
		std::shared_ptr<RayCastShooter> shooter;
		
		
	};
}
