#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

#include "Object.h"


class Camera
{

private:
	RigidBody body;

	
	float windowWidth;
	float windowHeight;
	float view;
	float zNear, zFar;  // near and far plane distances
	float fovy;			// field of view
	float aspect;       // aspect ratio 

	float const pi = 3.1415926535897;

	glm::mat4 calculateViewMatrix();  // calculate view matrix
	glm::mat4 calculatePersMatrix();  // calculate projection matrix

public:
	transform thisTransform;
	glm::mat4 cameraMatrix;
	float zoom;
	
	Camera();  // Constructor, default
	Camera(glm::vec3 loc, glm::vec3 rot, float zoooom, float width, 
		float height, float newView, float near, float far);  // Constructor, parameterized
	~Camera();  // Destructor

	void update();  // update
	void applyForce(float deltaTime, glm::vec3 vel);  // move!

};

