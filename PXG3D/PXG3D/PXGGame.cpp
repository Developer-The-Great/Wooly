#include "PXGGame.h"
#include "FreeMovementComponent.h"
#include "World.h"

#include "FileConfig.h"

#include "ColorMaterial.h"
#include "TextureMaterial.h"
#include "StandardLitMaterial.h"

#include "LightComponent.h"

#include "CameraComponent.h"
#include "RotatorComponent.h"
#include "Texture.h"
#include "Input.h"
#include "KeyCode.h"

namespace PXG
{
	PXGGame::PXGGame() : Game()
	{


	}

	void PXGGame::Initialize()
	{
		Input::AddKeysToTrack(
			KeyCode::A, KeyCode::W, KeyCode::S, KeyCode::D, KeyCode::Q, KeyCode::E,
			KeyCode::LeftMouse, KeyCode::RightMouse, KeyCode::MiddleMouse);

		GetWorld()->name = "World";

		//---------------------------Initialize Textures---------------------------------------//
		Texture diffuse1(config::PXG_INDEPENDENT_TEXTURES_PATH + "diffuse1.jpg",TextureType::DIFFUSE);

		


		//------------------------- Initialize Materials ---------------------------//
		std::shared_ptr<StandardLitMaterial> litMaterial = std::make_shared<StandardLitMaterial>();
		std::shared_ptr<ColorMaterial> defaultColorMat = std::make_shared<ColorMaterial>();
		std::shared_ptr<ColorMaterial> bluetColorMat = std::make_shared<ColorMaterial>(Vector3(1,0,0));
		std::shared_ptr<TextureMaterial> textureMaterial = std::make_shared<TextureMaterial>();
		std::shared_ptr<ColorMaterial> yellowColorMat = std::make_shared<ColorMaterial>(Vector3(1, 1, 0));

		//--------------------------Initialize GameObjects and their Components--------------------------------//

		std::shared_ptr<RotatorComponent> rotator = std::make_shared<RotatorComponent>(Vector3(0, 0.77, 0.77), 1.0f);

		GameObj firstObj = Instantiate();
		firstObj->name = "firstObj";
		firstObj->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "chopper/chopper.obj");
		firstObj->GetMeshComponent()->SetMaterial(litMaterial);
		firstObj->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
		firstObj->AddComponent(rotator);
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
		cameraObj->AddComponent(camRotator);
		world->AddToChildren(cameraObj);
		
		cameraObj->GetTransform()->SetLocalPosition(Vector3(0, 1.8, 5.3));


		



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






		/*GameObj orthoObject = Instantiate();
		orthoObject->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "chopper/chopper.obj");
		orthoObject->GetMeshComponent()->SetMaterial(litMaterial);
		orthoObject->GetTransform()->Scale(Vector3(200.2f, 200.2f, 200.2f));
		orthoObject->AddComponent(rotator);
		orthoObject->GetTransform()->SetLocalPosition(Vector3(1.5f, 2.0f, 0));
		world->AddToChildren(orthoObject);*/


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

