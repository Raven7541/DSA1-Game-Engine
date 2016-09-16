#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

#include "ShaderManager.h"
#include "InputManager.h"
#include "Object.h"
#include "Torpedo.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"


/*
* Engine Class
*/
class Engine
{

private:
	GLFWwindow* GLFWwindowPtr;
	ShaderManager shader;		 // shader!
	unsigned int texID;          // texture ID
	std::vector<Object> assets;  // store objects
	std::map<int, Torpedo> torpedoes;

	Model model;

	Camera* camera;
	Light* light;

	int const WINDOW_WIDTH = 800;
	int const WINDOW_HEIGHT = 600;
	float const pi = 3.1415926535897;  // nom nom nom
	
	float currentTime = 0;
	float previousTime = 0;
	float deltaTime = 0;

	float moveObject = 0.01f;
	bool isTriggered = false;
	bool torpedoSpawn = false;

	bool gameOver = false;


public:
	Engine();  // Constructor, default
	virtual ~Engine();  // Destructor

	void setup();  // Set up the engine

	bool init();							    // Initialize the engine
	bool bufferModels(std::string fileName);	// Sets up the bufferered model(s)
	bool loadShaders();							// Use the shaders
	GLuint loadTextures(const char* fileName);  // Sets up the textures
	bool loadObjects();							// Sets up the objects
	bool gameLoop();							// Sets up the game loop

	void prepareTorpedo(glm::vec3 spawnLoc);
	void renderTexture(Object& obj);
	void destroy(int index);
	void updateFPS();

};