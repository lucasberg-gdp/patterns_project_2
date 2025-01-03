#pragma once

#ifdef APIENTRY
#undef APIENTRY
#endif
#include "OpenGLCommon.h"	// glfw.h, etc. (openGL.h)

class cSkyBox
{
public:
	cSkyBox();
	void DrawSkyBox(GLuint shaderProgramID, double deltaTime);
	
};

