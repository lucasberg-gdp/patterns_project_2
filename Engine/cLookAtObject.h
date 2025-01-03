#pragma once
#include "iCommand.h"
#include <glm/vec3.hpp>
#include "sPhysicsProperties.h"

class cLookAtObject : public iCommand
{
	double m_TimeToOrient;
	sPhysicsProperties* m_CommandedPhysics;
	sPhysicsProperties* m_LookedPhysics;

	glm::vec3 m_StartOrientation;
	glm::vec3 m_EndOrientation;
	double m_ElapsedTime;

	MoveType m_MoveType = MoveType::Linear;

	double m_RotSpeed = 0.0;
public:
	void Init(sPhysicsProperties* commandedPhysics, sPhysicsProperties* lookedPhysics, double rotationSpeed);
	void Init(sPhysicsProperties* commandedPhysics, sPhysicsProperties* lookedPhysics, double rotationSpeed, bool easeIn, bool easeOut);

	void Start(void) override;
	void Update(double deltaTime) override;
	bool isFinished(void) override;
	void OnFinished(void) override;
};

