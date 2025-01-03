#pragma once
#include <glm/vec4.hpp>
#include <string>

class cColor
{
public:
	static glm::vec4 ConvertHexToVec4(const std::string& hex);
	static void NormalizeRGBA(glm::vec4& RGBA);

	static const glm::vec4 COLOR_RED;// = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	static const glm::vec4 COLOR_GREEN;// = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	static const glm::vec4 COLOR_BLUE;// = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	static const glm::vec4 COLOR_WHITE;
};

