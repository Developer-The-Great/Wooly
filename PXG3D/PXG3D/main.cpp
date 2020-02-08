#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "PXGWindow.h"
#include <iostream>

#include "Mathf.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Debug.h"
#include "Input.h"
#include "Transform.h"

#include "PXGGame.h"
#include "core/RenderingEngine.h"
#include "Time.h"
#include "World.h"
#include "KeyCode.h"

constexpr int width = 800;
constexpr int height = 600;

int main()
{
	Debug::SetDebugState(true);

	Debug::Log(Verbosity::Info, "PXG is running");

	Input::AddKeysToTrack(
		KeyCode::A,KeyCode::W,KeyCode::S,KeyCode::D,KeyCode::Q,KeyCode::E,
		KeyCode::LeftMouse,KeyCode::RightMouse,KeyCode::MiddleMouse);

	Debug::Log(Verbosity::Info, "KeyCount {0}", Input::GetTrackedKeyCount());

	//--------------------------- Initializing GLFW Stuff--------------------//

	//TODO refactor glfw stuff
	
	PXG::PXGWindow::Init();

	GLFWwindow* window = PXG::PXGWindow::MakeGLFWWindow(width, height, "PXG3D Game", NULL, NULL);
	
	if (window == nullptr)
	{
		Debug::Log(Verbosity::Error, "Failed to create window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//GLAD manages function pointers for OPENGL, we must initialize GLAD before we call any OPENGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Debug::Log(Verbosity::Error, "Failed to initialize glad");
		return -1;
	}
	//tell OpenGL the size of the rendering window

	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, PXG::PXGWindow::key_callback);
	glfwSetMouseButtonCallback(window, PXG::PXGWindow::mouse_button_callback);


	//--------------------Initialize Game-----------------------//

	std::shared_ptr<PXG::PXGGame> gamePtr = std::make_shared<PXG::PXGGame>();

	//--------------------Initialize Rendering Engine-----------------------//

	std::unique_ptr<PXG::RenderingEngine> renderingEngine = std::make_unique<PXG::RenderingEngine>();
	renderingEngine->SetWorld(gamePtr->GetWorld());

	//--------------------Initialize Physics Engine -------------------------//

	std::unique_ptr<PXG::PhysicsEngine> physicsEngine = std::make_unique<PXG::PhysicsEngine>();
	physicsEngine->SetWorld(gamePtr->GetWorld());

	//------------------- Initialize World -----------------------------------//
	//this is very weird. Will fix it later
	gamePtr->GetWorld()->GetMeshComponent()->SetOwner(gamePtr->GetWorld());
	


	int frameTickStored = 6;
	int estimatedInitialFPS = 60.0f;

	std::shared_ptr<PXG::Time> time = std::make_shared<PXG::Time>(frameTickStored, estimatedInitialFPS);



	
	gamePtr->Initialize();
	gamePtr->Start();

	Debug::Log("children in world {0}", gamePtr->GetWorld()->GetImmediateChildrenCount());

	//-------------------------------------- GAME LOOP ------------------------------------//
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0, 0, 0, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//track current time
		time->UpdateTimePassed();

		Input::PollEvents();

		physicsEngine->SetTickRemaining(time->GetAverageDeltaTime());
		
		while (physicsEngine->IsTicking())
		{
			float tick = physicsEngine->DecreaseRemainingTickTime();
			//fixed update on game
			gamePtr->FixedUpdate(tick);

		}
			
		renderingEngine->RenderCurrentlySetWorld();

		glfwSwapBuffers(window);

		Input::LateUpdateTrackedKeyStates();

		time->UpdateAverageTime();
		

	}




	return 0;
}