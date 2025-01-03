#pragma once

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <string>
#include "cJsonHandler.h"

// This structure matches what's in the shader
class cLight
{
private:
    unsigned int m_lightUniqueID;
    static const unsigned int FIRST_LIGHT_UNIQUE_ID = 1000;
    static unsigned int m_nextLightUniqueID;

public:
    cLight();

    std::string lightFriendlyName;
    glm::vec4 position;
    glm::vec4 diffuse;	// Colour of the light (used for diffuse)
    glm::vec4 specular;	// rgb = highlight colour, w = power
    glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
    glm::vec4 direction;	// Spot, directional lights
    glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
                    // 0 = pointlight
                    // 1 = spot light
                    // 2 = directional light
    glm::vec4 param2;	// x = 0 for off, 1 for on

    void TurnOn(void);
    void TurnOff(void);

    // These are the uniform locations for this light in the shader
    GLint position_UL;
    GLint diffuse_UL;
    GLint specular_UL;
    GLint atten_UL;
    GLint direction_UL;
    GLint param1_UL;
    GLint param2_UL;

    float flicker = 0.0f;
    float flickerSpeed = 0.01f;
};

class cLightManager
{
    int m_selectedLight = 0;


public:
    cLightManager();

    // This is called once
    void SetUniformLocations(GLuint shaderID);

    // This is called every frame
    void UpdateUniformValues(GLuint shaderID);

    static const unsigned int NUMBER_OF_LIGHTS_IM_USING = 50;
    cLight theLights[NUMBER_OF_LIGHTS_IM_USING];

    int GetSelectedLight();
    bool SelectLight(int index);

    void SelectNextLight();
    void SelectPreviousLight();

    std::string LightsInfo();

    cLight GetLightSelected();
    bool GetLightByFriendlyName(std::string friendlyName, cLight& light);

    bool SetLightPosition(std::string friendlyName, glm::vec4 position);
    bool SetLightOrientation(std::string friendlyName, glm::vec4 orientation);
    bool SetLightAttenuation(std::string lightName, glm::vec4 attenuation);

    std::vector<sLightsInfo> SerializeLight();

    void Update(double deltaTime);

    void TurnOnLightByFriendlyName(std::string friendlyName);
    void TurnOffLightByFriendlyName(std::string friendlyName);

    void TurnOffAllLights();
    void TurnOnAllLights();
};

