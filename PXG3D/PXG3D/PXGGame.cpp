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


namespace PXG
{
	PXGGame::PXGGame() : Game()
	{


	}

	FontRenderer::render_queue render_queue;

	void PXGGame::Initialize()
	{
		std::ifstream item_config(config::PXG_CONFIGS_PATH + "item_config.json");

		ItemRegistry::LoadConfig(&item_config);


		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E,KeyCode::K,KeyCode::J,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse,KeyCode::Enter);

		GetWorld()->name = "World";
		GetCanvas()->name = "canvas";
		//---------------------------Initialize Textures---------------------------------------//

		Texture diffuse1(config::PXG_INDEPENDENT_TEXTURES_PATH + "diffuse1.jpg",TextureType::DIFFUSE);
		Texture raphsTexture(config::PXG_INDEPENDENT_TEXTURES_PATH + "texture.png", TextureType::DIFFUSE);

		//------------------------- Initialize Materials ---------------------------//
		std::shared_ptr<StandardLitMaterial> litMaterial = std::make_shared<StandardLitMaterial>();
		std::shared_ptr<ColorMaterial> defaultColorMat = std::make_shared<ColorMaterial>();
		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(0,0,1));
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

		//--------------------------SetUpUICanvas--------------------------------//

		std::shared_ptr<CameraComponent> UICam = std::make_shared<CameraComponent>();
		GameObj UICanvasCam = InstantiateUIObject();
		UICanvasCam->name = "UICAM";
		UICanvasCam->AddComponent(UICam);
		canvas->AddToChildren(UICanvasCam);

		//half of game width and half of game height
		UICanvasCam->GetTransform()->SetLocalPosition(Vector3(300, 200, 0));

		//--------------------------- Map movement -----------------------------------//


		GameObj TileMap = Instantiate();
		world->AddToChildren(TileMap);
		
		std::shared_ptr<MapMovementComponent> mapMovement = std::make_shared<MapMovementComponent>();
		mapMovement->subscribe(*raycaster);
		mapMovement->SetMap(TileMap);
		
		GameObj movementHandler = Instantiate();
		movementHandler->name = "Movement";
		movementHandler->AddComponent(mapMovement);

		world->AddToChildren(movementHandler);

		//--------------------------- Instantiate Cubes -----------------------------------//

		float offset = 100.0f;
		int xCount = 5;
		int yCount = 5;


		font = new Font(config::PXG_FONT_PATH + "Roboto-Regular.ttf");

		frender->text(font, "Hello World", 1, { 100,10 });

		render_queue = frender->save_queue();

		auto level_loader = std::make_shared<LevelLoader>();

		TileMap->AddComponent(level_loader);

		std::ifstream level_config(config::PXG_CONFIGS_PATH + "level_data.json");

		level_loader->LoadLevel(level_config, this);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

