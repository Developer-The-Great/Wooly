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

	inline std::pair < bool,std::shared_ptr<std::vector<PathFindingNode*>>> FindPath(std::vector<PathFindingNode>& translated_graph,::PXG::Node* start,::PXG::Node* end)
	{

		AStar::getInstance().clear();
		//auto translated_graph = TranslateNodeGraph(graph);

		auto translated_start = FindNode(translated_graph, start);
		auto translated_end = FindNode(translated_graph, end);

		

		finder.setStart(*translated_start);
		finder.setGoal(*translated_end);

		std::shared_ptr<std::vector<PathFindingNode*>> path = std::make_shared<std::vector<PathFindingNode*>>();
		
		bool result = finder.findPath<AStar>(*path);

		return std::pair(result, path);		
	}

	
}

/*
namespace PXG
{
	class PathFindingNode : std::enable_shared_from_this<PathFindingNode>
	{
	public:
		PathFindingNode(Node* realNode) : node(realNode)
		{
			if(node != nullptr)
			{
				distance = node->GetNodeWheight();
				node->PathFindingData = shared_from_this();
				for(auto* child : node->GetConnectedNodes())
				{
					auto sptr = std::make_shared<PathFindingNode>(child);
					
				}
			}
		}

		[[nodiscard]] Node* GetRealNode() const { return node; }

		
		void SetParent(PathFindingNode* p)
		{
			parent = p;
		}

		[[nodiscard]] PathFindingNode* GetParent() const
		{
			return parent;
		}

		void AddChild(std::shared_ptr<PathFindingNode> child,int distance)
		{
			children.emplace_back(child, distance);
		}

		void SetClosed(bool c)
		{
			closed = c;
		}

		void SetDistance(int d)
		{
			distance = d;
		}

		[[nodiscard]] bool GetClosed() const
		{
			return closed;
		}

		[[nodiscard]] int GetDistance() const
		{
			return distance;
		}

		void Release()
		{
			distance = INT_MAX;
			closed = false;
			parent = nullptr;
		}

		friend bool operator<(const PathFindingNode& lhs,const PathFindingNode& rhs)
		{
			return lhs.GetDistance() < rhs.GetDistance();
		}

	private:

		bool closed = false;
		int distance = INT_MAX;
		
		Node* node;
		PathFindingNode* parent = nullptr;
		std::vector<std::pair<std::shared_ptr<PathFindingNode>, int>> children;
		
	};

	
	class PathFinder
	{

	private:

		std::vector<PathFindingNode*> open, closed;
		
		void pushOpen(PathFindingNode* node)
		{
			open.push_back(node);
			std::push_heap(open.begin(), open.end(), [](auto* l, auto* r) { return *l < *r;	});
		}
		void popOpen(PathFindingNode* node)
		{
			std::pop_heap(open.begin(), open.end(),[](auto* l, auto* r) { return *l < *r;	});
			open.pop_back();
			node->SetClosed(true);
			closed.push_back(node);
		}

		void releaseNodes()
		{
			for (const auto& node : open)
				node->Release();
			for (const auto& node : closed)
				node->Release();
		}
		
		static int minDistance(std::vector<int>& distances,std::vector<bool> sptSet)
		{
			int min = INT_MAX, min_index = 0;

			for(size_t v = 0; v < distances.size();++v) if (sptSet[v] == false && distances[v] <= min)
			{
				min = distances[v], min_index = v;
			}
			return min_index;
		}

		
	public:
		
		std::vector<PathFindingNode*> GenPath(const std::vector<PathFindingNode*>& nodes,Node* A, Node* B)
		{
			PathFindingNode* currentNode, * childNode;
			float dist;

			std::make_heap(open.begin(), open.end(), [](auto* l, auto* r) { return *l < *r;	});
			pushOpen()

			//generate array for all distances
			std::vector<int32_t> distances;
			distances.resize(nodes.size(),INT_MAX);

			//generate array for completed sets
			std::vector<bool> sptSet;
			sptSet.resize(nodes.size(),false);

			//find the iterator to A
			const auto src_itr = std::find(nodes.begin(), nodes.end(), A);

			//set the distance to the source to 0
			distances[std::distance(nodes.begin(), src_itr)] = 0;
			
			for(auto* node : nodes)
			{
				int u = minDistance(distances, sptSet);

				sptSet[static_cast<size_t>(u)] = true;
				
			}
		}
	};
}
*/