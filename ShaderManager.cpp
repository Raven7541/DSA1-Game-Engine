// ShaderManager.cpp		3/16/2016
//
// Manages the shaders!

#include "ShaderManager.h"


// Constructor - default
ShaderManager::ShaderManager()
{

}

// Destructor
ShaderManager::~ShaderManager()
{

}

// Load the shader privately
GLuint ShaderManager::loadShader(const char* file, GLenum shaderType)
{
	// Load the file
	std::ifstream inFile(file, std::ios::binary);
	char* fileContents;  // array to hold file
	int length;

	// Handle file I/O
	if (!inFile.is_open())
	{
		std::cout << "Error reading file!" << std::endl;
		return 0;
	}
	else
	{
		// Seek and get the length
		inFile.seekg(0, std::ios::end);
		length = (int)inFile.tellg();

		// Seek back to the beginning
		inFile.seekg(0, std::ios::beg);

		// Make an array to hold the file
		fileContents = new char[length + 1];

		// Read the file and set null pointer
		inFile.read(fileContents, length);
		fileContents[length] = '\0';

		// Done with file
		inFile.close();
	}

	// Create vertex shader
	GLuint shader = glCreateShader(shaderType);

	// Create shader source
	glShaderSource(shader, 1, &fileContents, &length);

	// Compile shader
	glCompileShader(shader);

	// Delete the array
	delete[] fileContents;

	// Determine if the shader has compiled
	try
	{
		GLint shaderTest;  // hold the info log length
		glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderTest);

		std::cout << "Shader loading success!" << std::endl;

		return shader;  // return the shader index
	}
	catch (...)
	{
		std::cout << "Shader didn't compile!" << std::endl;

		// Get the log
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		// Set up the array
		GLchar* log = new GLchar[logLength];

		// Get the log
		glGetShaderInfoLog(shader, logLength, 0, log);

		// Print the results
		for (int i = 0; i < logLength; i++)
		{
			std::cout << log[i];
		}

		// Delete stuff!
		glDeleteShader(shader);
		delete[] log;

		return 0;
	}
}

// Load shaders publicly
bool ShaderManager::loadShaders(const char* vertexFile, const char* fragmentFile)
{
	GLuint vertexShader;
	GLuint fragmentShader;

	// Load the vertex shader
	try
	{
		vertexShader = loadShader(vertexFile, GL_VERTEX_SHADER);
	}
	catch (...)
	{
		std::cout << "Error loading vertex shader!" << std::endl;
		return 0;
	}

	// Load the fragment shader
	try
	{
		fragmentShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	}
	catch (...)
	{
		std::cout << "Error loading fragment shader!" << std::endl;
		return 0;
	}

	// Create shader program
	program = glCreateProgram();

	// Attach the shaders
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link the program
	glLinkProgram(program);

	// Check the program link
	GLint programTest;
	glGetProgramiv(program, GL_LINK_STATUS, &programTest);

	if (programTest == 0)
	{
		std::cout << "Error with program link!" << std::endl;

		// Get the log and...you know the drill
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		// Make the array!
		GLchar* log = new GLchar[logLength];

		// Get the log!
		glGetProgramInfoLog(program, logLength, 0, log);

		// Print results!
		for (int i = 0; i < logLength; i++)
		{
			std::cout << log[i];
		}

		// Delete stuff!
		glDeleteProgram(program);
		delete[] log;

		return false;
	}
	std::cout << "Program link success!" << std::endl;
	
}

// Get the program
GLuint ShaderManager::getProgram() const
{
	return program;
}