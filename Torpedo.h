#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Object.h"

class Torpedo : public Object
{

private:
	bool active;


public:
	Torpedo();
	Torpedo(GLuint tex, glm::vec3 loc, glm::vec3 rot, glm::vec3 size,
		glm::vec3 vel, glm::vec3 force, float mass, float r, Collider cldt);
	virtual ~Torpedo();

	void update(float deltaTime);
	bool isActive();  // check if the torpedoe is active
	void setActive(bool value);
	void setLocation(glm::vec3 spawnLoc);

};

