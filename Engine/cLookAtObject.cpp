#include "cLookAtObject.h"
#include <glm/gtx/easing.hpp>

void cLookAtObject::Init(sPhysicsProperties* commandedPhysics, sPhysicsProperties* lookedPhysics, double rotationSpeed)
{
	Init(commandedPhysics, lookedPhysics, rotationSpeed, false, false);
}

void cLookAtObject::Init(sPhysicsProperties* commandedPhysics, sPhysicsProperties* lookedPhysics, double rotationSpeed, bool easeIn, bool easeOut)
{
	m_CommandedPhysics = commandedPhysics;
	m_LookedPhysics = lookedPhysics;
	m_RotSpeed = rotationSpeed;

	if (easeIn && !easeOut)
	{
		m_MoveType = MoveType::EasyIn;
	}
	else if (!easeIn && easeOut)
	{
		m_MoveType = MoveType::EasyOut;
	}
	else if (easeIn && easeOut)
	{
		m_MoveType = MoveType::EasyInAndOut;
	}
}

void cLookAtObject::Start(void)
{
    glm::vec3 commandedPosition = m_CommandedPhysics->position;
    glm::vec3 lookedPosition = m_LookedPhysics->position;

	glm::vec3 directionVec = lookedPosition - commandedPosition;

	glm::vec3 flatDirection = glm::normalize(glm::vec3(directionVec.x, 0.0f, directionVec.z));

	m_StartOrientation = m_CommandedPhysics->eulerOrientation;

	float yaw = atan2(flatDirection.x, flatDirection.z);
	m_EndOrientation = glm::vec3(0.0f, yaw, 0.0f);

	// s = s0 + vt :: vt = s - s0 :: t = (s - s0) / v
	float angularDifference = glm::abs(yaw - m_StartOrientation.y);
	m_TimeToOrient = angularDifference / m_RotSpeed;
}

void cLookAtObject::Update(double deltaTime)
{
	m_ElapsedTime += deltaTime;

	double percentToDestiny = m_ElapsedTime / m_TimeToOrient;
	glm::vec3 totalDistance = m_EndOrientation - m_StartOrientation;

	float easedPercent;

	switch (m_MoveType)
	{
	case MoveType::Linear:
		glm::vec3 totalRotation = m_EndOrientation - m_CommandedPhysics->eulerOrientation;
		m_CommandedPhysics->rotationSpeed = totalRotation / (float)m_TimeToOrient;

		break;

	case MoveType::EasyIn:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseIn(percentToDestiny);
			m_CommandedPhysics->eulerOrientation = m_StartOrientation + easedPercent * totalDistance;
		}
		else
		{
			m_CommandedPhysics->StopPhysicsObject();
		}

		break;

	case MoveType::EasyOut:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseOut(percentToDestiny);
			m_CommandedPhysics->eulerOrientation = m_StartOrientation + easedPercent * totalDistance;
		}
		else
		{
			m_CommandedPhysics->StopPhysicsObject();
		}

		break;

	case MoveType::EasyInAndOut:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseInOut(percentToDestiny);
			m_CommandedPhysics->eulerOrientation = m_StartOrientation + easedPercent * totalDistance;
		}
		else
		{
			m_CommandedPhysics->StopPhysicsObject();
		}

		break;
	}


	return;
}

bool cLookAtObject::isFinished(void)
{
	if (m_ElapsedTime >= m_TimeToOrient)
	{
		OnFinished();
		return true;
	}

	// Keep going...
	return false;
}

void cLookAtObject::OnFinished(void)
{

}
