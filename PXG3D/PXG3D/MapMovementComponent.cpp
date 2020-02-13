#pragma once 
#include "MapMovementComponent.h"
#include "Input.h"
#include "GameObject.h"
#include "KeyCode.h"
#include "Debug.h"
namespace PXG
{

	MapMovementComponent::MapMovementComponent()
	{
	}

	MapMovementComponent::~MapMovementComponent()
	{
	}


	void MapMovementComponent::Start()
	{
	}

	void MapMovementComponent::FixedUpdate(float tick)
	{

		if (Input::GetKeyDown(KeyCode::K))
		{
			move(PXG::Vector3(1, 0, 0));
		}
		if (Input::GetKeyDown(KeyCode::J))
		{
			move(PXG::Vector3(-1, 0, 0));
		}
	}

	void MapMovementComponent::SetMap(std::shared_ptr<GameObject> newMap)
	{
		this->map = newMap;
	}

	void MapMovementComponent::move(PXG::Vector3 direction)
	{
		std::vector<std::shared_ptr<GameObject>> tiles = map->GetChildren();
		int i = 0;
		Transform* transform = map->GetTransform();
		direction = direction * 100;
		Debug::Log(transform->GetLocalPosition().ToString());
		transform->SetLocalPosition(transform->GetPosition() + direction);
	}


}