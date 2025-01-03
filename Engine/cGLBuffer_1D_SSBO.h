#pragma once

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <string>

// This example uses a single 1D vec4 array + SSBO 
// Shader is "compute_01_SSBO_1D.glsl"

class cGLBuffer_1D_SSBO
{
public:
	bool init(unsigned int shaderID, std::vector< glm::vec4 >& vecTheData);
	//	bool updateValues(std::vector< glm::vec4 >& vecTheData);

	bool run(void);

	bool getData(std::vector< glm::vec4 >& vecTheData);

	GLint getBufferID(void)
	{
		return this->m_BufferID;
	}

private:
	GLuint m_BufferID = 0;

	// Where the data is:
	std::vector< glm::vec4 > m_vecInputToGPU;
	std::vector< glm::vec4 > m_vecOutputFromGPU;
	unsigned int m_BufferSize = 0;
};