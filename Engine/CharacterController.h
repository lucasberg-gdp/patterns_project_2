#pragma once

#include "iSystem.h"
#include "cGameObject.h"

class CharacterController : public iSystem
{
public:
	CharacterController();

	cGameObject* characterGameObject = nullptr;
	cMesh* terrainMesh = nullptr;

	float horizontalMove = 0.0f;
	float verticalMove = 0.0f;

	float height = 0.0f;

	bool isPressingShift = false;

	float walkingSpeed = 50.0f;
	float runningSpeed = 100.0f;

	float GetHeightInTerrain(glm::vec3 position);

	void MoveToDirection(glm::vec3 direction);

	void UpdateCharacterAnimation(double deltaTime);

	// Inherited via iSystem
	void Awake(GLFWwindow& window) override;

	void Start(GLFWwindow& window) override;

	void EarlyUpdate(GLFWwindow& window, double deltaTime) override;

	void Update(GLFWwindow& window, double deltaTime) override;

	void LateUpdate(GLFWwindow& window, double deltaTime) override;

	void End(GLFWwindow& window) override;

	// Inherited via iSystem
	std::string GetSystemType() override;
};

