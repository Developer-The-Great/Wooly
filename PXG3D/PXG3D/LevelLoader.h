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
#include "MeshComponent.h"
#include "CollisionCubeParams.h"
#include "NodeToPositionContainer.h"
#include "World.h"
#include "GameObject.h"
#include "NodeGraph.h"
#include "FollowPlayerComponent.h"
#include "TriggerComponent.h"
#include "RockPushComponent.h"
#include "JumperComponent.h"
#include <map>
#include <memory>
#include "GrandpaComponent.h"
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


		void LoadLevel(std::ifstream& file, Game* game, std::shared_ptr<NodeGraph> nodeGraph,std::vector<NodeToPositionContainer>& nodeToPositionContainer,
			std::shared_ptr<MapMovementComponent> mapMovement)
		{
			using json = nlohmann::json;

			json config;
			file >> config;



			auto material = std::make_shared<TextureMaterial>();

			//--------------------------------------------- Iterate through tiles ------------------------------------//

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

							NodeToPositionContainer container;
							container.node = newNode;
							container.x = offset.x;
							container.y = offset.y;
							container.z = offset.z;

							nodeToPositionContainer.push_back(container);

							for (auto[key, value] : value.items())
							{
								if (key == "ramp" && value.is_boolean())
								{
									bool isKey = value.get<bool>();
									if(isKey) 
									{  
									
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
			Debug::Log("-----------------------------------------------------------");
			Debug::Log("finished loading tiles");
			Debug::Log("-----------------------------------------------------------");
			Debug::Log("loading other objects");
			//storing sheeps for the grandpa to reference
			std::vector<GameObj> sheepVector;

			for (auto& otherObjects : config["OtherObjects"])
			{
				//check if we are dealing with an object
				if (!otherObjects.is_object())
				{
					Debug::Log(Verbosity::Warning, "encountered non object type in tile-map, skipping!");
					continue;
				}
				//check if the object has the required fields
				if (!otherObjects["position"].is_array() || !otherObjects["model"].is_string())
				{
					Debug::Log(Verbosity::Error, "encountered object with invalid data!, abort loading");
					continue;
				}
				Vector3 offset;
				//check if the position field has enough entries
				if (otherObjects["position"].size() < 3)
				{
					Debug::Log(Verbosity::Error, "encountered an object with invalid position, not enough elements");
					continue;
				}
				//load the position field and make sure they are numbers 
				for (int i = 0; i < 3; ++i)
				{
					if (!otherObjects["position"][i].is_number())
					{
						Debug::Log(Verbosity::Error, "encountered an object with invalid position, element was not a number!");
						continue;
					}
					const auto dp = otherObjects["position"][i].get<float>();
					offset[i] = dp;
				}
				Debug::Log("{}", offset.ToString());


				//create a game-object
				GameObj child = game->Instantiate();


				//set the position
				child->SetLocalPosition((offset * Tile::SIZE) - Tile::CENTER_OFFSET);

				//load the model
				child->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + otherObjects["model"].get<std::string>());
				child->GetMeshComponent()->SetMaterial(material);

				//create physics representation 
				//child->GetPhysicsComponent()->ConstructPhysicsRepresentationFromMeshComponent();

				child->GetTransform()->Scale(glm::vec3{ Tile::WORLD_SCALE });
				//check if there is a separate texture and load it
				if (otherObjects["texture"].is_string())
				{
					child->GetMeshComponent()->AddTextureToMeshAt(Texture{ config::PXG_INDEPENDENT_TEXTURES_PATH + otherObjects["texture"].get<std::string>(),TextureType::DIFFUSE }, 0);

				}
				std::shared_ptr<TileMetaData> metaData = std::make_shared<TileMetaData>();
				//check if there is meta-data to add
				if (otherObjects["meta-data"].is_object())
				{
					Debug::Log(Verbosity::Info, "encountered object with attached meta-data!");
					for (auto[key, value] : otherObjects["meta-data"].items())
					{
						//check if other objects have behaviour 
						//creates components to the according to the behaviours name 
						if (key == "behaviour")
						{
							for (auto node : nodeGraph->GetNodes())
							{
								Vector3 pos = Vector3(offset.x, offset.y - 1, offset.z);
								if(pos==node->getPos())
								{
									Debug::Log("found node below object with behaviour");
									node->SetNodeWeight(2000);
								}
							}
							nodeGraph->AddNewInteractiveObj(child);
							if (value.is_string())
							{
								auto triggerComp = std::make_shared<TriggerComponent>();
								child->AddComponent(triggerComp);
								Vector3 nodePos = Vector3(offset.x, offset.y - 1, offset.z);
								triggerComp->SetNodePos(nodePos);
								triggerComp->SetNodeGraph(nodeGraph);
								triggerComp->subscribe(*mapMovement);
								
								if (value == "followPlayer")
								{
									auto followPlayer = std::make_shared<FollowPlayerComponent>();
									followPlayer->setMove(false);
									child->AddComponent(followPlayer);
									triggerComp->SetComponent(followPlayer);
									auto jumpComp = std::make_shared<JumperComponent>();
									child->AddComponent(jumpComp);
									mapMovement->attach(jumpComp.get());
									jumpComp->IsStatic(true);
									sheepVector.push_back(child);
								}
								if (value == "grandpa")
								{
									auto grandpaComp = std::make_shared<GrandpaComponent>();
									grandpaComp->setMove(false);
									child->AddComponent(grandpaComp);
									triggerComp->SetComponent(grandpaComp);
									mapMovement->attach(grandpaComp.get());
									for (auto sheep : sheepVector )
									{
										grandpaComp->AddGameObject(sheep);
									}
									//grandpaComp->AddGameObject()
								}
						/*		if (value == "movable")
								{
									auto rockPush = std::make_shared < RockPushComponent>();
									child->AddComponent(rockPush);
									triggerComp->SetComponent(rockPush);
								}
								if (value == "attackSheep")
								{

								}
								if (value == "trigger")
								{

								}*/
							
							}
						}
						metaData->metaData[key] = value.get<std::string>();
					}
				}
				metaData->offset = offset;
				child->AddComponent(metaData);
				mapMovement->AddOtherObjectToMove(child);

				GetOwner()->GetWorld().lock()->AddToChildren(child);
				//add the child to the map
				//GetOwner()->AddToChildren(child);

			}
		}

	};
}
