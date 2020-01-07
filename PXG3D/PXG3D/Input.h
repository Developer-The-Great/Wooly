#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <map>
#include "Utils.h"

enum class KeyCode
{
 Space = GLFW_KEY_SPACE,              
 Apostophe = GLFW_KEY_APOSTROPHE,         
 Coma = GLFW_KEY_COMMA,            
 Minus = GLFW_KEY_MINUS,              
 Period = GLFW_KEY_PERIOD,          
 Slash = GLFW_KEY_SLASH,             
 Zero = GLFW_KEY_0,                  
 One = GLFW_KEY_1,                  
 Two = GLFW_KEY_2,                  
 Three = GLFW_KEY_3,                  
 Four = GLFW_KEY_4,                  
 Five = GLFW_KEY_5,                  
 Six = GLFW_KEY_6 ,                 
 Seven = GLFW_KEY_7,                  
 Eight = GLFW_KEY_8 ,               
 Nine = GLFW_KEY_9   ,               
 SemiColon = GLFW_KEY_SEMICOLON,          
 EqualKey = GLFW_KEY_EQUAL   ,           
 A = GLFW_KEY_A  ,                
 B = GLFW_KEY_B,                  
 C = GLFW_KEY_C,                  
 D =GLFW_KEY_D ,                 
 E = GLFW_KEY_E   ,               
F = GLFW_KEY_F,
G = GLFW_KEY_G,
H = GLFW_KEY_H,
I = GLFW_KEY_I,
J = GLFW_KEY_J,
K = GLFW_KEY_K,
L = GLFW_KEY_L,
M = GLFW_KEY_M,
N = GLFW_KEY_N,
O = GLFW_KEY_O,
P = GLFW_KEY_P,
Q = GLFW_KEY_Q,
R = GLFW_KEY_R,
S = GLFW_KEY_S,
T = GLFW_KEY_T,
U = GLFW_KEY_U,
V = GLFW_KEY_V,
W = GLFW_KEY_W,
X = GLFW_KEY_X,
Y = GLFW_KEY_Y,
Z = GLFW_KEY_Z,
LeftBracketKey = GLFW_KEY_LEFT_BRACKET,
Backlash = GLFW_KEY_BACKSLASH,
RightBracketKey = GLFW_KEY_RIGHT_BRACKET,
GraveAccentKey = GLFW_KEY_GRAVE_ACCENT,
WorldKey1 = GLFW_KEY_WORLD_1,
WorldKey2 = GLFW_KEY_WORLD_2,

/* Function keys */
Esc = GLFW_KEY_ESCAPE,
Enter = GLFW_KEY_ENTER,
Tab = GLFW_KEY_TAB,
Backspace = GLFW_KEY_BACKSPACE,
Insert = GLFW_KEY_INSERT,
Delete = GLFW_KEY_DELETE,
KeyRight = GLFW_KEY_RIGHT,
KeyLeft = GLFW_KEY_LEFT,
KeyDown = GLFW_KEY_DOWN,
Up = GLFW_KEY_UP,
PageUp = GLFW_KEY_PAGE_UP,
PageDown = GLFW_KEY_PAGE_DOWN,
Home = GLFW_KEY_HOME,
End = GLFW_KEY_END,
CapsLock = GLFW_KEY_CAPS_LOCK,
ScrollLock = GLFW_KEY_SCROLL_LOCK,
NumLock = GLFW_KEY_NUM_LOCK,
PrintScreen = GLFW_KEY_PRINT_SCREEN,
KeyPause = GLFW_KEY_PAUSE,
F1 = GLFW_KEY_F1,
F2 = GLFW_KEY_F2,
F3 = GLFW_KEY_F3,
F4 = GLFW_KEY_F4,
F5 = GLFW_KEY_F5,
F6 = GLFW_KEY_F6,
F7 = GLFW_KEY_F7,
F8 = GLFW_KEY_F8,
F9 = GLFW_KEY_F9,
F10 = GLFW_KEY_F10,
F11 = GLFW_KEY_F11,
F12 = GLFW_KEY_F12,
F13 = GLFW_KEY_F13,
F14 = GLFW_KEY_F14,
F15 = GLFW_KEY_F15,
F16 = GLFW_KEY_F16,
F17 = GLFW_KEY_F17,
F18 = GLFW_KEY_F18,
F19 = GLFW_KEY_F19,
F20 = GLFW_KEY_F20,
F21 = GLFW_KEY_F21,
F22 = GLFW_KEY_F22,
F23 = GLFW_KEY_F23,
F24 = GLFW_KEY_F24,
F25 = GLFW_KEY_F25,
KP0 = GLFW_KEY_KP_0,
KP1 = GLFW_KEY_KP_1,
KP2 = GLFW_KEY_KP_2,
KP3 = GLFW_KEY_KP_3,
KP4 = GLFW_KEY_KP_4,
KP5 = GLFW_KEY_KP_5,
KP6 = GLFW_KEY_KP_6,
KP7 = GLFW_KEY_KP_7,
KP8 = GLFW_KEY_KP_8,
KP9 = GLFW_KEY_KP_9,
KPDecimal = GLFW_KEY_KP_DECIMAL,
KPDivide = GLFW_KEY_KP_DIVIDE,
KPMultiply = GLFW_KEY_KP_MULTIPLY,
KPSubstract = GLFW_KEY_KP_SUBTRACT,
KPAdd = GLFW_KEY_KP_ADD,
KPEnter = GLFW_KEY_KP_ENTER,
KPEqual = GLFW_KEY_KP_EQUAL,
LeftShift = GLFW_KEY_LEFT_SHIFT,
LeftControl = GLFW_KEY_LEFT_CONTROL,
LeftAlt = GLFW_KEY_LEFT_ALT,
LeftSuper = GLFW_KEY_LEFT_SUPER,
RightShift = GLFW_KEY_RIGHT_SHIFT,
RightControl = GLFW_KEY_RIGHT_CONTROL,
RightAlt = GLFW_KEY_RIGHT_ALT,
RightSuper = GLFW_KEY_RIGHT_SUPER,
KeyMenu = GLFW_KEY_MENU,

LeftMouse = GLFW_MOUSE_BUTTON_1,
RightMouse = GLFW_MOUSE_BUTTON_2,
MiddleMouse = GLFW_MOUSE_BUTTON_3


};

struct InputState
{
	bool PreviousButtonPressState;
	bool CurrentButtonPressState;
};

struct GLFWwindow;

class Input
{

public:
	~Input();

	static void PollEvents();

	static Input& GetInstance();

	static int GetTrackedKeyCount();

	//static void AddKeysToTrack();
	template<typename T> static void AddKeysToTrack(T key);

	template<typename T, typename ...Args> static void AddKeysToTrack(T key, Args ...args)
	{
		AddKeysToTrack(key);
		AddKeysToTrack(args...);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


	static bool GetKey(KeyCode keyCode);

	static bool GetKeyDown(KeyCode keyCode);

	static bool GetKeyUp(KeyCode keyCode);

	static void LateUpdateTrackedKeyStates();


private:

	Input();

	std::map<int, InputState> inputStates;

	bool findKey(int key, std::map<int, InputState>::iterator& OUT outIter);

	bool findKey(KeyCode keyCode, std::map<int, InputState>::iterator& OUT outIter);

	static void UpdateKey(int key, int action);



	
};




