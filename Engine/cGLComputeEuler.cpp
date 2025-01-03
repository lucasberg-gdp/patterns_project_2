#include "cGLComputeEuler.h"

#include <string>
#include <iostream> 



cGLComputeEuler::cGLComputeEuler()
{
}


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
bool cGLComputeEuler::init(unsigned int shaderID, std::vector< sPysicsState >& vecTheData)
{
	gDEBUG_PROCESS_GL_ERROR

	if (vecTheData.empty())
	{
		// No data in the vector...
		return false;
	}

	// See: https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
	//
	if (this->m_pTheData != NULL)
	{
		// delete the old data
		delete[] this->m_pTheData;
	}

	// Allocate the buffer
	this->m_BufferSize = (unsigned int)vecTheData.size();
	this->m_pTheData = new sPysicsState[this->m_BufferSize];
	// Clear it to all zeros...
	memset(this->m_pTheData, 0, sizeof(sPysicsState) * this->m_BufferSize);

	// Copy it over...
	for (unsigned int index = 0; index != this->m_BufferSize; index++)
	{
		this->m_pTheData[index] = vecTheData[index];
	}


	// Make an ID ('name') for this buffer
	glGenBuffers(1, &(this->m_SSBO_ID));
	// Set buffer and make it the active buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_SSBO_ID);
	gDEBUG_PROCESS_GL_ERROR


		// Copy the data to the SSBO
		glBufferData(GL_SHADER_STORAGE_BUFFER,
			sizeof(sPysicsState) * this->m_BufferSize,		// Size in bytes
			(void*)this->m_pTheData,						// Local array
			GL_DYNAMIC_READ);
	// Can be:
	// GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, 
	// GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, 
	// GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY

//	::g_ProcessOpenGLError(__FILE__, __LINE__);
	gDEBUG_PROCESS_GL_ERROR

		// Connect this to the SSBO in the shader
		// 
		// layout(std430, binding = 5) buffer physicInfo

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, this->m_SSBO_ID);
	gDEBUG_PROCESS_GL_ERROR


		// Unbind (so nothing is connected)
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind


	return true;
}

// This will update - i.e. overwrite - the data
bool cGLComputeEuler::update(unsigned int shaderID, std::vector< sPysicsState >& vecTheData)
{
	//::g_ProcessOpenGLError(__FILE__, __LINE__);
	gDEBUG_PROCESS_GL_ERROR

		if (vecTheData.empty())
		{
			// No data in the vector...
			return false;
		}

	if (this->m_pTheData != NULL)
	{
		return this->init(shaderID, vecTheData);
	}

	// At this point, there IS a buffer and there IS an array

	// Set buffer and make it the active buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_SSBO_ID);
	gDEBUG_PROCESS_GL_ERROR


		// Copy the data to the SSBO
		// not: glBufferData()
		// 
		glBufferSubData(GL_SHADER_STORAGE_BUFFER,
			0,												// offset into buffer (if needed)
			sizeof(sPysicsState) * this->m_BufferSize,		// Size in bytes
			(void*)this->m_pTheData);						// Local array
	gDEBUG_PROCESS_GL_ERROR


		// Unbind (so nothing is connected)
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind


	return true;
}

// This will retrieve the data (copy the buffer back from the SSBO)
bool cGLComputeEuler::getData(unsigned int shaderID, std::vector< sPysicsState >& vecTheData)
{
	if (this->m_pTheData == NULL)
	{
		// No data to get...
		return false;
	}

	gDEBUG_PROCESS_GL_ERROR



		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetBufferSubData.xhtml

		// connect to the buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_SSBO_ID);
	gDEBUG_PROCESS_GL_ERROR



		// Clear the local array
		memset(this->m_pTheData, 0, sizeof(sPysicsState) * this->m_BufferSize);

	// Copy the data from the SSBO
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetBufferSubData.xhtml
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER,		// GLenum target,
		0,								// GLintptr offset in bytes from start
		sizeof(sPysicsState) * this->m_BufferSize,		// Size in bytes
		(void*)this->m_pTheData);		// void pointer to the data to copy back
	gDEBUG_PROCESS_GL_ERROR



		// Unbind (so nothing is connected)
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

	vecTheData.clear();
	vecTheData.reserve(this->m_BufferSize);

	// Copy the data back to the vector
	for (unsigned int index = 0; index != this->m_BufferSize; index++)
	{
		vecTheData.push_back(this->m_pTheData[index]);
	}

	return true;
}