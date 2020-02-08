#pragma once

struct GLFWwindow;

namespace PXG
{
	
	namespace PXGWindow
	{
		void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);

		void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

		GLFWwindow* MakeGLFWWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

		void Init();


	}
}
