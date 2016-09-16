// Camera.cpp			4/27/2016
//
// The camera!

#include "Camera.h"


// Constructor, default
Camera::Camera()
{
	thisTransform.loc = glm::vec3(0, 0, 2);
	thisTransform.rot = glm::vec3();
	thisTransform.size = glm::vec3(1, 1, 1);
	body.velocity = glm::vec3();
	body.force = glm::vec3(1, 1, 1);
	body.mass = 1;
}

// Constructor, parameterized
Camera::Camera(glm::vec3 loc, glm::vec3 rot, float zoooom, float width, 
	float height, float newView, float near, float far)
{
	thisTransform.loc = loc;
	thisTransform.rot = rot;
	zoom = zoooom;
	windowWidth = width;
	windowHeight = height;
	view = newView;
	zNear = near;
	zFar = far;

	// Set up default values
	thisTransform.size = glm::vec3(1, 1, 1);
	body.velocity = glm::vec3();
	body.force = glm::vec3(1, 1, 1);
	body.mass = 1;
}

// Destructor
Camera::~Camera()
{

}


/*
* Public methods
*/
// Update
void Camera::update()
{
	// Look-at (view) matrix
	glm::mat4 viewMatrix = calculateViewMatrix();

	// Projection matrix
	glm::mat4 projMatrix = calculatePersMatrix();

	// Update the camera's matrix
	cameraMatrix = projMatrix * viewMatrix;

	// Upload the camera matrix and location
	glUniformMatrix4fv(4, 1, GL_FALSE, &cameraMatrix[0][0]);
	glUniformMatrix4fv(6, 1, GL_FALSE, &thisTransform.loc[0]);
}

// Move!
void Camera::applyForce(float deltaTime, glm::vec3 vel)
{
	// Reset acceleration, velocity (final), and position (final)
	glm::vec3 acceleration;

	glm::vec3 tempForce = body.force;
	//body.force -= body.velocity * 1.f;

	// Calculate acceleration
	acceleration = body.force / body.mass;

	// Calculate change in velocity
	// a = dv/dt -> dv = a * dt
	// vf = (a * t) + vi
	body.velocity = (acceleration * deltaTime) + vel;
	body.force = tempForce;  // reset

	// Calculate change in location
	// v = dr/dt -> dr = v * dt
	// rf = (a * t^2) + (v * t) + ri 
	 //newPos += (acceleration * pow(deltaTime, 2)) + (body.velocity * deltaTime) + thisTransform.loc;
	thisTransform.loc += body.velocity * deltaTime;
}


/*
* Private methods
*/
// Calculate the view matrix
glm::mat4 Camera::calculateViewMatrix()
{
	// Calculate the rotation matrix
	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(thisTransform.rot.y,
		thisTransform.rot.x, thisTransform.rot.z);

	glm::vec3 eye = thisTransform.loc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);

	return glm::lookAt(eye, center, up);
}

// Calculate the projection matrix
glm::mat4 Camera::calculatePersMatrix()
{
	float fovy = pi * view / zoom;
	float aspect = (float)windowWidth / (float)windowHeight;

	return glm::perspective(fovy, aspect, zNear, zFar);
}