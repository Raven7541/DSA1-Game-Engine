// Object.cpp			4/18/2016
//
// Game objects!

#include "Object.h"


// Constructor - default
Object::Object()
{
	textureFile = -1;
	thisTransform.loc = glm::vec3();
	thisTransform.rot = glm::vec3();
	thisTransform.size = glm::vec3();
	body.velocity = glm::vec3();
	body.force = glm::vec3();
	body.mass = 0;
	radius = 0;
	collideType = Colliderless;
	isPlayer = false;
	isEnemy = false;
	isTrigger = false;
	isTreasure = false;
	body.dragForce = glm::vec3();
}

// Constructor - parameterized
Object::Object(GLuint tex, glm::vec3 loc, glm::vec3 rot, glm::vec3 size, 
	glm::vec3 vel, glm::vec3 force, float mass, float r, Collider cldt, 
	bool player, bool enemy)
{
	textureFile = tex;
	thisTransform.loc = loc;
	thisTransform.rot = rot;
	thisTransform.size = size;
	body.velocity = vel;
	body.force = force;
	body.mass = mass;
	radius = r;
	collideType = cldt;
	isPlayer = player;
	isEnemy = enemy;
	body.dragForce = -body.force;

	isTrigger = false;
	isTreasure = false;
}

// Destructor
Object::~Object()
{
	
}


/*
* Methods
*/
// Update!
void Object::updateBody()
{
	body.min = thisTransform.loc - thisTransform.size;
	body.max = thisTransform.loc + thisTransform.size;
}

// Use the force, dammit!
void Object::applyForce(float deltaTime, glm::vec3 dir)
{
	// Reset acceleration, velocity (final), and position (final)
	glm::vec3 acceleration;

	glm::vec3 tempForce = body.force;
	body.force -= body.velocity * 1.f;
	body.force *= dir;

	//body.force -= body.velocity * 1.f;

	// Calculate acceleration
	acceleration = body.force / body.mass;

	// Calculate change in velocity
	// a = dv/dt -> dv = a * dt
	// vf = (a * t) + vi
	body.velocity += acceleration * deltaTime;
	body.force = tempForce;  // reset

	// Calculate change in location
	// v = dr/dt -> dr = v * dt
	// rf = (a * t^2) + (v * t) + ri 
	//newPos += (acceleration * pow(deltaTime, 2)) + (body.velocity * deltaTime) + thisTransform.loc;
	thisTransform.loc += body.velocity * deltaTime;
	
}

// Are you out of bounds?!
bool Object::outOfBounds()
{
	if (thisTransform.loc.x < -3 || thisTransform.loc.x > 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Collision detection
bool Object::collideWith(const Object& obj1)
{
	float distance = 0;  // reset

	// Check if the objects have a collider
	if (collideType == Colliderless || obj1.collideType == Colliderless)
	{
		// No collider!
		return false;
	}
	else if (collideType == Sphere && obj1.collideType == Sphere)
	{
		// Sphere collision
		// Calculate the distance
		distance = pow((thisTransform.loc.z - obj1.thisTransform.loc.z), 2) +
			pow((thisTransform.loc.x - obj1.thisTransform.loc.x), 2) +
			pow((thisTransform.loc.y - obj1.thisTransform.loc.y), 2);

		distance = sqrt(distance);

		// Check if the distance is greater than the radii combined
		if (distance > (radius + obj1.radius))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (collideType == AABB && obj1.collideType == AABB)
	{
		// AABB collision
		if (body.min.x < obj1.body.max.x && 
			body.max.x > obj1.body.min.x && 
			body.min.y < obj1.body.max.y && 
			body.max.y > obj1.body.min.y && 
			body.min.z < obj1.body.max.z && 
			body.max.z > obj1.body.min.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (collideType == Sphere && obj1.collideType == AABB)
	{
		// Sphere and AABB collision
		// x-axis
		if (thisTransform.loc.x < obj1.body.min.x)
		{
			distance += pow((obj1.body.min.x - thisTransform.loc.x), 2);
		}
		else if (thisTransform.loc.x > obj1.body.max.x)
		{
			distance += pow((thisTransform.loc.x - obj1.body.max.x), 2);
		}

		// y-axis
		if (thisTransform.loc.y < obj1.body.min.y)
		{
			distance += pow((obj1.body.min.y - thisTransform.loc.y), 2);
		}
		else if (thisTransform.loc.y > obj1.body.max.y)
		{
			distance += pow((thisTransform.loc.y - obj1.body.max.y), 2);
		}

		// z-axis
		if (thisTransform.loc.z < obj1.body.min.z)
		{
			distance += pow((obj1.body.min.z - thisTransform.loc.z), 2);
		}
		else if (thisTransform.loc.z > obj1.body.max.z)
		{
			distance += pow((thisTransform.loc.z - obj1.body.max.z), 2);
		}

		//std::cout << "Sphere + AABB Distance: " << distance << ", " << pow(thisTransform.size.x, 2) << std::endl;  // debug
		return distance < pow(thisTransform.size.x, 2);
	}
	else if (collideType == AABB && obj1.collideType == Sphere)
	{
		// AABB and Sphere collision (other way around)
		// x-axis
		if (obj1.thisTransform.loc.x < body.min.x)
		{
			distance += pow((body.min.x - obj1.thisTransform.loc.x), 2);
		}
		else if (obj1.thisTransform.loc.x > body.max.x)
		{
			distance += pow((obj1.thisTransform.loc.x - body.max.x), 2);
		}

		// y-axis
		if (obj1.thisTransform.loc.y < body.min.y)
		{
			distance += pow((body.min.y - obj1.thisTransform.loc.y), 2);
		}
		else if (obj1.thisTransform.loc.y > body.max.y)
		{
			distance += pow((obj1.thisTransform.loc.y - body.max.y), 2);
		}

		// z-axis
		if (obj1.thisTransform.loc.z < body.min.z)
		{
			distance += pow((body.min.z - obj1.thisTransform.loc.z), 2);
		}
		else if (obj1.thisTransform.loc.z > body.max.y)
		{
			distance += pow((obj1.thisTransform.loc.z - body.max.z), 2);
		}

		//std::cout << "AABB + Sphere Distance: " << distance << ", " << pow(obj1.thisTransform.size.x, 2) << std::endl;  // debug
		return distance < pow(obj1.thisTransform.size.x, 2);
	}
}

// Rebound!
void Object::rebound(float deltaTime, glm::vec3 dir)
{
	// Reset acceleration, velocity (final), and position (final)
	glm::vec3 acceleration;

	glm::vec3 reboundForce = body.force * glm::vec3(10, 10, 10);  // reset
	//reboundForce -= body.velocity * 1.f;
	reboundForce *= dir;

	//body.force -= body.velocity * 1.f;

	// Calculate acceleration
	acceleration = reboundForce / body.mass;

	// Calculate change in velocity
	// a = dv/dt -> dv = a * dt
	// vf = (a * t) + vi
	body.velocity += acceleration * deltaTime;

	// Calculate change in location
	// v = dr/dt -> dr = v * dt
	// rf = (a * t^2) + (v * t) + ri 
	//newPos += (acceleration * pow(deltaTime, 2)) + (body.velocity * deltaTime) + thisTransform.loc;
	thisTransform.loc += body.velocity * deltaTime;
}


/*
* Accessors
*/
// What do you look like?
GLuint Object::getTexture()
{
	return textureFile;
}

// How big is your radius?
float Object::getRadius()
{
	return radius;
}

// Are you the player?
bool Object::playerStatus()
{
	return isPlayer;
}

// Are you the enemy?
bool Object::enemyStatus()
{
	return isEnemy;
}

// Is this the trigger?
bool Object::triggerStatus()
{
	return isTrigger;
}

// Is this the treasure?
bool Object::treasureStatus()
{
	return isTreasure;
}

// Set up the trigger
void Object::setTrigger(bool status)
{
	isTrigger = status;
}

// Set up the treasure
void Object::setTreasure(bool status)
{
	isTreasure = status;
}

// For debug purposes
glm::vec3 Object::getLocation()
{
	return thisTransform.loc;
}