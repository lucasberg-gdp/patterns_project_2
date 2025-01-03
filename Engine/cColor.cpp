#include "cColor.h"
#include <iostream>

const glm::vec4 cColor::COLOR_RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 cColor::COLOR_GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 cColor::COLOR_BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
const glm::vec4 cColor::COLOR_WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

glm::vec4 cColor::ConvertHexToVec4(const std::string& hex)
{
	size_t pos;
	std::string hexWithoutHash = hex.substr(1);

	std::string red = hex.substr(1, 2);
	std::string green = hex.substr(3, 2);
	std::string blue = hex.substr(5, 2);

	float redNumber = (float)std::stoi(red, &pos, 16);
	float greenNumber = (float)std::stoi(green, &pos, 16);
	float blueNumber = (float)std::stoi(blue, &pos, 16);

	glm::vec4 convertedHex = glm::vec4(redNumber, greenNumber, blueNumber, 255.0f);
	NormalizeRGBA(convertedHex);
	return convertedHex;
}

void cColor::NormalizeRGBA(glm::vec4& RGBA)
{
	RGBA = glm::vec4(RGBA.r / 255.0f, RGBA.g / 255.0f, RGBA.b / 255.0f, RGBA.a / 255.0f);
}
