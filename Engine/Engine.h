#pragma once
#include "OpenGLCommon.h"

#include <string>

#include "cJsonHandler.h"
#include "fbo/cFBO.h"
#include <glm/vec2.hpp>
#include "cMesh.h"
#include "iSystem.h"

class Engine
{
	// TODO: Turn everything into systems
	//enum System {
	//	Mesh,
	//	Physics,
	//};

	//std::map<System, iSystem> m_systems;

	std::string windowText = "Project 2 - Game Engine Frameworks & Pattterns";

	static void OpenGlInit(GLFWwindow* window);
	static void LoadScene(GLFWwindow* window);

public:
	static std::vector < cFBO* > g_FBOs;

	cFBO* GetRandomFBO();

	static cFBO* g_FBO_1;
	static cFBO* g_FBO_2;
	//static cFBO* g_FBO_3;
	//static cFBO* g_FBO_4;
	//static cFBO* g_FBO_5;

	float g_HeightAdjust = 10.0f;
	glm::vec2 g_UVOffset = glm::vec2(0.0f, 0.0f);

	static void Init(GLFWwindow* window);

	void Update(double deltaTime, GLFWwindow& window, GLuint shaderProgramID);

	void End(GLFWwindow& window);

	void EndOfFrame(double deltaTime, GLFWwindow& window);

	static void LoadAnimations(std::vector<Bones::CharacterAnimation*>& animations);

	static void LoadTexturesFromFile(std::vector<std::string>& textures);

	static void LoadLights(std::vector<sLightsInfo>& sceneLights, GLuint shaderProgramID);

	static bool LoadModels(std::vector<sModel> sceneModelsInfo);
	static bool LoadPhysics(void);

	void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel, GLuint shaderProgramID, double deltaTime);
	void DrawObjects(GLuint shaderProgramID, double deltaTime);

	void SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgramID);

	void DrawPass1(GLFWwindow* window, double deltaTime, int width, int height, glm::vec3 cameraEye, glm::vec3 cameraTarget);
	void DrawPass2(GLFWwindow* window, double deltaTime, int widht, int height);
	void DrawPass3(GLFWwindow* window, double deltaTime);

	static void SetBoolInFragmentShader(std::string, bool trueOrFalse);

	static void SetUniformFloatInShader(std::string floatName, float value);

	static void SetUniformVec4InShader(std::string vectorName, glm::vec4 value);

	void RenderBoneDebug() const;

	void DrawLightDebugSpheres(glm::mat4 matProjection, glm::mat4 matView, GLuint shaderProgramID, double deltaTime);

	void g_DrawDebugSphere(glm::vec3 position, float scale, glm::vec4 colourRGBA, double deltaTime);

	void SetWindowName(std::string windowName);
};