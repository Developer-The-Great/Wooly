#include "PXGGame.h"
#include "PhysicsEngine.h"
#include "HitInfo.h"
#include "FreeMovementComponent.h"
#include "World.h"

#include "FileConfig.h"

#include "ColorMaterial.h"
#include "TextureMaterial.h"
#include "StandardLitMaterial.h"

#include "LightComponent.h"

#include "MapMovementComponent.h"
#include "CameraComponent.h"
#include "RotatorComponent.h"
#include "Texture.h"
#include "Input.h"
#include "KeyCode.h"
#include "ItemRegistry.h"
#include "InventoryComponent.h"
#include "LevelLoader.h"

namespace PXG
{
	PXGGame::PXGGame() : Game()
	{


	}

	void PXGGame::Initialize()
	{
		std::ifstream item_config(config::PXG_CONFIGS_PATH + "item_config.json");
		
		ItemRegistry::LoadConfig(&item_config);

		
		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E,KeyCode::K,KeyCode::J,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse,KeyCode::Enter);

		GetWorld()->name = "World";

		//---------------------------Initialize Textures---------------------------------------//
		Texture diffuse1(config::PXG_INDEPENDENT_TEXTURES_PATH + "diffuse1.jpg",TextureType::DIFFUSE);
		Texture raphsTexture(config::PXG_INDEPENDENT_TEXTURES_PATH + "texture.png", TextureType::DIFFUSE);

		//------------------------- Initialize Materials ---------------------------//
		std::shared_ptr<StandardLitMaterial> litMaterial = std::make_shared<StandardLitMaterial>();
		std::shared_ptr<ColorMaterial> defaultColorMat = std::make_shared<ColorMaterial>();
		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(1,0,0));
		std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();
		std::shared_ptr<ColorMaterial> yellowColorMat = std::make_shared<ColorMaterial>(Vector3(1, 1, 0));

		//--------------------------Initialize GameObjects and their Components--------------------------------//

		std::shared_ptr<RotatorComponent> rotator = std::make_shared<RotatorComponent>(Vector3(0, 0.77, 0.77), 1.0f);
		auto inventory = std::make_shared<Inventory>();
		GameObj firstObj = Instantiate();
		firstObj->name = "firstObj";
		firstObj->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "chopper/chopper.obj");
		firstObj->GetMeshComponent()->SetMaterial(litMaterial);
		firstObj->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
		firstObj->AddComponent(rotator);
		firstObj->AddComponent(inventory);

		inventory->AddItem(ItemRegistry::LookUpItem("Wood"));


		for (auto item_id : inventory->EnumerateUniqueItems())
		{
			auto [fname,pname] = ItemRegistry::GetItemInfo(item_id);
			Debug::Log("Player has items {}:{}", fname, pname);
		}


		
		world->AddToChildren(firstObj);

		
		GameObj childOfFirst = Instantiate();
		childOfFirst->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "teapot/teapot.obj");
		childOfFirst->GetMeshComponent()->SetMaterial(bluetColorMat);
		childOfFirst->GetTransform()->SetLocalPosition(Vector3(0, 0, -5));
		firstObj->AddToChildren(childOfFirst);
		//world->AddToChildren(childOfFirst);

		std::shared_ptr<CameraComponent> camera = std::make_shared<CameraComponent>();
		std::shared_ptr<FreeMovementComponent> movementComponent = std::make_shared<FreeMovementComponent>();
		std::shared_ptr<RotatorComponent> camRotator = std::make_shared<RotatorComponent>(Vector3(0, 1.0, 0.0), 1.0f);
		GameObj cameraObj = Instantiate();
		cameraObj->name = "cameraObj";
		cameraObj->AddComponent(camera);
		cameraObj->AddComponent(movementComponent);
		//cameraObj->AddComponent(camRotator);
		world->AddToChildren(cameraObj);
		
		cameraObj->GetTransform()->SetLocalPosition(Vector3(600, 600, 600));
	
		cameraObj->GetTransform()->rotate(Vector3(1, 0, 0), -20.0f);
		cameraObj->GetTransform()->rotate(Vector3(0, 1, 0), 45);



		GameObj TileMap = Instantiate();
		world->AddToChildren(TileMap);
		std::shared_ptr<MapMovementComponent> mapMovement = std::make_shared<MapMovementComponent>();
		mapMovement->SetMap(TileMap);
		GameObj movementHandler = Instantiate();
		movementHandler->name = "Movement";
		movementHandler->AddComponent(mapMovement);
		

		world->AddToChildren(movementHandler);
		//--------------------------- Instantiate lights -----------------------------------//


		std::shared_ptr<LightComponent> light1 = std::make_shared<LightComponent>();
		light1->SetIntensity(200.0f);

		GameObj firstLightObj = Instantiate();
		firstLightObj->GetTransform()->SetLocalPosition(Vector3(10.0f, 0, 0));
		firstLightObj->GetTransform()->Scale(Vector3(0.2f, 0.2f, 0.2f));
		firstLightObj->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "cube.obj");
		firstLightObj->GetMeshComponent()->SetMaterial(yellowColorMat);
		firstLightObj->name = "light1";
		firstLightObj->AddComponent(light1);
		world->AddToChildren(firstLightObj);
		

		std::shared_ptr<LightComponent> light2 = std::make_shared<LightComponent>();
		light2->SetIntensity(200.0f);

		GameObj secondLightObj = Instantiate();
		secondLightObj->GetTransform()->SetLocalPosition(Vector3(-10.0f, 0, 0));
		secondLightObj->GetTransform()->Scale(Vector3(0.2f, 0.2f, 0.2f));
		secondLightObj->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "cube.obj");
		secondLightObj->GetMeshComponent()->SetMaterial(yellowColorMat);
		secondLightObj->name = "light2";
		secondLightObj->AddComponent(light2);
		world->AddToChildren(secondLightObj);

		Debug::Log("light count {0}", world->GetLightCount());

		std::shared_ptr<RotatorComponent> orthoRotator = std::make_shared<RotatorComponent>(Vector3(0, 1.0, 0.0), 1.0f);

		

		float offset = 100.0f;
		int xCount = 5;
		int yCount = 5;


		auto level_loader = std::make_shared<LevelLoader>();

		TileMap->AddComponent(level_loader);

		std::ifstream level_config(config::PXG_CONFIGS_PATH + "level_data.json");
		
		level_loader->LoadLevel(level_config, this);
		

		/*
		for (int x = 0; x < xCount; x++)
		{
			for (int y = 0; y < yCount; y++)
			{
				GameObj orthoObject = Instantiate();

				TileMap->AddToChildren(orthoObject);

				orthoObject->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "cube.obj");
				orthoObject->GetMeshComponent()->SetMaterial(textureMaterial);
				orthoObject->GetTransform()->Scale(glm::vec3(50));
				//orthoObject->AddComponent(orthoRotator);
				orthoObject->GetTransform()->SetLocalPosition(Vector3(x*offset-400, 0, y*offset-400));
				orthoObject->GetMeshComponent()->AddTextureToMeshAt(raphsTexture, 0);
				//world->AddToChildren(orthoObject);
			}
		}
		*/
		//GameObj orthoObject = Instantiate();
		//orthoObject->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "cube.obj");
		//orthoObject->GetMeshComponent()->SetMaterial(textureMaterial);
		//orthoObject->GetTransform()->Scale(glm::vec3(50));
		////orthoObject->AddComponent(orthoRotator);
		//orthoObject->GetTransform()->SetLocalPosition(Vector3(0.0f,0,0 ));
		//orthoObject->GetMeshComponent()->AddTextureToMeshAt(raphsTexture,0);
		//world->AddToChildren(orthoObject);


		//GameObj orthoObject2 = Instantiate();
		//orthoObject2->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "cube.obj");
		//orthoObject2->GetMeshComponent()->SetMaterial(textureMaterial);
		//orthoObject2->GetTransform()->Scale(glm::vec3(50));
		////orthoObject->AddComponent(orthoRotator);
		//orthoObject2->GetTransform()->SetLocalPosition(Vector3(300.0f, 0, 0));
		//orthoObject2->GetMeshComponent()->AddTextureToMeshAt(raphsTexture, 0);
		//world->AddToChildren(orthoObject2);
	
	}

	void PXGGame::Start()
	{
		world->Start();
	}

	void PXGGame::Update()
	{
	}

	void PXGGame::FixedUpdate(float tick)
	{
		world->FixedUpdate(tick);
	}

	
}

