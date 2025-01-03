#pragma once

#include "OpenGLCommon.h"

#include <string>

class iSystem
{
public:
	virtual std::string GetSystemType() = 0;
	virtual void Awake(GLFWwindow& window) = 0;
	virtual void Start(GLFWwindow& window) = 0;
	virtual void EarlyUpdate(GLFWwindow& window, double deltaTime = 0.0) = 0;
	virtual void Update(GLFWwindow& window, double deltaTime = 0.0) = 0;
	virtual void LateUpdate(GLFWwindow& window, double deltaTime = 0.0) = 0;
	virtual void End(GLFWwindow& window) = 0;
};