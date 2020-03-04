#pragma once
#include <set>

#include "PathFinder.hpp"
#include "Subscriber.h"
#include "Trigger.h"
#include "GameObject.h"
#include "MapMovementComponent.h"

namespace PXG
{
	class NodeGraphDistTrigger : public Trigger , RayCastHitHandler::NodeGraphAccessor
	{
	public:

		NodeGraphDistTrigger(RayCastHitHandler& hit_handler) : NodeGraphDistTrigger(GetTranslatedGraph(hit_handler))
		{	;
		}
		
		NodeGraphDistTrigger(std::vector<PathFindingNode>& graph,size_t max_dist = 2) : m_start(nullptr) , m_end(nullptr) , m_dist(max_dist) , m_graph(graph)
		{
		}

		void SetA(PXG::Node* start) { m_start = start; }
		void SetB(PXG::Node* end) { m_end = end; }


		bool IsReady() const
		{
			return m_start != nullptr && m_end != nullptr;
		}

		void addEvent(event_t evnt)
		{
			m_listeners.insert(evnt);
		}

		
		void Test()
		{
			Debug::Log("A:{}, B:{}", m_start->getPos().ToString(), m_end->getPos().ToString());

			auto value = FindPath(m_graph, m_start, m_end).first;
			Debug::Log("value:{}", value);
			
			if (value && value <= m_dist)
				m_condition_met = true;
			else
				m_condition_met = false;
		}
		

		std::vector<PathFindingNode> & GetNodeGraph() const
		{
			return m_graph;
		}

		
	protected:
		bool triggerCondition() override { return m_condition_met; }
	private:

		::PXG::Node* m_start, * m_end;

		std::size_t m_dist;
		bool m_condition_met = false;
		std::vector<PathFindingNode>& m_graph;

		std::set<event_t> m_listeners;

		
	};


	class NodeGraphMMC_RCS_Con : public Component , public subscriber_base
	{
	public:


		void SetImpl(std::shared_ptr<NodeGraphDistTrigger> impl)
		{
			m_impl = impl;
		}
		
		void Start() override
		{
			auto [ manifestResult,manifestMessage] = Manifest(*owner.lock());
			if (manifestResult == false) throw std::runtime_error(fmt::format("manifest condition not satisfied! {}",manifestMessage));
		}

		
		std::pair<bool,std::string> Manifest(GameObject& receiver)
		{
			if(m_impl)
			{
				return { true,"already Satisfied" };
			}
			if (receiver.HasComponent<NodeGraphDistTrigger>())
			{
				m_impl = receiver.GetComponent<NodeGraphDistTrigger>();

			}
			else return { false,"missing NodeGraphDistTrigger" };

			return { true,"No Error" };
		}
		

		void onNotify(subject_base* subject_base, subject_base::event_t event) override
		{
			auto [is_mmc, mmc] = check_mmc_detail(subject_base);

			if(is_mmc && event == MapMovementComponent::ON_MOVE_START)
			{
				::PXG::Node* start = nullptr;
				
				for(auto& node : m_impl->GetNodeGraph())
				{
					if (node.GetRealNode()->getPos() == mmc->getOffset())
					{
						start = node.GetRealNode();
						break;
					}
				}
				if (start == nullptr)
					Debug::Log(Verbosity::Error, "Player position was not part of node graph!");
				
				m_impl->SetA(start);				
			}

			auto [is_rcs, rcs] = check_rcs_detail(subject_base);

			if(is_rcs && event == RayCastShooter::ON_RAYCAST_HIT)
			{
				::PXG::Node* end = nullptr;

				 auto obj = rcs->GetLastHit().GameObjectHit;

				if(obj->HasComponent<Node>())
				{
					end = obj->GetComponent<Node>().get();
				}
				if (end == nullptr)
					Debug::Log(Verbosity::Error, "Player position was not part of node graph!");

				m_impl->SetB(end);
				if (m_impl->IsReady())
				{
					m_impl->Test();
				}
			}
		}
	private:
		std::shared_ptr<NodeGraphDistTrigger> m_impl;


		static std::pair<bool,MapMovementComponent*> check_mmc_detail(subject_base* sub)
		{
			//static store
			static MapMovementComponent* _tinstance = nullptr;
			
			//check if the passed var was nullptr
			if (sub == nullptr) return std::pair{ false,_tinstance };

			//skip optimization
			if (sub == _tinstance)
				return { true,_tinstance };

			//check if it is a mmc
			auto swp_intance = dynamic_cast<MapMovementComponent*>(sub);
			if (swp_intance == nullptr) return std::pair{ false,_tinstance };

			//set store
			_tinstance = swp_intance;

			//return store
			return std::pair{ true,_tinstance };
			
		}

		static std::pair<bool, RayCastShooter*> check_rcs_detail(subject_base* sub)
		{
			//static store
			static RayCastShooter* _tinstance = nullptr;

			//check if the passed var was nullptr
			if (sub == nullptr) return std::pair{ false,_tinstance };

			//skip optimization
			if (sub == _tinstance)
				return { true,_tinstance };

			//check if it is a mmc
			auto swp_intance = dynamic_cast<RayCastShooter*>(sub);
			if (swp_intance == nullptr) return std::pair{ false,_tinstance };

			//set store
			_tinstance = swp_intance;

			//return store
			return std::pair{ true,_tinstance };

		}

		
	};
	
}
