#pragma once
#include <random>


#include "AudioEngine.hpp"
#include "AudioLibrary.h"
#include "MapMovementComponent.h"
#include "Subscriber.h"

namespace PXG
{
	class OnMoveAudioPlayer : public subscriber_base , public Component
	{
	public:


		OnMoveAudioPlayer()
		{
			rng_engine = std::mt19937(seeder());
		}

		
		void onNotify(subject_base* subject_base, subject_base::event_t event) override
		{
			if(event == MapMovementComponent::ON_MOVE_FINISHED)
			{
				auto idx = distribution(rng_engine);
				AudioEngine::GetInstance().Play(AudioLibrary::GetSample("SStep" + std::to_string(idx+1)));
				
			}
		}

		std::mt19937 rng_engine;
		std::uniform_int_distribution<size_t> distribution = std::uniform_int_distribution<size_t>(0, 2);
		std::random_device seeder;
	};
}
