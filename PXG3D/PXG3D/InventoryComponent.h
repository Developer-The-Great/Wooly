#pragma once
#include "Component.h"
#include <unordered_map>
#include "Debug.h"

namespace PXG
{
	using item_identifier_t = uint32_t;

	
	class Inventory : public Component
	{
	public:
		void Start() override{}
		void FixedUpdate(float tick) override{}

		void AddItem(item_identifier_t item);
		bool RemoveItem(item_identifier_t item);

		std::vector<item_identifier_t> EnumerateUniqueItems();
		uint8_t GetAmountOfItem(item_identifier_t item);

		
	private:

		std::unordered_map<item_identifier_t, uint8_t> item_map;
	};

	inline void Inventory::AddItem(item_identifier_t item)
	{
		Debug::Log("item#{0} was added to the inventory",item);

		item_map[item]++;
	}

	inline bool Inventory::RemoveItem(item_identifier_t item)
	{
		if(const auto itr = item_map.find(item); itr != item_map.end())
		{
			if (itr->second <= 0) return false;
			Debug::Log("item#{0} was removed from the inventory", item);
			itr->second--;
			return true;
		}
		return false;
	}


	inline std::vector<item_identifier_t> Inventory::EnumerateUniqueItems()
	{
		std::vector<item_identifier_t> collection;
		for(auto& [key,value] : item_map)
		{
			collection.push_back(key);
		}
		return collection;
		
	}

	inline uint8_t Inventory::GetAmountOfItem(item_identifier_t item)
	{
		if (const auto itr = item_map.find(item); itr != item_map.end())
		{
			return itr->second;
		}
		return 0;
	}
}
