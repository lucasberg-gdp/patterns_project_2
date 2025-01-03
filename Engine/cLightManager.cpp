#include "cLightManager.h"
#include <sstream> //"string stream"
#include <iostream>
#include "MathUtils.h"

#include <glm/gtc/matrix_transform.hpp>

cLight::cLight()
{
	this->lightFriendlyName = "notnamed";

	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// White light
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// White light

	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	this->atten = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);	
	// Spot, directional lights
	// (Default is stright down)
	this->direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);	
	// x = lightType, y = inner angle, z = outer angle, w = TBD
	// type = 0 => point light
	this->param1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					// 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	// x = 0 for off, 1 for on
	this->param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


	this->position_UL = -1;
	this->diffuse_UL = -1;
	this->specular_UL = -1;
	this->atten_UL = -1;
	this->direction_UL = -1;
	this->param1_UL = -1;
	this->param2_UL = -1;

}

void cLight::TurnOn(void)
{
	this->param2.x = 1.0f;
	return;
}

void cLight::TurnOff(void)
{
	this->param2.x = 0.0f;
	return;
}


void cLightManager::SetUniformLocations(GLuint shaderID)
{
	for (int i = 0; i < this->NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		// vec4 position;

		this->theLights[i].position_UL = glGetUniformLocation(shaderID, ("theLights[" + std::to_string(i) + "].position").c_str());
		//        vec4 diffuse;	// Colour of the light (used for diffuse)
		this->theLights[i].diffuse_UL = glGetUniformLocation(shaderID, ("theLights[" + std::to_string(i) + "].diffuse").c_str());
		//        vec4 specular;	// rgb = highlight colour, w = power
		this->theLights[i].specular_UL = glGetUniformLocation(shaderID, ("theLights[" + std::to_string(i) + "].specular").c_str());
		//        vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
		this->theLights[i].atten_UL = glGetUniformLocation(shaderID, ("theLights[" + std::to_string(i) + "].atten").c_str());
		//        vec4 direction;	// Spot, directional lights
		this->theLights[i].direction_UL = glGetUniformLocation(shaderID, ("theLights[" + std::to_string(i) + "].direction").c_str());
		//        vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
		this->theLights[i].param1_UL = glGetUniformLocation(shaderID, ("theLights[" + std::to_string(i) + "].param1").c_str());
		//        vec4 param2;	// x = 0 for off, 1 for on
		this->theLights[i].param2_UL = glGetUniformLocation(shaderID, ("theLights[" + std::to_string(i) + "].param2").c_str());
	}

	return;
}

// This is called every frame
void cLightManager::UpdateUniformValues(GLuint shaderID)
{
	for ( unsigned int index = 0; index != cLightManager::NUMBER_OF_LIGHTS_IM_USING; index++ )
	{
		glUniform4f(theLights[index].position_UL,
					theLights[index].position.x,
					theLights[index].position.y,
					theLights[index].position.z,
					theLights[index].position.w);

		glUniform4f(theLights[index].diffuse_UL,
					theLights[index].diffuse.x,
					theLights[index].diffuse.y,
					theLights[index].diffuse.z,
					theLights[index].diffuse.w);

		glUniform4f(theLights[index].specular_UL,
					theLights[index].specular.x,
					theLights[index].specular.y,
					theLights[index].specular.z,
					theLights[index].specular.w);

		glUniform4f(theLights[index].atten_UL,
					theLights[index].atten.x,
					theLights[index].atten.y,
					theLights[index].atten.z,
					theLights[index].atten.w);

		glUniform4f(theLights[index].direction_UL,
					theLights[index].direction.x,
					theLights[index].direction.y,
					theLights[index].direction.z,
					theLights[index].direction.w);

		glUniform4f(theLights[index].param1_UL,
					theLights[index].param1.x,
					theLights[index].param1.y,
					theLights[index].param1.z,
					theLights[index].param1.w);

		glUniform4f(theLights[index].param2_UL,
					theLights[index].param2.x,
					theLights[index].param2.y,
					theLights[index].param2.z,
					theLights[index].param2.w);
	}// for ( unsigned int index...

	return;
}

int cLightManager::GetSelectedLight()
{
	return m_selectedLight;
}

bool cLightManager::SelectLight(int lightIndex)
{
	if (lightIndex < NUMBER_OF_LIGHTS_IM_USING)
	{
		m_selectedLight = lightIndex;
		return true;
	}

	return false;
}

void cLightManager::SelectNextLight()
{
	m_selectedLight++;
	if (m_selectedLight >= NUMBER_OF_LIGHTS_IM_USING)
	{
		m_selectedLight = 0;
	}
	std::cout << "Selected light: " << m_selectedLight << " " << theLights[m_selectedLight].lightFriendlyName << std::endl;
}

void cLightManager::SelectPreviousLight()
{
	m_selectedLight--;
	if (m_selectedLight < 0)
	{
	    m_selectedLight = NUMBER_OF_LIGHTS_IM_USING - 1;
	}
	std::cout << "Selected light: " << m_selectedLight << " " << theLights[m_selectedLight].lightFriendlyName << std::endl;
}

std::string cLightManager::LightsInfo()
{
	// position
	std::string cameraPositionX = MathUtils::RoundFloat(theLights[m_selectedLight].position.x, 2);
	std::string cameraPositionY = MathUtils::RoundFloat(theLights[m_selectedLight].position.y, 2);
	std::string cameraPositionZ = MathUtils::RoundFloat(theLights[m_selectedLight].position.z, 2);

	// const, linear, quad, inner, outer
	std::string lightConst = std::to_string(theLights[m_selectedLight].atten.x);
	std::string lightLinear = std::to_string(theLights[m_selectedLight].atten.y);
	std::string lightQuad = std::to_string(theLights[m_selectedLight].atten.z);
	std::string lightInner = std::to_string(theLights[m_selectedLight].param1.y);
	std::string lightOuter = std::to_string(theLights[m_selectedLight].param1.z);

	std::string name;

	return "Light [" + std::to_string(m_selectedLight) + "] " + theLights[m_selectedLight].lightFriendlyName
		+ " Position: ("
		+ cameraPositionX + ", "
		+ cameraPositionY + ", "
		+ cameraPositionZ + ") "
		+ "const:" + lightConst + " "
		+ "linear:" + lightLinear + " "
		+ "quad:" + lightQuad + " "
		+ "inner:" + lightInner + " "
		+ "outer:" + lightOuter + " ";
}

cLight cLightManager::GetLightSelected()
{
	return theLights[m_selectedLight];
}

bool cLightManager::GetLightByFriendlyName(std::string friendlyName, cLight& light)
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		if (theLights[i].lightFriendlyName == friendlyName)
		{
			light = theLights[i];
			return true;
		}
	}

	return false;
}

bool cLightManager::SetLightPosition(std::string friendlyName, glm::vec4 position)
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		if (theLights[i].lightFriendlyName == friendlyName)
		{
			theLights[i].position = position;
			return true;
		}
	}

	return false;
}

bool cLightManager::SetLightOrientation(std::string friendlyName, glm::vec4 orientation)
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		if (theLights[i].lightFriendlyName == friendlyName)
		{
			theLights[i].direction = orientation;
			return true;
		}
	}

	return false;
}

bool cLightManager::SetLightAttenuation(std::string lightName, glm::vec4 attenuation)
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		if (theLights[i].lightFriendlyName == lightName)
		{
			theLights[i].atten = attenuation;
			return true;
		}
	}

	return false;
}

std::vector<sLightsInfo> cLightManager::SerializeLight()
{
	std::vector<sLightsInfo> lightsInfoVector;

	for (unsigned int i = 0; i < this->NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		sLightsInfo lightsInfo;

		lightsInfo.FriendlyName = theLights[i].lightFriendlyName;
		lightsInfo.Atten = theLights[i].atten;
		lightsInfo.Diffuse = theLights[i].diffuse;
		lightsInfo.Direction = theLights[i].direction;
		lightsInfo.Param1 = theLights[i].param1;
		lightsInfo.Param2 = theLights[i].param2;
		lightsInfo.Position = theLights[i].position;
		lightsInfo.Specular = theLights[i].specular;

		lightsInfoVector.push_back(lightsInfo);
	}
	
	return lightsInfoVector;
}

void cLightManager::Update(double deltaTime)
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		//if (theLights[i].lightFriendlyName == "beholder")
		//{
		//	cMesh* beholderMesh = g_currentScene->GetMeshByFriendlyName("beholder");

		//}

		//if (theLights[i].lightFriendlyName == "campfire")
		//{
		//	float value = glm::sin(theLights[i].flicker);

		//	theLights[i].flicker += 0.5f;

		//	if (value < 0.0f)
		//	{
		//		theLights[i].atten.z += theLights[i].flickerSpeed;
		//	}
		//	else
		//	{
		//		theLights[i].atten.z -= theLights[i].flickerSpeed;
		//	}
		//}

		//if (theLights[i].lightFriendlyName == "towerlight")
		//{
		//	glm::quat lightOrientation = glm::quat(
		//		theLights[i].direction.x, 
		//		theLights[i].direction.y, 
		//		theLights[i].direction.z, 
		//		theLights[i].direction.w);

		//	lightOrientation = glm::rotate(lightOrientation, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
		//	theLights[i].direction = glm::vec4(lightOrientation.w, lightOrientation.x, lightOrientation.y, lightOrientation.z);
		//}
	}
}

void cLightManager::TurnOnLightByFriendlyName(std::string friendlyName)
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		if (theLights[i].lightFriendlyName == friendlyName)
		{
			theLights[i].param2.x = 1.0f;
		}
	}
}

void cLightManager::TurnOffLightByFriendlyName(std::string friendlyName)
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		if (theLights[i].lightFriendlyName == friendlyName)
		{
			theLights[i].param2.x = 0.0f;
		}
	}
}

void cLightManager::TurnOffAllLights()
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		theLights[i].param2.x = 0.0f;
	}
}

void cLightManager::TurnOnAllLights()
{
	for (unsigned int i = 0; i < NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		theLights[i].param2.x = 1.0f;
	}
}

cLightManager::cLightManager()
{
}