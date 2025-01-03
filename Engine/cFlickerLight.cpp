#include "cFlickerLight.h"
#include "cScene.h"

extern cScene* g_currentScene;

void cFlickerLight::Init(std::string lightFriendlyName, glm::vec4 brightestLight, glm::vec4 dimmestLight, double flickerSpeed)
{
	m_ElapsedTime = 0.0;
	m_LightFriendlyName = lightFriendlyName;
	m_BrightestLight = brightestLight;
	m_DimmestLight = dimmestLight;
	m_FlickerSpeed = flickerSpeed;
}

void cFlickerLight::Start(void)
{
	m_IsActive = true;
}

void cFlickerLight::Update(double deltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	m_ElapsedTime += deltaTime * m_FlickerSpeed;

	glm::vec4 brightestLight = glm::vec4(0.0, 0.00001, 0.00001, 0.00001);

	glm::vec4 dimmestLight = glm::vec4(0.0, 0.00001, 0.001, 0.003);

	cLight light;
	g_currentScene->m_sceneLights->GetLightByFriendlyName(m_LightFriendlyName, light);

	float oscillationFactor = (glm::sin(static_cast<float>(m_ElapsedTime)) + 1.0f) / 2.0f;

	glm::vec4 attenuation = m_DimmestLight + oscillationFactor * (m_BrightestLight - m_DimmestLight);

	g_currentScene->m_sceneLights->SetLightAttenuation(m_LightFriendlyName, attenuation);
}

bool cFlickerLight::isFinished(void)
{
    return false;
}

void cFlickerLight::OnFinished(void)
{

}
