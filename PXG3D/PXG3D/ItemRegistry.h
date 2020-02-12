#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include "Item.h"


namespace PXG
{
	class ItemRegistry
	{
	public:

		//elem 1 pretty_name
		//elem 2 file_name
		using item_info_t = std::tuple<std::string, std::string>;

		

		static void LoadConfig(std::ifstream* file)
		{
			using json = nlohmann::json;
			json config;

			*file >> config;

			item_identifier_t i = 1;
			
			for(auto item_desc : config["items"])
			{
				if(item_desc["pretty_name"].is_string() && item_desc["file_name"].is_string())
				{
					const auto pretty_name	=	item_desc["pretty_name"].get<std::string>();
					const auto file_name		=	item_desc["file_name"].get<std::string>();
					nameTable[i] = { pretty_name,file_name };
					reverseTable[pretty_name] = i;
					Debug::Log(Verbosity::Debug, "loaded item#{0} with pname: {1} & fname: {2}", i, pretty_name, file_name);
					i++;

				}
			}
		}
		
		static item_identifier_t LookUpItem(const std::string& name)
		{
			if(const auto itr = reverseTable.find(name); itr != reverseTable.end())
			{
				return itr->second;
			}
			return null_item;
		}

		static item_info_t GetItemInfo(item_identifier_t ident)
		{
			if(ident == 0)
			{
				return item_info_t{};
			}
			if(const auto itr = nameTable.find(ident); itr != nameTable.end())
			{
				return itr->second;
			}
			return item_info_t{};
			
		}


	private:
		inline static std::map<item_identifier_t, item_info_t> nameTable;
		inline static std::unordered_map<std::string, item_identifier_t> reverseTable;
		
	};	
}
