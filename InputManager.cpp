// InputManager.cpp			4/2/2016
//
// Handles keyboard and mouse input

#include "InputManager.h"


std::map<int, bool> InputManager::keyDown;

// Constructor
InputManager::InputManager()
{

}

// Destructor
InputManager::~InputManager() {}


// Mouse
void InputManager::mouseClick(GLFWwindow* windowPtr, 
	int key, int action, int mods)
{
	keyDown[key] = action;
}

// Keyboard
void InputManager::keyCallback(GLFWwindow* windowPtr, 
	int key, int scancode, int action, int mods)
{
	keyDown[key] = action;
}

// Scroll
void InputManager::scrollCallback(GLFWwindow* windowPtr,
	double x, double y)
{
	// Something something
}

// Initialize
void InputManager::init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetScrollCallback(window, scrollCallback);
}

// Clear it!
void InputManager::releaseKey(int key)
{
	keyDown[key] = false;
}

// Get a key
int InputManager::getKey(int key)
{
	return keyDown[key];
}