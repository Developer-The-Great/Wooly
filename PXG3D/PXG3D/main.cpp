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
#include "RenderingEngine.h"
#include "Time.h"
#include "World.h"
#include "Canvas.h"
#include "KeyCode.h"

constexpr int width = 800;
constexpr int height = 600;

int main()
{
	Debug::SetDebugState(true);

	Debug::Log(Verbosity::Info, "PXG is running");

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


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Debug::Log(Verbosity::Error, "Failed to initialize glad");
		return -1;
	}

	//tell OpenGL the size of the rendering window
	glViewport(0, 0, width, height);
	//TODO LOW PRIORITY : refactor callbacks
	glfwSetKeyCallback(window, PXG::PXGWindow::key_callback);
	glfwSetMouseButtonCallback(window, PXG::PXGWindow::mouse_button_callback);
	glfwSetCursorPosCallback(window, PXG::PXGWindow::mouse_position_callback);

	//--------------------Initialize Game-----------------------//

	std::shared_ptr<PXG::PXGGame> gamePtr = std::make_shared<PXG::PXGGame>();
	gamePtr->GetWorld()->SetPhysicsComponentDrawActive(false);

	//--------------------Initialize Rendering Engine-----------------------//

	std::unique_ptr<PXG::RenderingEngine> renderingEngine = std::make_unique<PXG::RenderingEngine>();
	renderingEngine->SetWorld(gamePtr->GetWorld());
	renderingEngine->SetCanvas(gamePtr->GetCanvas());


	//--------------------Initialize Physics Engine -------------------------//

	std::unique_ptr<PXG::PhysicsEngine> physicsEngine = std::make_unique<PXG::PhysicsEngine>();
	physicsEngine->SetWorld(gamePtr->GetWorld());

	//------------------- Initialize World -----------------------------------//

	int frameTickStored = 6;
	int estimatedInitialFPS = 60;

	std::shared_ptr<PXG::Time> time = std::make_shared<PXG::Time>(frameTickStored, estimatedInitialFPS);

	gamePtr->frender = &renderingEngine->GetFontRenderer();
	gamePtr->Initialize();
	gamePtr->Start();

	Debug::Log("children in world {0}", gamePtr->GetWorld()->GetImmediateChildrenCount());
	Debug::Log("children in Canvas {0}", gamePtr->GetCanvas()->GetImmediateChildrenCount());


	//-------------------------------------- GAME LOOP ------------------------------------//
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.4f, 0.5f, 0.8f, 1.0f);

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

		glDisable(GL_DEPTH_TEST);
		renderingEngine->RenderCanvas();

		glfwSwapBuffers(window);

		Input::LateUpdateTrackedKeyStates();

		time->UpdateAverageTime();


	}




	return 0;
}