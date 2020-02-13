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

#include "RayCastShooter.h"

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


		std::shared_ptr<CameraComponent> camera = std::make_shared<CameraComponent>();
		std::shared_ptr<FreeMovementComponent> movementComponent = std::make_shared<FreeMovementComponent>();
		std::shared_ptr<RotatorComponent> camRotator = std::make_shared<RotatorComponent>(Vector3(0, 1.0, 0.0), 1.0f);
		std::shared_ptr<RayCastShooter> raycaster = std::make_shared<RayCastShooter>();

		GameObj cameraObj = Instantiate();
		cameraObj->name = "cameraObj";
		cameraObj->AddComponent(camera);
		cameraObj->AddComponent(movementComponent);
		cameraObj->AddComponent(raycaster);
		//cameraObj->AddComponent(camRotator);
		world->AddToChildren(cameraObj);

		cameraObj->GetTransform()->SetLocalPosition(Vector3(600,300, 600));

		cameraObj->GetTransform()->rotate(Vector3(1, 0, 0), -20.0f);
		cameraObj->GetTransform()->rotate(Vector3(0, 1, 0),45);

		GameObj debuggerObj = Instantiate();
		debuggerObj->GetTransform()->SetLocalPosition(Vector3(-0.879406929, 6.78816652, 0.372487307));
		debuggerObj->GetTransform()->Scale(Vector3(0.2f, 0.2f, 0.2f));
		debuggerObj->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "cube.obj");
		debuggerObj->GetMeshComponent()->SetMaterial(yellowColorMat);
		debuggerObj->name = "debuggerObj";
		world->AddToChildren(debuggerObj);

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


		Debug::Log("light count {0}", world->GetLightCount());

		std::shared_ptr<RotatorComponent> orthoRotator = std::make_shared<RotatorComponent>(Vector3(0, 1.0, 0.0), 1.0f);

		float offset = 100.0f;
		int xCount = 5;
		int yCount = 5;


		auto level_loader = std::make_shared<LevelLoader>();

		TileMap->AddComponent(level_loader);

		std::ifstream level_config(config::PXG_CONFIGS_PATH + "level_data.json");

		level_loader->LoadLevel(level_config, this);

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

