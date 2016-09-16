#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

/*
* Structures
*/
struct Vertex
{
	glm::vec3 loc;
	glm::vec2 uv;
	glm::vec3 norm;
};

struct VertInd
{
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};


class Model
{

private:
	GLuint vertArr;				 // vertex array 
	GLuint vertBuf;				 // vertex buffer
	unsigned int vertCount;		 // vert count

public:
	Model();  // Constructor - default
	~Model();  // Destructor

	bool bufferModel(std::string objFile);  // Sets up the buffered model(s)
	void render(GLuint texFile);			// Render the model

};

