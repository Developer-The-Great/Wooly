#pragma once
#include "Component.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Debug.h"
#include "Game.h"
#include "TileMetaData.h"
#include "Tile.h"
#include "FileConfig.h"
#include "TextureMaterial.h"

namespace PXG
{
	class LevelLoader : public Component
	{

		
	public:
		void Start() override{}
		void FixedUpdate(float tick) override{}

		void LoadLevel(std::ifstream& file,Game* game)
		{
			using json = nlohmann::json;

			json config;
			file >> config;


			auto material = std::make_shared<TextureMaterial>();

			for(auto& tile : config["tiles"])
			{

				//check if we are dealing with an object
				if(!tile.is_object())
				{
					Debug::Log(Verbosity::Warning, "encountered non object type in tile-map, skipping!");
					continue;
				}


				//check if the object has the required fields
				if( !tile["position"].is_array() ||	!tile["model"].is_string())
				{
					Debug::Log(Verbosity::Error, "encountered object with invalid data!, abort loading");
					continue;	
				}

				Vector3 offset;

				//check if the position field has enough entries
				if(tile["position"].size() < 3)
				{
					Debug::Log(Verbosity::Error, "encountered an object with invalid position, not enough elements");
					continue;
				}

				//load the position field and make sure they are numbers 
				for(int i = 0; i <3;++i)
				{
					if(!tile["position"][i].is_number())
					{
						Debug::Log(Verbosity::Error, "encountered an object with invalid position, element was not a number!");
						continue;
					}
					const auto dp = tile["position"][i].get<float>();
					offset[i] = dp;
				}
				
				Debug::Log("{}", offset.ToString());
				

				//create a game-object
				GameObj child = game->Instantiate();


				//set the position
				child->SetLocalPosition((offset * Tile::SIZE) - Tile::CENTER_OFFSET);

				//load the model
				child->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + tile["model"].get<std::string>());
				child->GetMeshComponent()->SetMaterial(material);
				child->GetTransform()->Scale(glm::vec3{ Tile::WORLD_SCALE });
				//check if there is a separate texture and load it
				if (tile["texture"].is_string())
				{
					child->GetMeshComponent()->AddTextureToMeshAt(Texture{ config::PXG_INDEPENDENT_TEXTURES_PATH + tile["texture"].get<std::string>(),TextureType::DIFFUSE }, 0);

				}

		

				//check if there is meta-data to add
				if(tile["meta-data"].is_object())
				{
					Debug::Log(Verbosity::Info, "encountered object with attached meta-data!");
					std::shared_ptr<TileMetaData> metaData = std::make_shared<TileMetaData>();
					for(auto[key,value] : tile["meta-data"].items())
					{
						if(!value.is_string())
						{
							Debug::Log(Verbosity::Error, "invalid meta-data in object , value was not string");
							continue;
						}

						metaData->metaData[key] = value.get<std::string>();

						child->AddComponent(metaData);
						
					}
					
				}
				if (tile["rotation"].is_number())
				{
					float angle = tile["rotation"].get<float>();
				}

				//add the child to the map
				GetOwner()->AddToChildren(child);
			}
		}
		
	};
}
