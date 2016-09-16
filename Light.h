#pragma once

#include "Object.h"


class Light
{

public:
	transform thisTransform;

	Light();  // Constructor, default
	Light(glm::vec3 loc, glm::vec3 rot, glm::vec3 size);
	~Light();  // Destructor

	void update();

};

