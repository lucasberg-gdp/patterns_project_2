#include "cGLBuffer_1D_SSBO.h"

#include <iostream>

bool cGLBuffer_1D_SSBO::init(unsigned int shaderID, std::vector< glm::vec4 >& vecTheData)
{

	if (vecTheData.empty())
	{
		return false;
	}

	this->m_BufferSize = (unsigned int)vecTheData.size();


	this->m_BufferID = 0;
	glGenBuffers(1, &(this->m_BufferID));

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_BufferID);
	glBufferData(GL_SHADER_STORAGE_BUFFER,					// Buffer type
		this->m_BufferSize * sizeof(glm::vec4),	// Size in bytes
		NULL,										// We'll copy the data soon
		GL_DYNAMIC_DRAW);
	// GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, 
	// GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, 
	// GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.

	gDEBUG_PROCESS_GL_ERROR

		// Map to the buffer...
		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glMapBufferRange.xhtml
		glm::vec4* pBufferGPU =
		(glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER,
			0,			// Offset
			this->m_BufferSize * sizeof(glm::vec4),	// Size in bytes
			// Will be writing & will overwrite entire buffer
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	gDEBUG_PROCESS_GL_ERROR

		// Copy data over
		for (unsigned int index = 0; index != this->m_BufferSize; index++)
		{
			pBufferGPU[index] = vecTheData[index];
		}

	GLboolean result = glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	switch (result)
	{
	case GL_TRUE:
		std::cout << "Buffer unmapped OK." << std::endl;
		break;
	case GL_FALSE:
		std::cout << "ERROR: Couldn't unmap buffer." << std::endl;
		break;
	default:
		std::cout << "ERROR: Unexpected return from glUnmapBuffer()." << std::endl;
	}



	return true;
}

//bool cGLBuffer_1D_SSBO::updateValues(std::vector< glm::vec4 >& vecTheData)
//{
//
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_BufferID);
//	glBufferData(GL_SHADER_STORAGE_BUFFER,					// Buffer type
//				 this->m_BufferSize * sizeof(glm::vec4),	// Size in bytes
//				 NULL,										// We'll copy the data soon
//				 GL_STATIC_DRAW);
//
//	// Map to the buffer...
//	glm::vec4* pBufferGPU =
//		(glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER,
//									 0,			// Offset
//									 this->m_BufferSize * sizeof(glm::vec4),	// Size in bytes
//									 // Will be writing & will overwrite entire buffer
//									 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
//
//	// Copy data over
//	for (unsigned int index = 0; index != this->m_BufferSize; index++)
//	{
//		pBufferGPU[index] = vecTheData[index];
//	}
//
//	if ( ! glUnmapBuffer(GL_SHADER_STORAGE_BUFFER) )
//	{
//		std::cout << "ERROR: Couldn't unmap buffer." << std::endl;
//	}
////	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_InputBufferID);
//
//	return true;
//}

bool cGLBuffer_1D_SSBO::run(void)
{
	//https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml
	//	layout ( std140, binding = 4 ) buffer Pos
	//	{
	//		vec4 Positions[];
	//	};
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, this->m_BufferID);


	//	glDispatchCompute(128, 1, 1);
	glDispatchCompute(this->m_BufferSize, 1, 1);
	gDEBUG_PROCESS_GL_ERROR

		// Wait
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	gDEBUG_PROCESS_GL_ERROR


		return true;
}

bool cGLBuffer_1D_SSBO::getData(std::vector< glm::vec4 >& vecTheData)
{
	vecTheData.clear();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_BufferID);
	//glBufferData(GL_SHADER_STORAGE_BUFFER,					// Buffer type
	//			 this->m_BufferSize * sizeof(glm::vec4),	// Size in bytes
	//			 NULL,										// We'll copy the data soon
	//			 GL_STATIC_DRAW);
	gDEBUG_PROCESS_GL_ERROR

		// Map to the buffer...
		glm::vec4* pBufferGPU =
		(glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER,
			0,			// Offset
			this->m_BufferSize * sizeof(glm::vec4),	// Size in bytes
			// Will be writing & will overwrite entire buffer
			GL_MAP_READ_BIT);
	gDEBUG_PROCESS_GL_ERROR

		// Copy data over
		for (unsigned int index = 0; index != this->m_BufferSize; index++)
		{
			vecTheData.push_back(pBufferGPU[index]);
		}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


	return true;
}

//bool cGLBuffer_1D::updateValue(unsigned int index, glm::vec4 newValue)
//{
//	this->m_theInputData[index] = newValue;
//	return true;
//}


