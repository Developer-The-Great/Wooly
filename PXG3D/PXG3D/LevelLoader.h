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
#include "PhysicsComponent.h"

#include "CollisionCubeParams.h"



#include "NodeGraph.h"

namespace PXG
{

	template <class JSON>
	auto extractV3(JSON j)
	{
		if (j.size() < 3)
		{
			Debug::Log(Verbosity::Error, "encountered an object with invalid position, not enough elements");
			throw std::runtime_error("Invalid conversion from array to V3");
		}
		
		Vector3 result;
		for (int i = 0; i < 3; ++i)
		{
			if (!j[i].is_number())
			{
				Debug::Log(Verbosity::Error, "encountered an object with invalid position, element was not a number!");
				continue;
			}
			const auto dp = j[i].template get<float>();
			result[i] = dp;
		}
		return result;
	}


	
	class LevelLoader : public Component
	{
	public:
		void Start() override {}
		void FixedUpdate(float tick) override {}


		void LoadLevel(std::ifstream& file, Game* game, std::shared_ptr<NodeGraph> nodeGraph)
		{
			using json = nlohmann::json;

			json config;
			file >> config;


			auto material = std::make_shared<TextureMaterial>();

			for (auto& tile : config["tiles"])
			{

				//check if we are dealing with an object
				if (!tile.is_object())
				{
					Debug::Log(Verbosity::Warning, "encountered non object type in tile-map, skipping!");
					continue;
				}


				//check if the object has the required fields
				if (!tile["position"].is_array() || !tile["model"].is_string())
				{
					Debug::Log(Verbosity::Error, "encountered object with invalid data!, abort loading");
					continue;
				}

				Vector3 offset = extractV3(tile["position"]);

				Debug::Log("{}", offset.ToString());


				//create a game-object
				GameObj child = game->Instantiate();


				//set the position
				child->SetLocalPosition((offset * Tile::SIZE) - Tile::CENTER_OFFSET);

				//load the model
				child->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + tile["model"].get<std::string>());
				child->GetMeshComponent()->SetMaterial(material);

				//create physics representation 

				CollisionCubeParams cubeParams;
				cubeParams.heightFromMin = 1;

				child->GetPhysicsComponent()->ConstructCollisionCube(cubeParams);
				//child->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();

				child->GetTransform()->Scale(glm::vec3{ Tile::WORLD_SCALE });
				//check if there is a separate texture and load it
				if (tile["texture"].is_string())
				{
					child->GetMeshComponent()->AddTextureToMeshAt(Texture{ config::PXG_INDEPENDENT_TEXTURES_PATH + tile["texture"].get<std::string>(),TextureType::DIFFUSE }, 0);

				}

				std::shared_ptr<TileMetaData> metaData = std::make_shared<TileMetaData>();


				//check if there is meta-data to add
				if (tile["meta-data"].is_object())
				{
					Debug::Log(Verbosity::Info, "encountered object with attached meta-data!");

					for (auto[key, value] : tile["meta-data"].items())
					{

						//check if the meta-data is a node
						if (key == "node" && value.is_object())
						{
							Debug::Log("found Node");
							//create Node 
							auto newNode = std::make_shared<Node>();
							int weight = 1;
							newNode->initNode(offset);
							nodeGraph->AddNewNode(newNode.get());

						
							Debug::Log("Object in Node Meta Data");
							for (auto[key, value] : value.items())
							{
								if (key == "connected_nodes" && value.is_array())
								{
									for (auto& connection : value)
									{

										Vector3 connectionPos = extractV3(connection);
										newNode->AddNewConnection(connectionPos);
									}
								}
							}
							child->AddComponent(newNode);
							continue;
						}
						if (!value.is_string())
						{
							Debug::Log(Verbosity::Error, "invalid meta-data in object , value was not string");
							continue;
						}
						metaData->metaData[key] = value.get<std::string>();
					}
				}
				metaData->offset = offset;
				child->AddComponent(metaData);

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
