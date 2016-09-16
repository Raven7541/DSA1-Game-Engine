#pragma once

#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

#include "Camera.h"

class InputManager
{

public:
	InputManager();
	virtual ~InputManager();

	static std::map<int, bool> keyDown;

	static void init(GLFWwindow* window);
	static void mouseClick(GLFWwindow* windowPtr, int key, int action, int mods);  // mouse event
	static void keyCallback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods);  // keyboard events
	static void scrollCallback(GLFWwindow* windowPtr, double x, double y);  // scroll events
	static void releaseKey(int key);
	static int getKey(int key);

};

