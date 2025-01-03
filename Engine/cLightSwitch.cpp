#include "cLightSwitch.h"
#include "cScene.h"

extern cScene* g_currentScene;

void cLightSwitch::Init(std::string lightName, double timeToSwitchLight, bool isTurnOn)
{
	m_LightName = lightName;
	m_TimeToSwitchLight = timeToSwitchLight;
	m_IsTurnOn = isTurnOn;
}

void cLightSwitch::Start(void)
{
	m_ElapsedTime = 0.0;
}

void cLightSwitch::Update(double deltaTime)
{
	m_ElapsedTime += deltaTime;
}

bool cLightSwitch::isFinished(void)
{
	if (this->m_ElapsedTime >= this->m_TimeToSwitchLight)
	{
		OnFinished();
		return true;
	}

	// Keep going...
	return false;
}

void cLightSwitch::OnFinished(void)
{
	if (m_IsTurnOn)
	{
		g_currentScene->m_sceneLights->TurnOnLightByFriendlyName(m_LightName);
	}
	else
	{
		g_currentScene->m_sceneLights->TurnOffLightByFriendlyName(m_LightName);
	}
}
