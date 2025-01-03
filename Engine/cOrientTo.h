#pragma once

#include "iCommand.h"
#include "sPhysicsProperties.h"

class cOrientTo : public iCommand
{
	glm::vec3 m_StartOrientation = glm::vec3(0.0f);
	glm::vec3 m_EndOrientation = glm::vec3(0.0f);
	double m_TimeToOrient = 0.0;
	sPhysicsProperties* m_PhysicsObject = nullptr;
	// This is calculated
	double m_ElapsedTime = 0.0;

	MoveType m_MoveType = MoveType::Linear;

	glm::vec3 m_RotSpeed = glm::vec3(0.0f);
public:
	void Init(sPhysicsProperties* pObject, glm::vec3 eulerOrientation, double timeToOrient);
	void Init(sPhysicsProperties* pObject, glm::vec3 eulerOrientation, double timeToOrient, bool easeIn, bool easeOut);

	// Inherited via iCommand
	void Start(void) override;
	void Update(double deltaTime) override;
	bool isFinished(void) override;
	void OnFinished(void) override;
};

