#pragma once
#include "cGameObject.h"

class cLionPlayer : public cGameObject
{
	float lionSpeed = 2.0f;

	void MoveTowards(glm::vec3 direction);

	void AdjustOrientation(glm::vec3 eulerOrientation);
public:
	float horizontalMovement = 0.0f;
	float verticalMovement = 0.0f;

	cLionPlayer(cMesh* mesh) : cGameObject( mesh) {}

	void SetHorizontalMovement(float horizontal);
	void SetVerticalMovement(float vertical);

	void Update(double deltaTime) override;
};

