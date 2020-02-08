#include "PXGGame.h"
#include "FreeMovementComponent.h"
#include "World.h"

#include "FileConfig.h"
#include "ColorMaterial.h"
#include "CameraComponent.h"


namespace PXG
{
	PXGGame::PXGGame() : Game()
	{


	}

	void PXGGame::Initialize()
	{

		GetWorld()->name = "World";

		//------------------------- Initialize Materials ---------------------------//

		std::shared_ptr<ColorMaterial> defaultColorMat = std::make_shared<ColorMaterial>();


		GameObj firstObj = Instantiate();
		firstObj->name = "firstObj";
		
		firstObj->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "teapot/teapot.obj");
		firstObj->GetMeshComponent()->SetMaterial(defaultColorMat);




		std::shared_ptr<CameraComponent> camera = std::make_shared<CameraComponent>();
		std::shared_ptr<FreeMovementComponent> movementComponent = std::make_shared<FreeMovementComponent>();

		world->SetCamera(camera);
		GameObj cameraObj = Instantiate();
		cameraObj->name = "cameraObj";
		
		cameraObj->AddComponent(camera);
		camera->SetOwner(cameraObj);
		
		cameraObj->AddComponent(movementComponent);
		movementComponent->SetOwner(cameraObj);


		cameraObj->GetTransform()->SetLocalPosition(Vector3(0, 1.8, 5.3));

		
		



		//ColorMaterial * test = new ColorMaterial();


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

