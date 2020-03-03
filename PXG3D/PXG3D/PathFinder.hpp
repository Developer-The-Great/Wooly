#pragma once
#include <vector>
#include "memory"
#include "PXGNode.h"

#include "pfinder/AStar.h"
#include "pfinder/PathFinder.h"


#define MAX_GRAPH_PATH 1024

#define PATH_REALM(X) (X)
#define BLOCKED_REALM(X) (MAX_GRAPH_PATH + (X))
#define IMPOSSIBLE_REALM(X) (INT_MAX - MAX_GRAPH_PATH + (X))


namespace PXG {
	inline bool is_blocked(::PXG::Node* n)
	{
		return 	n->GetNodeWheight() >= MAX_GRAPH_PATH && n->GetNodeWheight() <= INT_MAX - MAX_GRAPH_PATH;
	}

	inline bool is_impossible(::PXG::Node* n)
	{
		return n->GetNodeWheight() >= INT_MAX - MAX_GRAPH_PATH;
	}

	
	
	class PathFindingNode : public AStarNode
	{	
	public:


		
		PathFindingNode(PXG::Node* realNode) : node(realNode)
		{
		}

		[[nodiscard]] ::PXG::Node* GetRealNode() const
		{
			return node;
		}

		void AddChild(PathFindingNode& n)
		{
			addChild(&n,1);
		}
		
		float distanceTo(AStarNode* node) const override
		{
			auto prhsnode = dynamic_cast<PathFindingNode*>(node);
			if (prhsnode == nullptr) throw std::runtime_error("invalid nodegraph");

			const auto dist = prhsnode->GetRealNode()->getPos() - GetRealNode()->getPos();

			const float abs_dist = std::abs(dist.Length());

			if (abs_dist > MAX_GRAPH_PATH) throw std::runtime_error("abs_dist exceeded max_path");
			
			if (is_impossible(this->node) || is_impossible(prhsnode->node))
			{
				return IMPOSSIBLE_REALM(abs_dist);
			}
			else if (is_blocked(this->node) || is_blocked(prhsnode->node))
			{
				return BLOCKED_REALM(abs_dist);
			}
			else return PATH_REALM(abs_dist);
		}

	private:

		PXG::Node* node;

		
	};
	static inline PathFinder<PathFindingNode> finder;
	static inline uint32_t LastPathWeight = 0;

	
	inline PathFindingNode* FindNode(std::vector<PathFindingNode>& graph, ::PXG::Node* node)
	{
		for(auto& n : graph)
		{
			if (n.GetRealNode() == node) return &n;
		}
		return nullptr;
	}

	inline std::vector<PathFindingNode> TranslateNodeGraph(const std::vector<::PXG::Node*>& graph)
	{
		std::vector<PathFindingNode> translated_graph;
		translated_graph.reserve(graph.size());

		
		for (auto* node : graph)
		{
			translated_graph.emplace_back(node);
		}


		for(auto& node : translated_graph)
		{
			auto* rnode = node.GetRealNode();

			for(auto* connection : rnode->GetConnectedNodes())
			{
				node.AddChild(*FindNode(translated_graph, connection));
			}
		}
		
		return translated_graph;
	}

	inline std::pair < uint32_t,std::shared_ptr<std::vector<PathFindingNode*>>> FindPath(std::vector<PathFindingNode>& translated_graph,::PXG::Node* start,::PXG::Node* end)
	{

		AStar::getInstance().clear();
		//auto translated_graph = TranslateNodeGraph(graph);

		auto translated_start = FindNode(translated_graph, start);
		auto translated_end = FindNode(translated_graph, end);

		

		finder.setStart(*translated_start);
		finder.setGoal(*translated_end);

		std::shared_ptr<std::vector<PathFindingNode*>> path = std::make_shared<std::vector<PathFindingNode*>>();
		
		bool result = finder.findPath<AStar>(*path);

		uint32_t overall_weight{0};
		bool impossible{false}, blocked{false};
		
		for(auto* node: *path)
		{
			if(!blocked)
				if (is_blocked(node->GetRealNode())) blocked = true;
			if(!impossible)
				if (is_impossible(node->GetRealNode())) impossible = true;

			if (impossible)
				break;
		}

		if (blocked && !impossible)
			overall_weight = BLOCKED_REALM(path->size());
		else if (impossible)
			overall_weight = IMPOSSIBLE_REALM(path->size());
		else
			overall_weight = PATH_REALM(path->size());

		LastPathWeight = overall_weight;
		
		return std::pair(overall_weight, path);		
	}

	
}