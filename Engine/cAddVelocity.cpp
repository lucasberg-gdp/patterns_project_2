#include "cAddVelocity.h"

void cAddVelocity::Init(sPhysicsProperties* objectPhysics, double timeToAddVelocity, double totalTime, glm::vec3 velocityDirection, float velocity)
{
    m_ObjectPhysics = objectPhysics;
    m_VelocityDirection = velocityDirection;
	m_Velocity = velocity;
	m_TotalTime = totalTime;
	m_TimeToAddVelocity = timeToAddVelocity;
}

void cAddVelocity::Start(void)
{
    m_ElapsedTime = 0.0;

}

void cAddVelocity::Update(double deltaTime)
{
    m_ElapsedTime += deltaTime;

	if (this->m_ElapsedTime >= this->m_TimeToAddVelocity)
	{
		m_ObjectPhysics->velocity += m_VelocityDirection * m_Velocity;
	}
}

bool cAddVelocity::isFinished(void)
{
	if (this->m_ElapsedTime >= this->m_TotalTime)
	{

		OnFinished();
		return true;
	}

	return false;
}

void cAddVelocity::OnFinished(void)
{
	m_ObjectPhysics->velocity -= m_VelocityDirection * m_Velocity;
}
