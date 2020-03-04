#pragma once
#include "Component.h"
#include "CompoundDistanceOnClickTrigger.hpp"
#include "Game.h"
#include "NodeGraphDistTrigger.h"
#include "RayCastHitHandler.h"
#include "RayCastShooter.h"

namespace PXG
{
	class TriggerFactoryComponent : public Component
	{
		static void sanity()
		{
			if (!triggers_to_be_built) triggers_to_be_built = new std::unordered_map<std::string, std::shared_ptr<TriggerFactoryComponent>>;
			if (!triggers_to_be_connected) triggers_to_be_connected = new std::unordered_map<std::string, std::shared_ptr<subscriber_base>>;
		}

		
	public :
		
		static void Build(GameObj Player, std::shared_ptr<RayCastShooter> shooter,std::shared_ptr<RayCastHitHandler> hit_handler,
			std::shared_ptr<MapMovementComponent> movement)
		{
			sanity();
			
			for(auto [trigger_name,trigger] : *triggers_to_be_built)
			{
				auto onClickTrigger = std::make_shared<OnClickTrigger>();
				auto nodeGraphDistTrigger = std::make_shared<NodeGraphDistTrigger>(*hit_handler);

				auto compoundTrigger = std::make_shared<CompoundDistanceOnClickTrigger>(onClickTrigger, nodeGraphDistTrigger);

				
				auto nodeGraphMovementRayCastCon = std::make_shared<NodeGraphMMC_RCS_Con>();
				nodeGraphMovementRayCastCon->SetImpl(nodeGraphDistTrigger);

				
				nodeGraphMovementRayCastCon->subscribe(*shooter);
				nodeGraphMovementRayCastCon->subscribe(*movement);

				Player->AddComponent(onClickTrigger);
				Player->AddComponent(nodeGraphDistTrigger);
				Player->AddComponent(compoundTrigger);
				Player->AddComponent(nodeGraphMovementRayCastCon);

				onClickTrigger->SetSubject(trigger->GetOwner(), shooter);

				if (auto it = triggers_to_be_connected->find(trigger_name); it != triggers_to_be_connected->end())
				{
					it->second->subscribe(*compoundTrigger);
				}
				else
				{
					__debugbreak();
					Debug::Log(Verbosity::Warning, "unconnected trigger encountered");
				}
			}
		}

		static std::shared_ptr<TriggerFactoryComponent> CreateTrigger(const std::string& name)
		{
			sanity();
			
			auto trigger = std::make_shared<TriggerFactoryComponent>();
			triggers_to_be_built->emplace(name,trigger);
			return trigger;
		}

		static void ConnectTrigger(const std::string& name,std::shared_ptr<subscriber_base> subj)
		{
			sanity();
			
			triggers_to_be_connected->emplace(name, subj);
		}
		
	private:
		inline static std::unordered_map<std::string,std::shared_ptr<TriggerFactoryComponent>>* triggers_to_be_built;
		inline static std::unordered_map<std::string, std::shared_ptr<subscriber_base>>* triggers_to_be_connected;
	};	
}
