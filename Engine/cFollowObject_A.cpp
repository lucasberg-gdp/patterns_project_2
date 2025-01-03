#include "cFollowObject_A.h"

void cFollowObject_A::Init(sPhysicsProperties* follower, sPhysicsProperties* leader, double timeToFollow,
	float followDistance, glm::vec3 offset, float maxFollowSpeed,
	float minAccelerationRange, float maxAccelerationRange)
{
	m_ElapsedTime = 0.0;
	m_TimeToFollow = timeToFollow;

	m_FollowDistance = followDistance;
	m_Offset = offset;
	m_MaxFollowSpeed = maxFollowSpeed;
	m_MinAccelerationRange = minAccelerationRange;
	m_MaxAccelerationRange = maxAccelerationRange;

	m_FollowerPhysics = follower;
	m_LeaderPhysics = leader;
}

void cFollowObject_A::Start(void)
{
	glm::vec3 directionWithOffset = m_LeaderPhysics->position + m_Offset;

	if (glm::distance(directionWithOffset, m_FollowerPhysics->position) > m_FollowDistance)
	{
		glm::vec3 directionToLeader = glm::normalize(directionWithOffset - m_FollowerPhysics->position);
		m_FollowerPhysics->velocity = directionToLeader * m_MaxFollowSpeed;
	}
	else
	{
		m_FollowerPhysics->velocity = glm::vec3(0.0f);
	}

	return;
}

void cFollowObject_A::Update(double deltaTime)
{
	this->m_ElapsedTime += deltaTime;

	glm::vec3 directionWithOffset = m_LeaderPhysics->position + m_Offset;

	float distanceBetweenObjects = glm::distance(directionWithOffset, m_FollowerPhysics->position);
	glm::vec3 directionToLeader = glm::normalize(directionWithOffset - m_FollowerPhysics->position);

	if (m_HasArrived)
	{
		m_IsSpeedingDown = false;
		m_FollowerPhysics->velocity = glm::vec3(0.0f);
		m_FollowerPhysics->acceleration = glm::vec3(0.0f);
		return;
	}

	if (distanceBetweenObjects > m_MaxAccelerationRange && distanceBetweenObjects > m_MinAccelerationRange)
	{
		if (glm::length(m_FollowerPhysics->velocity) < glm::length(directionToLeader * m_MaxFollowSpeed))
		{
			m_FollowerPhysics->acceleration = directionToLeader;
		}
		else
		{
			m_FollowerPhysics->acceleration = glm::vec3(0.0f);
		}
	}
	else if (distanceBetweenObjects > m_MinAccelerationRange)
	{
		if (!m_IsSpeedingDown)
		{
			// Get current speed (magnitude of velocity vector)
			float currentSpeed = glm::length(m_FollowerPhysics->velocity);

			// Calculate required acceleration to stop at m_FollowDistance
			float requiredAccelerationMagnitude = -(currentSpeed * currentSpeed) / (2.0f * (m_FollowDistance - m_MinAccelerationRange));

			// Apply acceleration in the opposite direction of velocity
			m_Acceleration = glm::normalize(-m_FollowerPhysics->velocity) * requiredAccelerationMagnitude;
		}

		m_IsSpeedingDown = true;
		//m_FollowerPhysics->acceleration = m_Acceleration;
	}
	else if (distanceBetweenObjects > m_FollowDistance)
	{
		m_FollowerPhysics->velocity = m_FollowerPhysics->velocity + m_Acceleration * (float)deltaTime;

		if (glm::length(m_FollowerPhysics->velocity) < 10.0f)
		{
			m_HasArrived = true;
		}
	}
	else
	{
		m_IsSpeedingDown = false;
		m_FollowerPhysics->velocity = glm::vec3(0.0f);
		m_FollowerPhysics->acceleration = glm::vec3(0.0f);
	}

	return;
}
bool cFollowObject_A::isFinished(void)
{
	// Has the amount of time passed
	if (this->m_ElapsedTime >= this->m_TimeToFollow)
	{
		// We've arrived

		OnFinished();
		return true;
	}

	// Keep going...
	return false;

}

void cFollowObject_A::OnFinished(void)
{
	// TODO: 
	m_FollowerPhysics->StopPhysicsObject();

	return;
}