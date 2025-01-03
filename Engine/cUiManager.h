#pragma once

#ifdef APIENTRY
#undef APIENTRY
#endif
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "OpenGLCommon.h"

#include <glm/fwd.hpp>

#include "cLightManager.h"

class cUiManager
{
	bool show_demo_window = true;
	bool show_another_window = false;
	bool showProjectInfo = true;
	bool showNewScene = false;
	bool showLightsEditor = false;
	bool showAddModel = false;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.40f);

	ImGuiIO io;

	void DisplayVec3InLine(std::string label, glm::vec3& vec3);
	void DisplayObjectScaleInLine(std::string label, glm::vec3& vec3);
	void DisplayRotationInLine(std::string label, glm::vec3& vec3);
	
	void DisplayProjectInfo();
	void DisplayNewScene();
	void DisplayCameraEditor();
	void DisplayLightEditor(cLightManager& lights);
	void DisplayAddModelEditor();


public:
	bool isHoveringUi = false;

	cUiManager() {}

	void Init(GLFWwindow* window);
	void Update(GLFWwindow& window);
	void End(GLFWwindow* window);
};

