// Torpedo.cpp			4/20/2016
//
// A torpedo!

#include "Torpedo.h"


// Constructor, default
Torpedo::Torpedo()
{

}

// Constructor, parameterized
Torpedo::Torpedo(GLuint tex, glm::vec3 loc, glm::vec3 rot, glm::vec3 size,
	glm::vec3 vel, glm::vec3 force, float mass, float r, Collider cldt)
	: Object(tex, loc, rot, size, vel, force, mass, r, cldt, false, false)
{
	active = false;
}

// Destructor
Torpedo::~Torpedo()
{
	
}


/*
* Methods
*/
// Update!
void Torpedo::update(float deltaTime)
{
	if (active)
	{
		applyForce(deltaTime, glm::vec3(1, 0, 0));
		
		// Check if it's gone out of bounds
		if (outOfBounds())
		{
			active = false;
		}
	}
}


/*
* Accessors and mutators
*/
// Set the location
void Torpedo::setLocation(glm::vec3 spawnLoc)
{
	thisTransform.loc = glm::vec3(spawnLoc.x+0.2, spawnLoc.y-0.04, spawnLoc.z);
}

// Check if the torpedo is active
bool Torpedo::isActive()
{
	return active;
}

void Torpedo::setActive(bool value)
{
	active = value;
}
