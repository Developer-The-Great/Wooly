#pragma once
#include <set>

#include "Component.h"
#include "Subscriber.h"
#include "Transform.h"
#include "glm/gtc/quaternion.hpp"


namespace PXG
{
	class BridgeComponent : public Component , public subscriber_base, public subject_base
	{
	
		void onNotify(subject_base* subject_base, event_t event) override
		{
			if(listener_events.find(event) != listener_events.end())
			{
				m_goDown = true;
			}
		}
	public:

		void AddListener(event_t evnt)
		{
			listener_events.emplace(evnt);
		}


		enum event : event_t
		{
			ON_BRIDGE_DOWN
		};

		void FixedUpdate(float tick) override
		{
			if (!m_goDown) return;
			if (m_isDown) return;

			
			auto transform = GetOwnerTransform();

			time -= tick * 2;
			time = Mathf::Clamp(time, -glm::pi<float>() /2,0);

			auto tau = 1;
			
			if (transform)
			{
				auto q = glm::angleAxis(Mathf::Lerp(0, tau, time), glm::vec3{ 1,0,0 });
				transform->SetRotation(Quaternion{ q }.Normalized());
				notify(ON_BRIDGE_DOWN);
			}
			if(time == 1)
				m_isDown = true;	
		}
	private:
		float time = 0;
		bool m_goDown = false;
		bool m_isDown = false;

		
		std::set<event_t> listener_events;	
		
	};
}
