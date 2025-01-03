#ifndef _OpenGLCommon_HG_
#define _OpenGLCommon_HG_

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

// https://www.khronos.org/opengl/wiki/OpenGL_Error
void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

std::string g_DecodeGLError(GLenum theError);

void g_ProcessOpenGLError(const char* file, int line);

#define gDEBUG_PROCESS_GL_ERROR ::g_ProcessOpenGLError(__FILE__, __LINE__);

#endif
