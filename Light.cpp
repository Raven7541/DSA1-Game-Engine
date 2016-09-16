// Light.cpp			5/13/2016
//
// The lights! Kill the lights~!

#include "Light.h"


// Constructor - default
Light::Light()
{

}

// Constructor - parameterized
Light::Light(glm::vec3 loc, glm::vec3 rot, glm::vec3 size)
{
	thisTransform.loc = loc;
	thisTransform.rot = rot;
	thisTransform.size = size;
}

// Destructor
Light::~Light()
{

}


// Update!
void Light::update()
{
	glUniformMatrix4fv(5, 1, GL_FALSE, &thisTransform.loc[0]);
}