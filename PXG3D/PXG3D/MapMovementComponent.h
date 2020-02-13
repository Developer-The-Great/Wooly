#pragma once
#include "Component.h"
#include "GLMHeaders.h"
#include "Vector3.h"
#include "Subscriber.h"
#include "RayCastShooter.h"
#include "TileMetaData.h"

namespace PXG
{

	class MapMovementComponent : public Component, public subscriber_base
	{
	public:

		void onNotify(subject_base* subject_base, subject_base::event_t event) override
		{
			static RayCastShooter* shooter = nullptr;


			//check if we are looking at a ray-cast-shooter
			if(shooter != nullptr && shooter == subject_base || (shooter = dynamic_cast<RayCastShooter*>(subject_base)) != nullptr)
			{
				switch(event)
				{
				case RayCastShooter::ON_RAYCAST_HIT:
				{
					const HitInfo& info = shooter->GetLastHit();
					auto metadata = info.GameObjectHit->GetComponent<TileMetaData>();

					if (!metadata) return;
						
					Debug::Log("retrieved Meta-Inf via raycast!");

						
					auto delta =  metadata->GetOffset() - offset;
					delta.y = -delta.y;
					move(delta);

						
					break;
				}
				default:break;
				}
			}
		}

		
		MapMovementComponent();
		~MapMovementComponent();
		virtual void Start() override;
		void FixedUpdate(float tick) override;
		void SetMap(std::shared_ptr<GameObject> newMap);

	private:
		void move(PXG::Vector3 direction);
		std::shared_ptr<GameObject> map;

		Vector3 offset = {0,0,0};

		
	};

}
