#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "PXGWindow.h"
#include <iostream>

#include "Mathf.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Debug.h"
#include "Input.h"


constexpr int width = 800;
constexpr int height = 600;

int main()
{
	Debug::Log(Verbosity::Info, "PXG is running");

	Input::AddKeysToTrack(
		KeyCode::A,KeyCode::W,KeyCode::S,KeyCode::D,
		KeyCode::LeftMouse,KeyCode::RightMouse,KeyCode::MiddleMouse);

	Debug::Log(Verbosity::Info, "KeyCount {0}", Input::GetTrackedKeyCount());

	//initialize glfw

	//TODO refactor glfw stuff
	
	PXG::PXGWindow::Init();

	GLFWwindow* window = glfwCreateWindow(width, height, "PXG3D", NULL, NULL);
	
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
		
	while (!glfwWindowShouldClose(window))
	{
		//track current time


		Input::PollEvents();


		if (Input::GetKey(KeyCode::LeftMouse))
		{
			Debug::Log(Verbosity::Debug, "left mouse");
		}

		//while physics tick not over

			//fixed update on all game objects in currently loaded world

			//physics sub-system update

			//animation update



		//frame update on all game objects in currently loaded world


		//late update on all game objects in currenty loaded world

		//rendering sub-system update


		glfwSwapBuffers(window);

		Input::LateUpdateTrackedKeyStates();
		
	}
}