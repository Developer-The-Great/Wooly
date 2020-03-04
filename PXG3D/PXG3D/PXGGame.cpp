#include "PXGGame.h"
#include "HitInfo.h"
#include "FreeMovementComponent.h"
#include "World.h"
#include "Canvas.h"
#include "FileConfig.h"
#include "ColorMaterial.h"
#include "TextureMaterial.h"
#include "MapMovementComponent.h"
#include "CameraComponent.h"
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
#include "TriggerComponent.h"
#include "NodeToPositionContainer.h"
#include "SpecificOnClick.h"
#include "Subject.h"
#include "Subscriber.h"
#include "NodeGraph.h"
#include "ScreenSize.h"
#include "RayCastHitHandler.h"
#include "AudioClip.hpp"
#include "AudioEngine.hpp"
#include "AudioSource.h"
#include "BridgeComponent.hpp"
#include "CompoundDistanceOnClickTrigger.hpp"
#include "PlayerRotatorComponent.h"
#include "JumperComponent.h"
#include "NodeGraphDistTrigger.h"
#include "OnClickTrigger.hpp"
#include "RotatorComponent.h"
#include "CameraRotator.h"
#include "OnMovePlayer.hpp"

namespace PXG
{


	PXGGame::PXGGame() : Game()
	{
	}

	void PXGGame::Initialize()
	{
		AudioClip clip = AudioEngine::GetInstance().createClip(config::PXG_SOUND_PATH + "Music/Level_Music.streamed.wav");
		AudioEngine::GetInstance().Loop(true, clip);
		AudioEngine::GetInstance().Play(clip);



		font = new Font(config::PXG_FONT_PATH + "Roboto-Regular.ttf", 20);

		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E, KeyCode::K, KeyCode::J, KeyCode::Z,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse, KeyCode::Enter);


		//---------------------------Initialize Textures---------------------------------------//
		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(0, 0, 1));
		std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();


		//--------------------------Initialize GameObjects and their Components--------------------------------//
		auto camera = std::make_shared<CameraComponent>();
		auto movementComponent = std::make_shared<FreeMovementComponent>();
		auto energyCounter = std::make_shared<EnergyCounterComponent>(frender, font);
		auto raycaster = std::make_shared<RayCastShooter>();
		auto rayCastHandler = std::make_shared<RayCastHitHandler>();
		auto cameraRotator = std::make_shared<CameraRotator>();

		//--------------------------Initialize UI and their Components--------------------------------//
		//std::shared_ptr<TextComponent> textComp = std::make_shared<TextComponent>();
		//std::shared_ptr<TextComponent> textComp2 = std::make_shared<TextComponent>();

		//std::shared_ptr<ButtonComponent> buttonComp = std::make_shared<ButtonComponent>();
		//subscriber_base*  onClick = new SpecificOnClick();
		////button with onclick component & text
		//GameObj button = canvas->createCanvasObject(Vector2(200, 100), Vector2(50, 50), "Button1", bluetColorMat);
		//button->SetWorld(canvas);
		//button->AddComponent(buttonComp);
		//buttonComp->attach(onClick);
		//button->AddComponent(textComp);
		//button->AddComponent(movementComponent);
		////empty UI object with text
		//GameObj emptyUIObject = canvas->createEmptyCanvasObject();
		//emptyUIObject->SetWorld(canvas);
		//emptyUIObject->AddComponent(textComp2);



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
		cameraObj->AddComponent(rayCastHandler);
		cameraObj->AddComponent(energyCounter);
		cameraObj->AddComponent(cameraRotator);

		world->AddToChildren(cameraObj);

		cameraObj->GetTransform()->SetLocalPosition(Vector3(600, 450, 600));
		cameraObj->GetTransform()->rotate(Vector3(1, 0, 0), -20.0f);
		cameraObj->GetTransform()->rotate(Vector3(0, 1, 0), 45);
		cameraObj->AddComponent(std::make_shared<CameraRotator>());

		//------------------------------ Player --------------------------------------//

		auto asource = std::make_shared<PXG::AudioSourceComponent>(clip);
		auto pRotator = std::make_shared<PlayerRotatorComponent>();
		GameObj Player = MakeChild("Player");

		Player->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "Timmy.obj");
		Player->GetMeshComponent()->AddTextureToMeshAt({ config::PXG_INDEPENDENT_TEXTURES_PATH + "TimmyTexture.png",TextureType::DIFFUSE }, 0);
		Player->GetTransform()->SetLocalPosition({ 50,100,50 });
		Player->GetTransform()->Scale(glm::vec3{ 100 });
		Player->AddComponent(asource);
		Player->GetMeshComponent()->SetMaterial(textureMaterial);
		Player->AddComponent(pRotator);
		pRotator->setInitForward();
		auto aplayer = std::make_shared<OnMoveAudioPlayer>();
		Player->AddComponent(aplayer);

		
		//--------------------------- Map movement -----------------------------------//

		//--------------------------- Map movement -----------------------------------//
		GameObj TileMap = MakeChild("TileMap");

		std::shared_ptr<MapMovementComponent> mapMovement = std::make_shared<MapMovementComponent>();
		auto jumper = std::make_shared<JumperComponent>();
		mapMovement->attach(jumper.get());
		aplayer->subscribe(*mapMovement);
		mapMovement->attach(pRotator.get());
		Player->AddComponent(jumper);

		auto Background = MakeChild("bg");
		Background->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "plane.obj");
		Background->GetMeshComponent()->AddTextureToMeshAt({ config::PXG_INDEPENDENT_TEXTURES_PATH + "bg-image.png", TextureType::DIFFUSE }, 0);
		Background->GetMeshComponent()->SetMaterial(std::make_shared<TextureMaterial>());
		Background->GetTransform()->Scale(glm::vec3{ 800,0,600 });
		Background->GetTransform()->rotate(Vector3(1, 0, 0), 70.0f);
		Background->GetTransform()->rotate(Vector3(0, 1, 0), 45);
		Background->GetTransform()->SetLocalPosition({ -600,0,-600 });

		
		

//		mapMovement->subscribe(*raycaster);
		rayCastHandler->subscribe(*raycaster);
		mapMovement->SetMap(TileMap);

		GameObj movementHandler = Instantiate();
		movementHandler->name = "Movement";
		movementHandler->AddComponent(mapMovement);

		//mapMovement->pushCommand(MapMovementComponent::LEFT);
		mapMovement->pushCommand(MapMovementComponent::DOWN);
		mapMovement->pushCommand(MapMovementComponent::DOWN);
		mapMovement->pushCommand(MapMovementComponent::DOWN);
		mapMovement->pushCommand(MapMovementComponent::DOWN);

		for(int i = 0; i< 16;++i)
		{
			mapMovement->pushCommand(MapMovementComponent::BACKWARD);
		}
		mapMovement->pushCommand(MapMovementComponent::RIGHT);
		mapMovement->pushCommand(MapMovementComponent::RIGHT);
		mapMovement->pushCommand(MapMovementComponent::RIGHT);

		mapMovement->overrideOffset(Vector3{ 3,4,16 });
		
		world->AddToChildren(movementHandler);


		//--------------------------- Instantiate Cubes -----------------------------------//
		energyCounter->subscribe(*mapMovement);
	//	mapMovement->subscribe(*energyCounter);


		//--------------------------- Instantiate Cubes -----------------------------------//
		GameObj NodesObj = MakeChild("NodesObj");
		GameObj TriggerHandler = MakeChild("TriggerHand");
		auto triggerComp = std::make_shared<TriggerComponent>();
		auto levelLoader = std::make_shared<LevelLoader>();
		TileMap->AddComponent(levelLoader);

		auto nodeGraph = std::make_shared<NodeGraph>();
		NodesObj->AddComponent(nodeGraph);


		std::vector<NodeToPositionContainer> nodeToPositionContainer;

		std::ifstream level_config(config::PXG_CONFIGS_PATH + "level1.json");
		levelLoader->LoadLevel(level_config, this, nodeGraph, nodeToPositionContainer, mapMovement);
		rayCastHandler->setMapMovement(mapMovement);
		nodeGraph->generateConnections(nodeToPositionContainer);

		rayCastHandler->setNodeGraph(nodeGraph);
		TriggerFactoryComponent::Build(Player, raycaster, rayCastHandler, mapMovement);
		

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

