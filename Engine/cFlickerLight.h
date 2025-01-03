#pragma once
#include "iCommand.h"

#include <glm/vec4.hpp>
#include <string>

class cFlickerLight :public iCommand
{
	bool m_IsActive = false;

	double m_ElapsedTime;
	std::string m_LightFriendlyName;

	glm::vec4 m_BrightestLight;
	glm::vec4 m_DimmestLight;
	double m_FlickerSpeed;

public:
	void Init(std::string lightFriendlyName,
		glm::vec4 brightestLight,
		glm::vec4 dimmestLight,
		double flickerSpeed
		);

	// Inherited via iCommand
	void Start(void) override;
	void Update(double deltaTime) override;
	bool isFinished(void) override;
	void OnFinished(void) override;
};

