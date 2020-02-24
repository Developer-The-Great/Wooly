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
#include "TextComponent.h"
#include "ButtonComponent.h"
#include "LevelLoader.h"
#include "RayCastShooter.h"
#include "EnergyCounterComponent.h"

#include "SpecificOnClick.h"
#include "Subject.h"
#include "Subscriber.h"
#include "NodeGraph.h"
#include "ScreenSize.h"
namespace PXG
{
	PXGGame::PXGGame() : Game()
	{


	}

	//FontRenderer::render_queue render_queue;

	void PXGGame::Initialize()
	{
		font = new Font(config::PXG_FONT_PATH + "Roboto-Regular.ttf",20);
		std::ifstream item_config(config::PXG_CONFIGS_PATH + "item_config.json");

		ItemRegistry::LoadConfig(&item_config);

		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E, KeyCode::K, KeyCode::J,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse, KeyCode::Enter);

		//---------------------------Initialize Textures---------------------------------------//


		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(0, 0, 1));
		std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();

		//--------------------------Initialize GameObjects and their Components--------------------------------//


		auto camera				= std::make_shared<CameraComponent>();
		auto movementComponent	= std::make_shared<FreeMovementComponent>();
		auto energyCounter		= std::make_shared<EnergyCounterComponent>(frender, font);
		auto raycaster			= std::make_shared<RayCastShooter>();

		//--------------------------Initialize UI and their Components--------------------------------//

		std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>();
		std::shared_ptr<TextComponent> textComp2 = std::make_shared<TextComponent>();

		textComp->InitText(frender);
		textComp->SetFont(font);
		textComp->setRelativePosition(Vector2(50, 50));
		textComp->setString("Hello World");

		textComp2->InitText(frender);
		textComp2->SetFont(font);
		textComp2->setRelativePosition(Vector2(350, 150));
		textComp2->setString("Hello World2");

		std::shared_ptr<ButtonComponent> buttonComp = std::make_shared<ButtonComponent>();
		subscriber_base*  onClick = new SpecificOnClick();
		//button with onclick component & text
		GameObj button = canvas->createCanvasObject(Vector2(100, 100), Vector2(100, 100), "Button1", bluetColorMat);
		button->SetWorld(canvas);
		button->AddComponent(buttonComp);
		buttonComp->attach(onClick);
		button->AddComponent(textComp);
		button->AddComponent(movementComponent);
		//empty UI object with text
		GameObj emptyUIObject = canvas->createEmptyCanvasObject();
		emptyUIObject->SetWorld(canvas);
		emptyUIObject->AddComponent(textComp2);

		//--------------------------SetUpUICanvas--------------------------------//

		auto UICam = std::make_shared<CameraComponent>();
		GameObj UICanvasCam = InstantiateUIObject();

		canvas->AddToChildren(UICanvasCam);

		UICanvasCam->name = "UICAM";
		UICanvasCam->AddComponent(UICam);

		//half of game width and half of game height
		UICanvasCam->GetTransform()->SetLocalPosition(Vector3(300, 200, 0));

		//--------------------------SetUpCam--------------------------------//

		GameObj cameraObj = Instantiate();
		cameraObj->name = "cameraObj";

		cameraObj->AddComponent(camera);
		cameraObj->AddComponent(movementComponent);
		cameraObj->AddComponent(raycaster);
		cameraObj->AddComponent(energyCounter);

		world->AddToChildren(cameraObj);

		cameraObj->GetTransform()->SetLocalPosition(Vector3(600, 300, 600));
		cameraObj->GetTransform()->rotate(Vector3(1, 0, 0), -20.0f);
		cameraObj->GetTransform()->rotate(Vector3(0, 1, 0), 45);



		//--------------------------- Map movement -----------------------------------//


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


		//--------------------------- Instantiate Cubes -----------------------------------//

		energyCounter->subscribe(*mapMovement);
		mapMovement->subscribe(*energyCounter);


		//--------------------------- Instantiate Cubes -----------------------------------//
		GameObj NodesObj = MakeChild("NodesObj");

		auto level_loader = std::make_shared<LevelLoader>();
		TileMap->AddComponent(level_loader);

		auto node_graph = std::make_shared<NodeGraph>();
		NodesObj->AddComponent(node_graph);

		std::ifstream level_config(config::PXG_CONFIGS_PATH + "level_data.json");
		level_loader->LoadLevel(level_config, this, node_graph);
		node_graph->generateConnections();

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
		//frender->restore_queue(&render_queue);
		world->FixedUpdate(tick);
		canvas->FixedUpdate(tick);
	}
}

