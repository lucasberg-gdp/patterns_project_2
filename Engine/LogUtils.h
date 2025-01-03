#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/quaternion_float.hpp>

#include <string>

class LogUtils
{
public:
	static void Vector3Log(glm::vec3 vec3);
	static void Vector3Log(std::string vectorName, glm::vec3 vec3);

	static void Vector4Log(glm::vec4 vec4);
	static void Vector4Log(std::string vectorName, glm::vec4 vec4);

	static void QuatLog(glm::quat quat);
	static void QuatLog(std::string vectorName, glm::quat quat);

	static void Mat4Log(glm::mat4 mat);
};

