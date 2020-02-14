#pragma once
#include "Component.h"
#include "Subscriber.h"
#include "font.h"
#include "MapMovementComponent.h"

namespace PXG
{

	class EnergyCounterComponent : public Component, public subscriber_base, public subject_base
	{
		enum event : event_t
		{
			ON_OUT_OF_ENERGY = 0xDEAD
		};

		
	public:
		EnergyCounterComponent(FontRenderer* renderer,Font* font) :ffont(font), frender(renderer) {};

		void Start() override{}
		void FixedUpdate(float tick) override
		{
			frender->text(ffont, "Energy {}", 1, { 10 - 400,- 10 - 16 + 300 }, energy);
		}

		void onNotify(subject_base* subject_base, subject_base::event_t event) override
		{

			static MapMovementComponent* mmc;
			
			if (mmc != nullptr && mmc == subject_base || mmc == nullptr && (mmc = dynamic_cast<MapMovementComponent*>(subject_base)) != nullptr)
			{
				switch(event)
				{
				case MapMovementComponent::ON_MOVE:
				{
					energy--;
					if (energy <= 0)
					{
						energy = 10;

						notify(ON_OUT_OF_ENERGY);

					}
				}
				default:break;
				}
			}

		}
		

	private:

		int energy = 10;

		Font* ffont;
		FontRenderer * frender;
		std::string text;
		
	};
}
