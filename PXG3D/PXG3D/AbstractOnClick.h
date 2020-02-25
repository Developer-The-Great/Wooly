#pragma once
#include "Subscriber.h"
#include "Component.h"

namespace PXG
{
	class AbstractOnClick : public subscriber_base
	{
	public:


		virtual void onNotify(subject_base* subject_base, subject_base::event_t event) override;
	private:
		virtual void Execute() = 0;
	};



	inline void AbstractOnClick::onNotify(subject_base * subject_base, subject_base::event_t event)
	{
		Execute();
	}

}
