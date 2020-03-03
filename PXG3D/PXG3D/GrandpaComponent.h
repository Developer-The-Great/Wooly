#pragma once
#include "AbstractEventComponent.h"
#include "Component.h"
#include "Subscriber.h"
#include "MapMovementComponent.h"
#include <vector>
#include "GameObject.h"
#include "TriggerComponent.h"
#include "Vector3.h"
namespace PXG
{
	class GrandpaComponent: public AbstractEventComponent, public subscriber_base
	{
	public:
		void AddGameObject(std::shared_ptr<GameObject> object)
		{
			sheeps.push_back(object);
		}

	private:


		virtual void Execute(Node * currentNode, Node * targetNode) override;


		virtual void onNotify(subject_base * subject_base, subject_base::event_t event) override;
		std::vector<std::shared_ptr<GameObject>> sheeps;
	};

		
}
