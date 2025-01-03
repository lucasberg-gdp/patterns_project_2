#pragma once
#include "iCommand.h"
#include <string>
class cLightSwitch : public iCommand
{
	double m_ElapsedTime;

	std::string m_LightName;
	double m_TimeToSwitchLight;
	bool m_IsTurnOn = false;

public:
	void Init(std::string lightName, double timeToSwitchLight, bool isTurnOn);

	// Inherited via iCommand
	void Start(void) override;

	void Update(double deltaTime) override;

	bool isFinished(void) override;

	void OnFinished(void) override;

};

