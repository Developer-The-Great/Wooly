#pragma once
#include "Component.h"
#include "GLMHeaders.h"
#include "Vector3.h"
namespace PXG
{

	class MapMovementComponent : public Component
	{
	public:
		MapMovementComponent();
		~MapMovementComponent();
		virtual void Start() override;
		virtual void FixedUpdate(float tick) override;
		void SetMap(std::shared_ptr<GameObject> newMap);

	private:
		void move(PXG::Vector3 direction);
		std::shared_ptr<GameObject> map;
	};

}