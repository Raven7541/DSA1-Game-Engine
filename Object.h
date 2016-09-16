#pragma once

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

// Collider
enum Collider
{
	Colliderless, AABB, Sphere
};

/*
* Structures
*/
struct transform
{
	glm::vec3 loc;
	glm::vec3 rot;
	glm::vec3 size;
	glm::mat4 transformMatrix;  // object-world transformation matrix
};

struct RigidBody
{
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
	glm::vec3 dragForce;
	glm::vec3 min;
	glm::vec3 max;
};


class Object
{

protected:
	float radius;
	GLuint textureFile;
	RigidBody body;
	Collider collideType;

private:
	
	bool isEnemy;
	bool isPlayer;
	bool isTrigger;
	bool isTreasure;

public:
	transform thisTransform;

	Object();  // constructor - default
	Object(GLuint tex, glm::vec3 loc, glm::vec3 rot, glm::vec3 size, 
		glm::vec3 vel, glm::vec3 force, float mass, float r, Collider cldt, bool player, bool enemy);  // constructor - parameterized
	virtual ~Object();  // destructor

	void updateBody();
	void applyForce(float deltaTime, glm::vec3 dir);	// apply force
	bool outOfBounds();									// check if it's out of bounds
	bool collideWith(const Object& obj1);				// collision detection
	void rebound(float deltaTime, glm::vec3 dir);

	GLuint getTexture();  // accessors
	float getRadius();
	bool playerStatus();
	bool enemyStatus();
	bool triggerStatus();
	bool treasureStatus();

	glm::vec3 getLocation();  // debug purposes
	void setTrigger(bool status);  // set the trigger!
	void setTreasure(bool status);  // set the treasure!
	
};

