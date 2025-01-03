#pragma once
#include <vector>
#include "cScene.h"

class Game : public iSystem
{
	void CreateBeholder();
	void CreateCamera();
	void CreateRubble(std::string rubbleName);
	void CreateTorchlight(std::string torch);

	void MoveBeholderToPoint(
		glm::vec3 point,
		double seconds, 
		bool moveEaseIn, 
		bool moveEaseOut);

	void MoveBeholderToPointAndOrient(
		glm::vec3 point, double moveSeconds, bool moveEaseIn, bool moveEaseOut, 
		glm::vec3 degrees, double orientSeconds, bool orientEaseIn, bool orientEaseOut);

	void RunLuaScriptOnObjects(double deltaTime);
	std::vector<cGameObject*> luaHandledObjects;

	double timeElapsed = 0.0;
public:
	Game();

	void StartCutscene();
	void StopCutscene();

	void LinkLightToBeholder();

	std::vector<cScene> gameScenes;
	
	bool m_IsGameRunning = false;
	cGameObject* beholder = nullptr;
	cGameObject* camera = nullptr;

	void Awake(GLFWwindow& window) override;
	void Start(GLFWwindow& window) override;
	void EarlyUpdate(GLFWwindow& window, double deltaTime) override;
	void Update(GLFWwindow& window, double deltaTime) override;
	void LateUpdate(GLFWwindow& window, double deltaTime) override;
	void End(GLFWwindow& window) override;

	// Inherited via iSystem
	std::string GetSystemType() override;
};

