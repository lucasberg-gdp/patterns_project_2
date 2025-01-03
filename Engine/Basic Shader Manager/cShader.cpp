#include "cShaderManager.h"

#include "../OpenGLCommon.h"

cShaderManager::cShader::cShader()
{
	this->ID = 0;
	this->shaderType = cShader::UNKNOWN;
	return;
}

cShaderManager::cShader::~cShader()
{
	return;
}

std::string cShaderManager::cShader::getShaderTypeString(void)
{
	switch (this->shaderType)
	{
	case cShader::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case cShader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
		//
	case cShader::GEOMETRY_SHADER:
		return "GEOMETRY_SHADER";
		break;
		//
	case cShader::COMPUTE_SHADER:
		return "COMPUTE_SHADER";
		break;
	case cShader::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}
	// Should never reach here...
	return "UNKNOWN_SHADER_TYPE";
}


//// TODO: For the students to do, because it's FUN, FUN, FUN
//std::map< std::string /*name of uniform variable*/,
//		    unsigned int /* uniform location ID */ > 
//						mapUniformName_to_UniformLocation;

// Look up the uniform inside the shader, then save it, if it finds it
bool cShaderManager::cShaderProgram::LoadUniformLocation(std::string variableName)
{
	// 
	GLint uniLocation = glGetUniformLocation(this->ID,
		variableName.c_str());
	// Did it find it (not -1)
	if (uniLocation == -1)
	{	// Nope.
		return false;
	}
	// Save it
	this->mapUniformName_to_UniformLocation[variableName.c_str()] = uniLocation;

	return true;
}

bool cShaderManager::cShaderProgram::LoadActiveUniforms(void)
{
	return false;
}

// Translates the GLenum uniform type to a human readable string
// GL_FLOAT, GL_FLOAT_VEC2, GL_DOUBLE, etc.
// typedef unsigned int GLenum;
std::string cShaderManager::cShaderProgram::GetTypeFromGLEnum(unsigned int typeEnum)
{
	std::string glType;

	switch (typeEnum)
	{
		case 1406:
			glType = "GL_FLOAT";
		case 35564:
			glType = "GL_FLOAT_VEC2";
		case 5130:
			glType = "GL_DOUBLE";
		default:
			glType = "glType not found";
	}

	return glType;
}

// Look up the uniform location and save it.
int cShaderManager::cShaderProgram::getUniformID_From_Name(std::string name)
{
	std::map< std::string /*name of uniform variable*/,
		int /* uniform location ID */ >::iterator
		itUniform = this->mapUniformName_to_UniformLocation.find(name);

	if (itUniform == this->mapUniformName_to_UniformLocation.end())
	{
		return -1;		// OpenGL uniform not found value
	}

	return itUniform->second;		// second if the "int" value
}
