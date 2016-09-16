// Engine.cpp			3/16/2016
//
// The program's engine!
// Version 1.0 - Kraken

#include "Engine.h"



// Constructor - default
Engine::Engine()
{

}

// Destructor - delete everything!
Engine::~Engine()
{
	// Delete the textures
	//glDeleteTextures(5, texture);
	delete camera;
	delete light;
}


/*
* Setup
*/
void Engine::setup()
{
	// Set up the game loop if all conditions are met
	if (init() && 
		bufferModels("models/quad.obj") && 
		loadShaders() && 
		loadObjects())
	{
		gameLoop();
	}
}

// Try and initialize the engine
bool Engine::init()
{
	// Initialize the window library
	// glfwInit() initializes GLFW
	// returns GL_TRUE, which is  #define'd as 1 if it works; GL_FALSE as 0 if fails
	if (glfwInit() == GL_FALSE)
	{
		return false;
	}

	// Create GLFW window
	GLFWwindowPtr = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ellen DSA1 Kraken Engine", NULL, NULL);

	// Make it the currently active window, or quit
	if (GLFWwindowPtr != nullptr)
	{
		glfwMakeContextCurrent(GLFWwindowPtr);
	}
	else
	{
		glfwTerminate();
		return false;
	}

	// Initialize GLEW or quit
	// glewInit() returns GLEW_OK, which is #define'd as 0
	// Returns something else if fails
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}

	// Set up InputManager
	InputManager::init(GLFWwindowPtr);

	// Set up the camera
	camera = new Camera(glm::vec3(0, 0, 5), glm::vec3(), 1.f, 800, 600, 0.4f, 0.01f, 1000.0f);

	// Set up the lights
	light = new Light(glm::vec3(-1, -1, -1), glm::vec3(0, -20, 0), glm::vec3(1, 1, 1));

	// Hide the cursor
	glfwSetInputMode(GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Enable transparency
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	std::cout << "Engine initialized" << std::endl;  // debug

	return true;
}

// Set up the buffered models
bool Engine::bufferModels(std::string fileName)
{
	try
	{
		model.bufferModel(fileName);

		// Engage wire-frame rendering
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		return true;
	}
	catch (...)
	{
		std::cout << "Buffered models not loaded!" << std::endl;
		return false;
	}
}

// Use the shaders
bool Engine::loadShaders()
{
	if (shader.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl"))
	{
		// Run the program
		glUseProgram(shader.getProgram());
		return true;
	}
	else
	{
		return false;
	}
}

// Set up the textures
GLuint Engine::loadTextures(const char* fileName)
{	
	// Load the texture
	FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(fileName, 0), fileName);
	if (image == nullptr) return false;

	// Convert texture to 32bit bmp
	FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image);

	texID = 0;

	// Unload the image from memory
	FreeImage_Unload(image);

	// Bind the texture
	//glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Upload texture bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit), FreeImage_GetHeight(image32Bit),
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32Bit));

	// Set min filter instead of mipmap linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Clear the texture from RAM
	// and unbind the texture
	FreeImage_Unload(image32Bit);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE0);  // reset active texture to 0

	return texID;
}

// Set up the objects
bool Engine::loadObjects()
{
	// Background
	Object background = Object(loadTextures("textures/background.jpg"), glm::vec3(-0.75, 0, 0), glm::vec3(),
		glm::vec3(1.5, 1.3, 2), glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), 3, 0, Colliderless, false, false);
	Object ground = Object(loadTextures("textures/ground.png"), glm::vec3(-0.75, -0.5, 0), glm::vec3(),
		glm::vec3(1.51, 1, 2), glm::vec3(), glm::vec3(), 0, 5, Colliderless, false, false);
	Object rock = Object(loadTextures("textures/rock.png"), glm::vec3(-0.75, -0.75, 0), glm::vec3(),
		glm::vec3(0.5, 0.5, 1), glm::vec3(), glm::vec3(), 5, 0.3, AABB, false, false);

	// Characters/monsters
	Object playerAvatar = Object(loadTextures("textures/Lynn.png"), glm::vec3(-2, -0.5, 0), glm::vec3(),
		glm::vec3(0.2, 0.2, 0.2), glm::vec3(), glm::vec3(1, 1, 0), 3, 0.2, Sphere, true, false);
	Object shark = Object(loadTextures("textures/shark.png"), glm::vec3(0.5, 0, 0), glm::vec3(),
		glm::vec3(0.2, 0.2, 0.2), glm::vec3(), glm::vec3(5, 5, 0), 5, 0.2, Sphere, false, true);
	Object shark2 = Object(loadTextures("textures/shark.png"), glm::vec3(0.1, 1, 0), glm::vec3(),
		glm::vec3(0.2, 0.2, 0.2), glm::vec3(), glm::vec3(5, 5, 0), 5, 0.2, Sphere, false, true);
	Object shark3 = Object(loadTextures("textures/shark.png"), glm::vec3(0.25, 0.5, 0), glm::vec3(),
		glm::vec3(0.2, 0.2, 0.2), glm::vec3(), glm::vec3(5, 5, 0), 5, 0.2, Sphere, false, true);

	// Objects
	Object trigger = Object(loadTextures("textures/trigger.png"), glm::vec3(-1.5, 0, 0), glm::vec3(), 
		glm::vec3(0.7, 0.1, 0.1), glm::vec3(), glm::vec3(), 1, 0.1, AABB, false, false);
	trigger.setTrigger(true);
	Object treasure = Object(loadTextures("textures/treasure.png"), glm::vec3(0.5, -1, 0), glm::vec3(),
		glm::vec3(0.1, 0.1, 0.1), glm::vec3(), glm::vec3(), 1, 0.1, Sphere, false, false);
	treasure.setTreasure(true);

	// Add to assets
	assets.push_back(background);
	assets.push_back(ground);
	assets.push_back(rock);
	assets.push_back(playerAvatar);
	assets.push_back(shark);
	assets.push_back(shark2);
	assets.push_back(shark3);
	assets.push_back(trigger);
	assets.push_back(treasure);

	// Set up the torpedoes
	for (int i = 0; i < 100; i++)
	{
		torpedoes.insert(
			std::make_pair(i, Torpedo(loadTextures("textures/torpedo_shot.png"), glm::vec3(), glm::vec3(),
				glm::vec3(0.02, 0.02, 0.02), glm::vec3(), glm::vec3(1.5, 1.5, 0), 3, 0.02, Sphere))
			);
	}

	std::cout << "Objects loaded!" << std::endl;  // debug

	return true;
}

// Set up the game loop
bool Engine::gameLoop()
{
	currentTime = glfwGetTime();
	glBindVertexArray(0);

	std::cout << "Game running!" << std::endl;

	// Loop until the user closes the window
	// glfwWindowShouldClose(GLFWwindowPtr) returns true if window was told to close during last frame
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		
		// Process input/window events
		// also process queued window, mouse & keyboard callback events
		glfwPollEvents();
		
		// Update physical simulation
		// Draw buffered models

		// Clear the canvas
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// Set up objects
		for (int i = 0; i < assets.size(); i++)
		{
			for (int j = 0; j < assets.size(); j++)
			{
				assets[j].updateBody();
			}
			
			// Draw them out!
			renderTexture(assets[i]);

			// Movement (restricted to player avatar)
			if (assets[i].playerStatus())
			{
				// Check for collisions
				for (int j = 1; j < assets.size(); j++)
				{
					if (assets[j].enemyStatus() && assets[i].collideWith(assets[j]))
					{
						// You hit an enemy!
						std::cout << "Game over!" << std::endl;  // debug
						//isTriggered = false;  //  STAHP MOVING
						destroy(i);			  // Remove player
					}
					else if (!assets[j].enemyStatus() && !assets[j].playerStatus()
						&& assets[j].triggerStatus() && assets[i].collideWith(assets[j]))
					{
						// Enemy triggered!
						if (!isTriggered)
						{
							std::cout << "Shark deployed!" << std::endl;  // debug
						}
						isTriggered = true;
					}
					else if (!assets[j].enemyStatus() && !assets[j].playerStatus()
						&& !assets[j].triggerStatus()
						&& assets[j].treasureStatus() && assets[i].collideWith(assets[j]))
					{
						// Treasure!
						std::cout << "Congrats! You won!" << std::endl;
						destroy(j);
					}
					else if (!assets[j].enemyStatus() && !assets[j].playerStatus() 
						&& !assets[j].triggerStatus() && !assets[j].treasureStatus()
						&& assets[i].collideWith(assets[j]))
					{
						// You hit an obstacle!
						std::cout << "Bonk! You hit the sea floor!" << std::endl;  // debug
						assets[i].rebound(deltaTime, glm::vec3(0, 1, 0));
					}
				}
				
				// Up
				if (InputManager::getKey(GLFW_KEY_W))
				{
					// Go up
					assets[i].applyForce(deltaTime, glm::vec3(0, 1, 0));
				}
				else
				{
					assets[i].applyForce(deltaTime, glm::vec3());
				}

				// Left
				if (InputManager::getKey(GLFW_KEY_A))
				{
					// Go left
					assets[i].applyForce(deltaTime, glm::vec3(-1, 0, 0));
				}
				else
				{
					assets[i].applyForce(deltaTime, glm::vec3());
				}

				// Down
				if (InputManager::getKey(GLFW_KEY_S))
				{
					// Go down
					assets[i].applyForce(deltaTime, glm::vec3(0, -1, 0));
				}
				else
				{
					assets[i].applyForce(deltaTime, glm::vec3());
				}

				// Right
				if (InputManager::getKey(GLFW_KEY_D))
				{
					// Go right
					assets[i].applyForce(deltaTime, glm::vec3(1, 0, 0));
				}
				else
				{
					assets[i].applyForce(deltaTime, glm::vec3());
				}

				// Shoot!
				if (InputManager::getKey(GLFW_KEY_SPACE))
				{
					// Shoot shoot shoot!
					if (!torpedoSpawn)
					{
						std::cout << "Preparing torpedo!" << std::endl;
						prepareTorpedo(assets[i].thisTransform.loc);
						std::cout << "Fire!" << std::endl;  // debug
					}
					else
					{
						std::cout << "Torpedo still active!" << std::endl;  // debug
					}
				}
			}

			// Movement (restricted to enemies)
			if (assets[i].enemyStatus())
			{
				// Trigger the attack (temporarily)!
				if (isTriggered)
				{
					// Check for collisions
					for (int j = 1; j < assets.size(); j++)
					{
						if (assets[j].playerStatus() && assets[i].collideWith(assets[j]))
						{
							// Enemy hit player!
							std::cout << "Shark killed diver! Game over!" << std::endl;  // debug
							destroy(j);			  // Remove Player
							isTriggered = false;  // STAHP MOVING
						}
						else if (!assets[j].playerStatus() && !assets[j].enemyStatus() && !assets[j].triggerStatus()
							&& assets[i].collideWith(assets[j]))
						{
							std::cout << "Bonk! You hit the sea floor!" << std::endl;  // debug
							assets[i].rebound(deltaTime, glm::vec3(0, 1, 0));
						}
					}
					
					// Apply force
					assets[i].applyForce(deltaTime, glm::vec3(-1, 0, 0));

					// Check if it's out of bounds
					if (assets[i].outOfBounds())
					{
						std::cout << "Shark off-screen!" << std::endl;  // debug
						destroy(i);			  // Remove shark
						//isTriggered = false;  // STAHP MOVING!
					}
				}
			}
		}

		// Torpedo action
		if (torpedoSpawn)
		{
			// Find an active torpedo
			int index = 0;
			for (int i = 0; i < torpedoes.size(); i++)
			{
				if (torpedoes[i].isActive())
				{
					index = i;
					break;
				}
			}

			// Update!
			torpedoes[index].updateBody();
			renderTexture(torpedoes[index]);
			torpedoes[index].update(deltaTime);

			// Check for collisions
			for (int i = 0; i < assets.size(); i++)
			{
				if (torpedoes[index].collideWith(assets[i]) && !assets[i].playerStatus())
				{
					if (torpedoes[index].collideWith(assets[i]) && assets[i].triggerStatus())
					{
						continue;
					}
					
					torpedoes[index].setActive(false);

					// What did you hit?
					if (assets[i].enemyStatus())
					{
						// You hit an enemy!
						std::cout << "Enemy hit!" << std::endl;  // debug
						destroy(i);			  // Destroy shark
						//isTriggered = false;  //  STAHP MOVING
					}

					break;
				}
			}

			// Check if it's not active
			if (!torpedoes[index].isActive())
			{
				torpedoSpawn = false;

				std::cout << "Torpedo destroyed!" << std::endl;  // debug
			}
		}

		// Keyboard and mouse events
		// Quit the game!
		if (InputManager::getKey(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
		}

		// Swap the front (what the screen displays)
		// and back (what OpenGL draws to) buffers
		glfwSwapBuffers(GLFWwindowPtr);
	}

	glfwTerminate();  // called before engine closes
					  // or memory leak or worse
	return true;
}

// Prepare the torpedo!
void Engine::prepareTorpedo(glm::vec3 spawnLoc)
{
	std::cout << "Preparing torpedo!" << std::endl;  // debug

	// Find an inactive torpedo
	for (int i = 0; i < torpedoes.size(); i++)
	{
		if (!torpedoes[i].isActive())
		{
			// Set it up
			torpedoes[i].setLocation(spawnLoc);
			torpedoes[i].setActive(true);
			break;
		}
	}

	torpedoSpawn = true;
	std::cout << "Torpedo ready!" << std::endl;  // debug
}

// Render texture
void Engine::renderTexture(Object& obj)
{
	// Calculate the transformation matrix
	// translate * rot * scale
	obj.thisTransform.transformMatrix = glm::translate(obj.thisTransform.loc) *
		glm::yawPitchRoll(obj.thisTransform.rot.y, obj.thisTransform.rot.x, obj.thisTransform.rot.z) *
		glm::scale(obj.thisTransform.size);

	// Update camera
	updateFPS();
	camera->update();

	// Upload the transformation matrix
	glUniformMatrix4fv(3, 1, GL_FALSE, &obj.thisTransform.transformMatrix[0][0]);

	// Bind its vertex array and texture
	model.render(obj.getTexture());
}

// Destroy the object
void Engine::destroy(int index)
{
	assets.erase(assets.begin() + index);
}

// Update the camera
// with FPS controls!
void Engine::updateFPS()
{
	// Turn with mouse
	float sens = 0.005;
	double x, y;
	glfwGetCursorPos(GLFWwindowPtr, &x, &y);  // get mouse position

	// Update the camera rotation
	camera->thisTransform.rot.y -= sens * (x - WINDOW_WIDTH * 0.5f);  // yaw
	camera->thisTransform.rot.x -= sens * (y - WINDOW_HEIGHT * 0.5f);  // pitch
	camera->thisTransform.rot.x =
		glm::clamp(camera->thisTransform.rot.x, -0.5f * pi, 0.5f * pi);

	// Update the mouse
	glfwSetCursorPos(GLFWwindowPtr, WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f);

	// Move with arrows
	glm::vec3 camVel;
	glm::mat3 camRot = (glm::mat3)glm::yawPitchRoll(camera->thisTransform.rot.y, 
		camera->thisTransform.rot.x, camera->thisTransform.rot.z);

	// Movement
	if (InputManager::getKey(GLFW_KEY_UP))
	{
		// Go up
		camVel += camRot * glm::vec3(0, 0, -1);
	}

	if (InputManager::getKey(GLFW_KEY_LEFT))
	{
		// Go left
		camVel += camRot * glm::vec3(-1, 0, 0);
	}

	if (InputManager::getKey(GLFW_KEY_DOWN))
	{
		// Go down
		camVel += camRot * glm::vec3(0, 0, 1);
	}

	if (InputManager::getKey(GLFW_KEY_RIGHT))
	{
		// Go right
		camVel += camRot * glm::vec3(1, 0, 0);
	}

	float speed = 1.0f;
	if (camVel != glm::vec3())
	{
		camVel = glm::normalize(camVel) * speed;
		camera->applyForce(deltaTime, camVel);
	}
}