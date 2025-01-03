#pragma once
#include "iCommand.h"
#include "sPhysicsProperties.h"
#include <glm/vec3.hpp>

class cAddVelocity :public iCommand
{
	double m_ElapsedTime;
	sPhysicsProperties* m_ObjectPhysics = nullptr;
	glm::vec3 m_VelocityDirection = glm::vec3(0.0f);
	float m_Velocity = 0.0f;
	double m_TotalTime = 0.0;
	double m_TimeToAddVelocity = 0.0;
public:
	void Init(sPhysicsProperties* objectPhysics, double timeToAddVelocity, double totalTime, glm::vec3 direction, float velocity);

	// Inherited via iCommand
	void Start(void) override;
	void Update(double deltaTime) override;
	bool isFinished(void) override;
	void OnFinished(void) override;
};