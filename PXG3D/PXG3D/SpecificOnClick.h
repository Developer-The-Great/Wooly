#pragma once
#include "AbstractOnClick.h"
#include "Debug.h"
#include "GameObject.h"
#include "World.h"
#include <vector>
#include "RayCastShooter.h"
namespace PXG
{
	class SpecificOnClick : public AbstractOnClick
	{
	public:
		~SpecificOnClick();
		virtual void Execute()override;
		void setObjectToDisable(std::shared_ptr<GameObject> gam) { targetDisable.push_back(gam); };
		void setObjectToEnable(std::shared_ptr<GameObject> gam) { targetEnable.push_back(gam); };
		void setRay(std::shared_ptr<RayCastShooter > newRay) { raycaster = newRay; };


	private:
		std::vector<std::shared_ptr<GameObject>> targetDisable;
		std::vector<std::shared_ptr<GameObject>> targetEnable;
		std::shared_ptr<RayCastShooter > raycaster;
	};
}