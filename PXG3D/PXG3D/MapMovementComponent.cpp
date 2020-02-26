#include "MapMovementComponent.h"
#include "Input.h"
#include "GameObject.h"
#include "KeyCode.h"
#include "Debug.h"
#include "Tile.h"

namespace PXG
{


	void MapMovementComponent::Start()
	{
	}

	void MapMovementComponent::FixedUpdate(float tick)
	{
		static float timer = 0;
		timer += tick;

		if(timer > 0.5f)
		{
			if(!commandQueue.empty())
			{
				auto back = commandQueue.back();
				commandQueue.pop_back();

				switch(back)
				{
				case FORWARD:	move({  0, 0, 1 }); break;
				case BACKWARD:  move({  0 ,0,-1 }); break;
				case UP:		move({  0, 1, 0 }); break;
				case DOWN:		move({  0,-1, 0 }); break;
				case LEFT:		move({  1, 0, 0 }); break;
				case RIGHT:		move({ -1, 0, 0 }); break;
				default: ;
				}


			}
			timer = 0;
		}



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

	void MapMovementComponent::Reset() const
	{
		Transform* mapTransform = map->GetTransform();
		mapTransform->SetLocalPosition({ 0,0,0 });
	}

	void MapMovementComponent::move(PXG::Vector3 direction)
	{

		std::vector<std::shared_ptr<GameObject>> tiles = map->GetChildren();
		int i = 0;
		Transform* mapTransform = map->GetTransform();
		direction = direction * 100;
		mapTransform->SetLocalPosition(mapTransform->GetPosition() + direction);
		notify(ON_MOVE);


	}


}
