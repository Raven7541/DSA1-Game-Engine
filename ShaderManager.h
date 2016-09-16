#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>


class ShaderManager
{

private:
	GLuint program;  // index where loaded shaders are stored on graphics card

	GLuint loadShader(const char* file, GLenum shaderType);  // read and compile a shader file


public:
	ShaderManager();  // Constructor, default (initialize program to 0)
	virtual ~ShaderManager();  // Destructor

	GLuint getProgram() const;  // gets the program
	bool loadShaders(const char* vertexFile, const char* fragmentFile);  // load shaders

};

