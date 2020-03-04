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
#include "JumperComponent.h"

#include "WolfBehaviourComponent.h"

#include "RotatorComponent.h"

#include <map>
#include <memory>
#include "brokenCube.h"
#include "BridgeComponent.hpp"
#include "GrandpaComponent.h"
#include "NodeClearComponent.h"
#include "TriggerFactoryComponent.h"

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

		void HandleBridge(GameObj child,const nlohmann::json& value,Vector3 offset,Game* game,
			std::shared_ptr<NodeGraph> nodeGraph, std::vector<NodeToPositionContainer>& nodeToPositionContainer)
		{
			child->name = "bridge";

			//iterate over the keys in the bridge object
			for (auto [bridge_key, bridge_values] : value.items())
			{
				std::shared_ptr<BridgeComponent> comp;

				std::shared_ptr<NodeClearComponent> listener1, listener2;

				
				if(bridge_key == "trigger")
				{

					comp = std::make_shared<BridgeComponent>();
					
					child->AddComponent(comp);
					comp->AddListener(Trigger::ON_TRIGGER_RAISED);
					TriggerFactoryComponent::ConnectTrigger(bridge_values.get<std::string>(), comp);
				}
				
				//check if the orientation key is present
				if (bridge_key == "orientation")
				{

					//extract the rotation
					float rotation = bridge_values.get<float>();

					//rotate according to the rotation
					child->GetTransform()->rotate(Vector3(0, 1, 0), rotation);
					


					//normalize the rotation
					while (rotation < 0)
						rotation += 360;
					while (rotation >= 360)
						rotation -= 360;


					//delcare directions based on rotation
					const Vector3 fwd = Vector3{ 0,0,-1 };
					const Vector3 bwd = -fwd;
					const Vector3 lft = Vector3{ 1,0,0 };
					const Vector3 rgt = -lft;

					const Vector3 dwn = Vector3{ 0,-1,0 };


					//create the two new ghost nodes
					std::array<GameObj, 2> cot;

					auto& hidden1 = cot[0];
					auto& hidden2 = cot[1];

					//do initial setup on them
					for (auto& h : cot)
					{
						h = game->Instantiate();
						h->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "cube_empty.obj");
						h->GetMeshComponent()->SetMaterial(std::make_shared<TextureMaterial>());
						h->GetMeshComponent()->AddTextureToMeshAt({ config::PXG_INDEPENDENT_TEXTURES_PATH + "alpha256x256.png", TextureType::DIFFUSE }, 0);
						h->GetMeshComponent()->DisableRender(true);
						
						
						h->GetPhysicsComponent()->ConstructCollisionCube(CollisionCubeParams{ {0,0,0},0,0,1 });
						h->GetTransform()->Scale(glm::vec3{ Tile::WORLD_SCALE });

						GetOwner()->AddToChildren(h);

					}

					auto helper = [&](const Vector3 direction)
					{
						//create meta-data for the new ghost tile
						auto mdata1 = std::make_shared<TileMetaData>();

						//set the position
						mdata1->offset = offset + direction + dwn;
						hidden1->SetLocalPosition((offset * Tile::SIZE) + (direction + dwn) * Tile::SIZE- Tile::CENTER_OFFSET);

						//add the metadata to the node
						hidden1->AddComponent(mdata1);
						listener1 = std::make_shared<NodeClearComponent>();
						hidden1->AddComponent(listener1);


						//repeat for tile 2
						auto mdata2 = std::make_shared<TileMetaData>();
						mdata2->offset = offset + direction * 2 + dwn;
						hidden2->SetLocalPosition((offset * Tile::SIZE) + (direction * 2 + dwn) * Tile::SIZE - Tile::CENTER_OFFSET);
						hidden2->AddComponent(mdata2);

						listener2 = std::make_shared<NodeClearComponent>();
						hidden2->AddComponent(listener2);

					};

					auto helper2 = [&](const Vector3 direction)
					{
						//create meta-data for the new ghost tile
						auto mdata1 = std::make_shared<TileMetaData>();

						//set the position
						mdata1->offset = offset + (dwn + Vector3{ -direction.z,0,direction.x });
						hidden1->SetLocalPosition((offset * Tile::SIZE) + (dwn + Vector3{ -direction.z,0,direction.x }) *Tile::SIZE - Tile::CENTER_OFFSET);

						//add the metadata to the node
						hidden1->AddComponent(mdata1);
						listener1 = std::make_shared<NodeClearComponent>();
						hidden1->AddComponent(listener1);


						//repeat for tile 2
						auto mdata2 = std::make_shared<TileMetaData>();
						mdata2->offset = offset + (direction + dwn + Vector3{ -direction.z,0,direction.x });
						hidden2->SetLocalPosition((offset * Tile::SIZE) + (direction + dwn + Vector3{ -direction.z,0,direction.x })* Tile::SIZE - Tile::CENTER_OFFSET);
						hidden2->AddComponent(mdata2);

						listener2 = std::make_shared<NodeClearComponent>();
						hidden2->AddComponent(listener2);

					};
					

					//check all possible rotations steps
					if (rotation >= 0 && rotation < 90)
						helper(fwd);
					else if (rotation >= 90 && rotation < 180)
						helper(lft);
					else if (rotation >= 180 && rotation < 270)
						helper2(bwd);
					else if (rotation >= 270 && rotation < 360)
						helper2(rgt);


					for (auto& h : cot)
					{

						//make a new node for the hidden object
						auto node = std::make_shared<Node>();
						TileMetaData& mdat = *h->GetComponent<TileMetaData>();
						//initialize it with the offset
						node->initNode(mdat.offset);
						node->SetNodeWeight(BLOCKED_REALM(1));

						nodeGraph->AddNewNode(node.get());
						h->AddComponent(node);
						nodeToPositionContainer.emplace_back(mdat.offset, node);
					}

					if(listener1 && listener2 && comp)
					{
						listener1->AddListener(comp->ON_BRIDGE_DOWN);
						listener2->AddListener(comp->ON_BRIDGE_DOWN);

						listener1->subscribe(*comp);
						listener2->subscribe(*comp);
					}
				}
			}
		}
		

		void LoadLevel(std::ifstream& file, Game* game, std::shared_ptr<NodeGraph> nodeGraph, std::vector<NodeToPositionContainer>& nodeToPositionContainer,
			std::shared_ptr<MapMovementComponent> mapMovement, std::shared_ptr<RayCastHitHandler> rayCaster)
		{

			mapMovement->SetNodeGraph(nodeGraph);
			//SetNodeGraph(std::shared_ptr<NodeGraph> nodeGraph)



			using json = nlohmann::json;

			json config;
			file >> config;
			auto cube = std::shared_ptr<brokenCube>();


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

				//Debug::Log("{}", offset.ToString());


				//create a game-object
				GameObj child = game->Instantiate();


				//set the position
				child->SetLocalPosition((offset * Tile::SIZE) - Tile::CENTER_OFFSET);

				//load the model
				child->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + tile["model"].get<std::string>());
				child->GetMeshComponent()->SetMaterial(std::make_shared<TextureMaterial>());

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
					//Debug::Log(Verbosity::Info, "encountered object with attached meta-data!");

					for (auto[key, value] : tile["meta-data"].items())
					{

						//check if the meta-data is a node
						if (key == "node" && value.is_object())
						{
						//	Debug::Log("found Node");
							//create Node 
							auto newNode = std::make_shared<Node>();
							newNode->initNode(offset);
							nodeGraph->AddNewNode(newNode.get());

							NodeToPositionContainer container{ offset,newNode };

							nodeToPositionContainer.push_back(container);
							child->AddComponent(newNode);
							
							for (auto[key, value] : value.items())
							{
								if (key == "ramp" && value.is_object())
								{

								}

								if (key == "ladder" && value.is_object())
								{
									child->name = "ladder";
									newNode->SetNodeTypeTo(NodeType::Ladder);

									for (auto[key, value] : value.items())
									{
										if (key == "direction")
										{
											Vector3 dir = extractV3(value);
											dir.Normalize();
											newNode->SetLadderConnectionDirection(dir);

										}

										if (key == "rotateWorldY")
										{
											const float yRotateAmount = value.get<float>();

											child->GetTransform()->rotate(Vector3(0, 1, 0), yRotateAmount);
										}
									}
								}

							
							}
						
							continue;
						}
						//check if we are dealing with a bridge
						if (key == "bridge" && value.is_object())
						{
							HandleBridge(child, value, offset, game, nodeGraph, nodeToPositionContainer);
							continue;
						}
						if (!value.is_string())
						{
							Debug::Log(Verbosity::Error, "invalid meta-data in object , value was not string");
							continue;
						}

						if(key == "is_trigger")
						{
							child->AddComponent(TriggerFactoryComponent::CreateTrigger(value.get<std::string>()));
						}
						if(key =="is_broken")
						{
							cube= std::make_shared<brokenCube>();

							cube->map = mapMovement;
						//	cube->ray = rayCaster;
							cube->graph = nodeGraph;
							child->AddComponent(cube);
							mapMovement->attach(cube.get());


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
	/*		Debug::Log("-----------------------------------------------------------");
			Debug::Log("finished loading tiles");
			Debug::Log("-----------------------------------------------------------");
			Debug::Log("loading other objects");*/
			//storing sheeps for the grandpa to reference
			std::vector<GameObj> sheepVector;




			//-------------------------------------------------- Adding other Objects------------------------------------------//

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
				Vector3 offset = extractV3(otherObjects["position"]);
				
		//		Debug::Log("{}", offset.ToString());


				//create a game-object
				GameObj child = game->Instantiate();


				//set the position
				child->SetLocalPosition((offset * Tile::SIZE) - Tile::CENTER_OFFSET);

				//load the model
				child->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + otherObjects["model"].get<std::string>());
				child->GetMeshComponent()->SetMaterial(std::make_shared<TextureMaterial>());

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
					//Debug::Log(Verbosity::Info, "encountered object with attached meta-data!");
					for (auto[key, value] : otherObjects["meta-data"].items())
					{
						//check if other objects have behaviour 
						//creates components to the according to the behaviours name 
						if (key == "behaviour")
						{
							for (auto node : nodeGraph->GetNodes())
							{
								Vector3 pos = Vector3(offset.x, offset.y - 1, offset.z);
								if (pos == node->getPos())
								{
									//Debug::Log("found node below object with behaviour");
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
									auto rotator = std::make_shared<RotatorComponent>(Vector3(0, 1, 0), 0.5f);
									//child->AddComponent(rotator);
									child->GetTransform()->translate(Vector3(50, 0, 50));
									triggerComp->onNotify(mapMovement.get(), MapMovementComponent::ON_MOVE_FINISHED);
									cube->attach(triggerComp.get());

								}
								if (value == "grandpa")
								{
									auto grandpaComp = std::make_shared<GrandpaComponent>();
									grandpaComp->setMove(false);
									child->AddComponent(grandpaComp);
									triggerComp->SetComponent(grandpaComp);
									mapMovement->attach(grandpaComp.get());
									for (auto sheep : sheepVector)
									{
										grandpaComp->AddGameObject(sheep);

									}
									//grandpaComp->AddGameObject()
								}

								if (value == "wolf")
								{
									auto wolfBehaviourComponent = std::make_shared<WolfBehaviourComponent>(offset,mapMovement);
									child->AddComponent(wolfBehaviourComponent);
									triggerComp->SetComponent(wolfBehaviourComponent);
									mapMovement->attach(wolfBehaviourComponent.get());
									
									child->GetTransform()->translate(Vector3(50, 0, 50));
									//child->AddComponent(std::make_shared<RotatorComponent>(Vector3(0, 1, 0), 3.0f));

								}



							}
						}

						if (key == "wolfData" && value.is_object())
						{



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

	private:



	};
}
