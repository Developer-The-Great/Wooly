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
namespace PXG
{


	PXGGame::PXGGame() : Game()
	{
	}

	void PXGGame::Initialize()
	{
		AudioClip clip = AudioEngine::GetInstance().createClip("ImportantSoundFile.wav");
		AudioEngine::GetInstance().Loop(true, clip);
		//AudioEngine::GetInstance().Play(clip);



		font = new Font(config::PXG_FONT_PATH + "Roboto-Regular.ttf", 20);
		std::ifstream item_config(config::PXG_CONFIGS_PATH + "item_config.json");

		ItemRegistry::LoadConfig(&item_config);

		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E, KeyCode::K, KeyCode::J, KeyCode::Z,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse, KeyCode::Enter);


		//---------------------------Initialize Textures---------------------------------------//
		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(0, 0, 1));
		std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();



		//--------------------------Initialize GameObjects and their Components--------------------------------//
		auto camera = std::make_shared<CameraComponent>();
		auto movementComponent = std::make_shared<FreeMovementComponent>();
		auto raycaster = std::make_shared<RayCastShooter>();
		auto rayCastHandler = std::make_shared<RayCastHitHandler>();
		auto cameraRotator = std::make_shared<CameraRotator>();

		//--------------------------Initialize UI and their Components--------------------------------//


		//--------------------------SetUpUICanvas--------------------------------//
		auto UICam = std::make_shared<CameraComponent>();
		GameObj UICanvasCam = InstantiateUIObject();
		canvas->AddToChildren(UICanvasCam);
		UICanvasCam->name = "UICAM";
		UICanvasCam->AddComponent(UICam);

		//half of game width and half of game height
		UICanvasCam->GetTransform()->SetLocalPosition(Vector3(ScreenSize::WIDTH / 2, ScreenSize::HEIGHT / 2, 0));
		Vector2 BGPos = Vector2(ScreenSize::WIDTH / 2, ScreenSize::HEIGHT / 2);


		//first view
		GameObj Background1 = canvas->createCanvasObject(BGPos, BGPos, "Background1", "StartScreen.png");
		GameObj firstBGbutton = canvas->createCanvasObject(Vector2(600, 160), Vector2(250, 100), "button2", "gray.png");
		firstBGbutton->SetWorld(canvas);
		firstBGbutton->GetMeshComponent()->DisableRender(true);

		//second view
		GameObj Background2 = canvas->createCanvasObject(BGPos, BGPos, "background2", "WorldSelection.png");
		Background2->GetMeshComponent()->DisableRender(true);
		GameObj bg2buttonObj = canvas->createCanvasObject(Vector2(ScreenSize::WIDTH / 2, ScreenSize::HEIGHT / 2), Vector2(200, 200), "button3", "alpha256x256.png");
		bg2buttonObj->GetMeshComponent()->DisableRender(false);


		//third view 
		GameObj Background3 = canvas->createCanvasObject(BGPos, BGPos, "background3", "Map.png");
		Background3->GetMeshComponent()->DisableRender(true);
		GameObj bg3buttonObj = canvas->createCanvasObject(Vector2(ScreenSize::WIDTH / 5*3-200, ScreenSize::HEIGHT / 5), Vector2(150, 200), "button4", "alpha256x256.png");
		bg3buttonObj->GetMeshComponent()->DisableRender(true);



		//start button
		auto bg1Button = std::make_shared<ButtonComponent>();
		auto onClick = new SpecificOnClick();
		firstBGbutton->AddComponent(bg1Button);
		bg1Button->SetOwner(firstBGbutton);
		bg1Button->attach(onClick);
		onClick->setObjectToDisable(Background1);
		onClick->setObjectToDisable(firstBGbutton);
		onClick->setObjectToEnable(Background2);



		//centerd button
		auto bg2Button1 = std::make_shared<ButtonComponent>();
		auto bg2Onclick1 = new SpecificOnClick();
		bg2buttonObj->AddComponent(bg2Button1);
		bg2Button1->SetOwner(bg2buttonObj);
		bg2Button1->attach(bg2Onclick1);
		bg2Onclick1->setObjectToDisable(bg2buttonObj);
		bg2Onclick1->setObjectToDisable(Background2);
		bg2Onclick1->setObjectToEnable(Background3);
		bg2Onclick1->setObjectToEnable(bg3buttonObj);

		auto bg3Button = std::make_shared<ButtonComponent>();
		auto bg3OnClick = new SpecificOnClick();
		bg3buttonObj->AddComponent(bg3Button);
		bg3Button->SetOwner(bg3buttonObj);
		bg3Button->attach(bg3OnClick);
		bg3OnClick->setObjectToDisable(Background3);
		bg3OnClick->setObjectToDisable(bg3buttonObj);
		bg3OnClick->setRay(raycaster);



		//--------------------------SetUpCam--------------------------------//
		GameObj cameraObj = Instantiate();
		cameraObj->name = "cameraObj";

		cameraObj->AddComponent(camera);
		cameraObj->AddComponent(movementComponent);
		cameraObj->AddComponent(raycaster);
		cameraObj->AddComponent(rayCastHandler);
		cameraObj->AddComponent(cameraRotator);

		world->AddToChildren(cameraObj);

		cameraObj->GetTransform()->SetLocalPosition(Vector3(600, 450, 600));
		cameraObj->GetTransform()->rotate(Vector3(1, 0, 0), -20.0f);
		cameraObj->GetTransform()->rotate(Vector3(0, 1, 0), 45);

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




		//--------------------------- Map movement -----------------------------------//
		GameObj TileMap = MakeChild("TileMap");

		std::shared_ptr<MapMovementComponent> mapMovement = std::make_shared<MapMovementComponent>();
		auto jumper = std::make_shared<JumperComponent>();
		mapMovement->attach(jumper.get());
		mapMovement->attach(pRotator.get());
		Player->AddComponent(jumper);

		/*auto Background = MakeChild("bg");
		Background->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "plane.obj");
		Background->GetMeshComponent()->AddTextureToMeshAt({ config::PXG_INDEPENDENT_TEXTURES_PATH + "bg-image.png", TextureType::DIFFUSE }, 0);
		Background->GetMeshComponent()->SetMaterial(std::make_shared<TextureMaterial>());
		Background->GetTransform()->Scale(glm::vec3{ 800,0,600 });
		Background->GetTransform()->rotate(Vector3(1, 0, 0), 70.0f);
		Background->GetTransform()->rotate(Vector3(0, 1, 0), 45);
		Background->GetTransform()->SetLocalPosition({ -600,0,-600 });*/




		mapMovement->subscribe(*raycaster);
		rayCastHandler->subscribe(*raycaster);
		mapMovement->SetMap(TileMap);

		GameObj movementHandler = Instantiate();
		movementHandler->name = "Movement";
		movementHandler->AddComponent(mapMovement);

		world->AddToChildren(movementHandler);


		//--------------------------- Instantiate Cubes -----------------------------------//
		GameObj NodesObj = MakeChild("NodesObj");
		GameObj TriggerHandler = MakeChild("TriggerHand");
		auto triggerComp = std::make_shared<TriggerComponent>();
		auto levelLoader = std::make_shared<LevelLoader>();
		TileMap->AddComponent(levelLoader);

		auto nodeGraph = std::make_shared<NodeGraph>();
		NodesObj->AddComponent(nodeGraph);


		std::vector<NodeToPositionContainer> nodeToPositionContainer;

		std::ifstream level_config(config::PXG_CONFIGS_PATH + "level_data.json");
		levelLoader->LoadLevel(level_config, this, nodeGraph, nodeToPositionContainer, mapMovement, rayCastHandler);
		rayCastHandler->setMapMovement(mapMovement);
		nodeGraph->generateConnections(nodeToPositionContainer);

		rayCastHandler->setNodeGraph(nodeGraph);

		/*		auto onClickTrigger = std::make_shared<OnClickTrigger>();
		auto nodeGraphDistTrigger = std::make_shared<NodeGraphDistTrigger>(*rayCastHandler);

		auto compoundTrigger = std::make_shared<CompoundDistanceOnClickTrigger>(onClickTrigger, nodeGraphDistTrigger);

		auto nodeGraphMovementRayCastCon = std::make_shared<NodeGraphMMC_RCS_Con>();

		nodeGraphMovementRayCastCon->subscribe(*raycaster);
		nodeGraphMovementRayCastCon->subscribe(*mapMovement);

		Player->AddComponent(onClickTrigger);
		Player->AddComponent(nodeGraphDistTrigger);
		Player->AddComponent(compoundTrigger);
		Player->AddComponent(nodeGraphMovementRayCastCon);


		auto dbgnode = std::find_if(TileMap->GetChildren().begin(), TileMap->GetChildren().end(), [](GameObj go)
		{
			if (go->HasComponent<Node>())
				if (go->GetComponent<Node>()->getPos() == Vector3{ 1,0,2 })
					return true;


			return false;
		});

		if(dbgnode != TileMap->GetChildren().end())
		{
			Debug::Log("Found a things");
		}

		onClickTrigger->SetSubject(*dbgnode,raycaster);
		class DebugLogger : public subscriber_base,public Component
		{
		public:
			void onNotify(subject_base* subject_base, subject_base::event_t event) override { Debug::Log(Verbosity::Warning, "HI!"); }
		};
		auto logger = std::make_shared<DebugLogger>();
		logger->subscribe(*compoundTrigger);
		bcomp->subscribe(*compoundTrigger);
		bcomp->AddListener(compoundTrigger->ON_TRIGGER_RAISED);

		Player->AddComponent(logger);
		*/

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

