#pragma once
#include "iCommand.h"
#include <vector>
#include <glm/vec3.hpp>
#include "sPhysicsProperties.h"

class cFollowBezierCubicCurve : public iCommand
{
	std::vector < glm::vec3> m_ControlPoints;
	float m_TimeToMove = 0.0f;
	sPhysicsProperties* m_physicsToMove = nullptr;

	glm::vec3 m_startXYZ;
	glm::vec3 m_endXYZ;
	double m_ElapsedTime;

	glm::vec3 m_velocity = glm::vec3(0.0f);
	glm::vec3 m_acceleration = glm::vec3(0.0f);

public:
	void Init(sPhysicsProperties* pMeshToControl, std::vector<glm::vec3> controlPoints, float timeToMove);

	glm::vec3 GetBezierPosition();

	// Inherited via iCommand
	void Start(void) override;
	void Update(double deltaTime) override;
	bool isFinished(void) override;
	void OnFinished(void) override;
};

