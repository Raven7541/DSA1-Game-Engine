// Model.cpp			5/2/2016
//
// Where the models and model loading happen!



#include "Model.h"

// Constructor, default
Model::Model()
{
	vertArr = 0;
	vertCount = 0;
}

// Destructor
Model::~Model()
{

}


/*
* Methods
*/
// Set up the buffered model
bool Model::bufferModel(std::string objFile)
{
	// Set up vertex, UV, normal, and vertInds data
	std::vector<glm::vec3> locs, norms;
	std::vector<glm::vec2> uvs;
	std::vector<VertInd> vertInds;
	
	try
	{
		// Open the file
		std::ifstream inFile(objFile);
		std::string line;

		if (inFile.is_open())
		{
			// Read a single line at a time
			while (std::getline(inFile, line))
			{
				std::istringstream input(line);
				std::string label;
				input >> label;
				if (label == "v")
				{
					// Location
					float x, y, z;

					input >> x >> y >> z;

					locs.push_back(glm::vec3(x, y, z));
				}
				else if (label == "vt")
				{
					// UV
					float x, y;

					input >> x >> y;

					uvs.push_back(glm::vec2(x, y));
				}
				else if (label == "vn")
				{
					// Normals
					float x, y, z;

					input >> x >> y >> z;

					norms.push_back(glm::vec3(x, y, z));
				}
				else if (label == "f")
				{
					// Faces
					unsigned int f1, f2, f3;
					char slash;

					for (int i = 0; i < 3; i++)
					{
						input >> f1 >> slash >> f2 >> slash >> f3;

						// Decrement each int
						f1 -= 1;
						f2 -= 1;
						f3 -= 1;

						// Put the three indices into a VertInd
						VertInd newVertInd;
						newVertInd.locInd = f1;
						newVertInd.uvInd = f2;
						newVertInd.normInd = f3;
						vertInds.push_back(newVertInd);
					}
				}
			}

			// Close the file
			inFile.close();
		}
	}
	catch (...)
	{
		std::cout << "Error reading file!" << std::endl;
		return false;
	}

	// Process model data
	// Duplicate vertices into a single buffer
	vertCount = vertInds.size();
	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = { locs[vertInds[i].locInd], 
						   uvs[vertInds[i].uvInd],
						   norms[vertInds[i].normInd] };
	}

	// Generate the buffers
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	// Bind the buffers
	// Controls which array and buffer are active
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	// Store data in the buffer
	glBufferData(GL_ARRAY_BUFFER,		// where to copy to
		sizeof(Vertex) * vertCount,  // # bytes to copy
		&vertBufData[0],				// where to copy from
		GL_STATIC_DRAW);				// "hint" to OpenGL

	// Describe the buffer layout to the vertex attribute
	// Enable the attribute
	glEnableVertexAttribArray(0);

	// Set up the attribute for loc data
	glVertexAttribPointer(
		0,					// attribute index
		3,					// number of components (x, y, z)
		GL_FLOAT,			// type of data
		GL_FALSE,			// should we normalize the data?
		sizeof(Vertex),  // stride (bytes per vertex)
		0);					// offset to this attribute

	// Set up the attribute for UVs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// attribute index
		2,					// number of components (x, y)
		GL_FLOAT,			// type of data
		GL_FALSE,			// should we normalize the data?
		sizeof(Vertex),  // stride (bytes per vertex)
		(void*)sizeof(glm::vec3));	// offset to this attribute

	// Set up the attribute for normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,					// attribute index
		3,					// number of components (x, y, z)
		GL_FLOAT,			// type of data
		GL_FALSE,			// should we normalize the data?
		sizeof(Vertex),  // stride (bytes per vertex)
		(void*)sizeof(glm::vec3));	// offset to this attribute


	// Set the window color (rgba)
	glClearColor(0.392f, 0.548f, 0.929f, 1.0f);

	std::cout << "Buffer model loaded!" << std::endl;  // debug

	return true;
}

// Render the model
void Model::render(GLuint texFile)
{
	glBindVertexArray(vertArr);
	glBindTexture(GL_TEXTURE_2D, texFile);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);

	glBindVertexArray(0);
}