#include "PXGGame.h"
#include "PhysicsEngine.h"
#include "HitInfo.h"
#include "FreeMovementComponent.h"
#include "World.h"
#include "Canvas.h"

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

#include "ButtonComponent.h"
#include "LevelLoader.h"
#include "RayCastShooter.h"
#include "EnergyCounterComponent.h"


namespace PXG
{
	PXGGame::PXGGame() : Game()
	{


	}

	FontRenderer::render_queue render_queue;

	void PXGGame::Initialize()
	{
		font = new Font(config::PXG_FONT_PATH + "Roboto-Regular.ttf");

		
		std::ifstream item_config(config::PXG_CONFIGS_PATH + "item_config.json");

		ItemRegistry::LoadConfig(&item_config);

		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E,KeyCode::K,KeyCode::J,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse,KeyCode::Enter);

		//---------------------------Initialize Textures---------------------------------------//


		//------------------------- Initialize Materials ---------------------------//
		std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();

		//--------------------------Initialize GameObjects and their Components--------------------------------//


		auto camera				= std::make_shared<CameraComponent>();
		auto movementComponent	= std::make_shared<FreeMovementComponent>();
		auto energyCounter		= std::make_shared<EnergyCounterComponent>(frender, font);
		auto raycaster			= std::make_shared<RayCastShooter>();

		GameObj cameraObj = MakeChild("cameraObj");

		cameraObj->AddComponent(camera);
		cameraObj->AddComponent(movementComponent);
		cameraObj->AddComponent(energyCounter);
		cameraObj->AddComponent(raycaster);
		

		cameraObj->GetTransform()->SetLocalPosition(Vector3(600,300, 600));
		cameraObj->GetTransform()->rotate(Vector3(1, 0, 0), -20.0f);
		cameraObj->GetTransform()->rotate(Vector3(0, 1, 0),45);

		//--------------------------SetUpUICanvas--------------------------------//

		auto UICam = std::make_shared<CameraComponent>();
		
		GameObj UICanvasCam = InstantiateUIObject();
		canvas->AddToChildren(UICanvasCam);
		
		UICanvasCam->name = "UICAM";
		UICanvasCam->AddComponent(UICam);
		

		//half of game width and half of game height
		UICanvasCam->GetTransform()->SetLocalPosition(Vector3(300, 200, 0));


		//------------------------------ Player --------------------------------------//

		GameObj Player = MakeChild("Player");
		
		Player->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "character.obj");
		Player->GetMeshComponent()->AddTextureToMeshAt({ config::PXG_INDEPENDENT_TEXTURES_PATH + "texture.png",TextureType::DIFFUSE }, 0);
		Player->GetTransform()->SetLocalPosition({ 0,100,0 });
		Player->GetTransform()->Scale(glm::vec3{ 100 });
		Player->GetMeshComponent()->SetMaterial(textureMaterial);

		
		//--------------------------- Map movement -----------------------------------//

		GameObj TileMap = MakeChild("TileMap");
		
		std::shared_ptr<MapMovementComponent> mapMovement = std::make_shared<MapMovementComponent>();
		
		mapMovement->subscribe(*raycaster);

		mapMovement->SetMap(TileMap);
		
		GameObj movementHandler = Instantiate();
		movementHandler->name = "Movement";
		movementHandler->AddComponent(mapMovement);

		world->AddToChildren(movementHandler);

		energyCounter->subscribe(*mapMovement);
		mapMovement->subscribe(*energyCounter);


		//--------------------------- Instantiate Cubes -----------------------------------//

		auto level_loader = std::make_shared<LevelLoader>();

		TileMap->AddComponent(level_loader);

		std::ifstream level_config(config::PXG_CONFIGS_PATH + "level_data.json");
		level_loader->LoadLevel(level_config, this);

	}

	GameObj PXGGame::MakeChild(const std::string& name)
{
		GameObj go = Instantiate();
		go->name = name;
		world->AddToChildren(go);
		return go;
	}

	void PXGGame::Start()
	{
		world->Start();
		canvas->Start();
	}

	void PXGGame::Update()
	{
	}

	void PXGGame::FixedUpdate(float tick)
	{
		frender->restore_queue(&render_queue);
		world->FixedUpdate(tick);
		canvas->FixedUpdate(tick);
	}
}

