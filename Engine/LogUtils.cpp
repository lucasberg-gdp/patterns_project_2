#include "LogUtils.h"
#include <iostream>

void LogUtils::Vector3Log(glm::vec3 vec3)
{
	std::cout << "x: " << vec3.x << " y: " << vec3.y << " z: " << vec3.z << std::endl;
}

void LogUtils::Vector3Log(std::string vectorName, glm::vec3 vec3)
{
	std::cout << vectorName << " (" << vec3.x << ", " << vec3.y << ", " << vec3.z << ")" << std::endl;
}

void LogUtils::Vector4Log(glm::vec4 vec4)
{
	std::cout << "x: " << vec4.x << " y: " << vec4.y << " z: " << vec4.z << " w: " << vec4.w << std::endl;
}

void LogUtils::Vector4Log(std::string vectorName, glm::vec4 vec4)
{
	std::cout << vectorName << " (" << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << ")" << std::endl;
}

void LogUtils::QuatLog(glm::quat quat)
{
	std::cout << "w: " << quat.w << " x: " << quat.x << " y: " << quat.y << " z: " << quat.z << std::endl;
}

void LogUtils::QuatLog(std::string vectorName, glm::quat quat)
{
	std::cout << vectorName << " (" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << ")" << std::endl;
}

void LogUtils::Mat4Log(glm::mat4 mat)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		std::cout << "[" << mat[i][0] << ", " << mat[i][1] << ", " << mat[i][2] << ", " << mat[i][3] << "]" << std::endl;
	}
}
