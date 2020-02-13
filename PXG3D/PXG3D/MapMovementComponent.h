#pragma once
#include "Component.h"
#include "GLMHeaders.h"
#include "Vector3.h"
#include "Subscriber.h"
#include "RayCastShooter.h"
#include "TileMetaData.h"
#include <queue>

namespace PXG
{

	class MapMovementComponent : public Component, public subscriber_base, public subject_base
	{
	public:

		enum event : event_t
		{
			ON_MOVE
		};
		

		void onNotify(subject_base* subject_base, subject_base::event_t event) override
		{
			static RayCastShooter* shooter = nullptr;


			if (event == 0xDEAD)
			{
				Reset();
				commandQueue.clear();
				offset = Vector3{ 0,0,0 };
			}

			//check if we are looking at a ray-cast-shooter
			if(shooter != nullptr && shooter == subject_base || shooter == nullptr && (shooter = dynamic_cast<RayCastShooter*>(subject_base)) != nullptr)
			{
				switch(event)
				{
				case RayCastShooter::ON_RAYCAST_HIT:
				{
					const HitInfo& info = shooter->GetLastHit();
					auto metadata = info.GameObjectHit->GetComponent<TileMetaData>();

					if (!metadata) return;
						
					Debug::Log("retrieved Meta-Inf via raycast!");

						
					auto delta = offset-metadata->GetOffset() ;
					//delta.x = -delta.x;

					auto pusher = [&](int dp, MovementCommands pos, MovementCommands neg)
					{
						if (dp > 0)
						{
							for (; dp!= 0; --dp)
							{
								commandQueue.push_front(pos);
							}
						}
						if (dp < 0)
						{
							for (; dp != 0; ++dp)
							{
								commandQueue.push_front(neg);
							}
						}
					};

						
					pusher(delta.x, LEFT, RIGHT);
					pusher(delta.y, UP, DOWN);
					pusher(delta.z, FORWARD, BACKWARD);

					Debug::Log("Movement delta: {}", delta.ToString());
					for(auto& elem : commandQueue)
					{
						switch (elem) {
						case FORWARD:	Debug::Log("Forward"); break;
						case BACKWARD:	Debug::Log("Backward"); break;
						case UP:		Debug::Log("Up"); break;
						case DOWN:		Debug::Log("Down"); break;
						case LEFT:		Debug::Log("Left"); break;
						case RIGHT:		Debug::Log("Right"); break;
						default: ; }
					}
						
					//move(delta);
					offset = metadata->GetOffset();
						
					break;
				}
				default:break;
				}
			}
		}


		void Start() override;
		void FixedUpdate(float tick) override;
		void SetMap(std::shared_ptr<GameObject> newMap);
		void Reset();

	private:
		void move(PXG::Vector3 direction);
		std::shared_ptr<GameObject> map;

		Vector3 offset = {0,0,0};


		enum MovementCommands
		{
			FORWARD,
			BACKWARD,
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		
		std::deque<MovementCommands> commandQueue;

		
	};

}
