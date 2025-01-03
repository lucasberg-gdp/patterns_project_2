#pragma once

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <vector>

class cGLComputeEuler
{
public:
	cGLComputeEuler();


	// This will represent an object
	struct sPysicsState
	{
		// We are only interested in vec3s BUT
		//	the compute buffer is happier with vec4s
		// (and out life will be WAY easier)
		glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 velocity = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 acceleration = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	// Will copy the initial data to the internal array (and SSBO)
	bool init(unsigned int shaderID, std::vector< sPysicsState >& vecTheData);
	// This will update - i.e. overwrite - the data
	bool update(unsigned int shaderID, std::vector< sPysicsState >& vecTheData);
	// This will retrieve the data (copy the buffer back from the SSBO)
	bool getData(unsigned int shaderID, std::vector< sPysicsState >& vecTheData);

private:
	sPysicsState* m_pTheData = NULL;
	unsigned int m_BufferSize = 0;
	GLuint m_SSBO_ID = 0;

	// We get the "5" from this "binding = 5" in the shader
	// layout(std430, binding = 5) buffer layoutName
	const unsigned int SSBO_GLSL_BINDING_POINT = 5;
};
