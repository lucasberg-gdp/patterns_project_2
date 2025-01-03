#pragma once

#include "iCommand.h"
#include "sPhysicsProperties.h"

// Follow an object at a certain distance. 
// Pass a mesh pointer and a min distance and 
//	a max velocity and an offset
class cFollowObject_A : public iCommand
{
	sPhysicsProperties* m_FollowerPhysics = nullptr;
	sPhysicsProperties* m_LeaderPhysics = nullptr;

	glm::vec3 m_FollowingVelocity = glm::vec3(0.0f);

	glm::vec3 m_Acceleration = glm::vec3(0.0f);

	float m_FollowDistance = 0.0f;
	glm::vec3 m_Offset = glm::vec3(0.0f);
	float m_MaxFollowSpeed;
	float m_MinAccelerationRange;
	float m_MaxAccelerationRange;

	double m_ElapsedTime;
	double m_TimeToFollow;

	bool m_IsSpeedingDown = false;
	bool m_HasArrived = false;
public:
	void Init(sPhysicsProperties* follower, sPhysicsProperties* leader, double timeToFollow,
		float followDistance, glm::vec3 offset, float maxFollowSpeed, float minAccelerationRange, float maxAccelerationRange);

	// From the iCommand interface
	virtual void Start(void);
	virtual void Update(double deltaTime);
	virtual bool isFinished(void);
	virtual void OnFinished(void);
};
