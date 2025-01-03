#include "OpenGLCommon.h"

#include <iostream>

std::string g_DecodeGLError(GLenum theError)
{
	std::string errorString = "UNKNOWN Error";

	switch (theError)
	{
	case GL_INVALID_ENUM:
		errorString = "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		errorString = "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		errorString = "GL_INVALID_OPERATION";
		break;
	case GL_STACK_OVERFLOW:
		errorString = "GL_STACK_OVERFLOW";
		break;
	case GL_STACK_UNDERFLOW:
		errorString = "GL_STACK_UNDERFLOW";
		break;
	case GL_OUT_OF_MEMORY:
		errorString = "GL_OUT_OF_MEMORY";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	case GL_CONTEXT_LOST:
		errorString = "GL_CONTEXT_LOST";
		break;
	}
	return errorString;
}

void g_ProcessOpenGLError(const char* file, int line)
{
	// Skip if in release mode
#ifdef _DEBUG

	GLenum err;
	std::string sError = "";
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "In " << file << " (" << line << "): " << ::g_DecodeGLError(err) << std::endl;
	}

#endif // DEBUG

	return;
}