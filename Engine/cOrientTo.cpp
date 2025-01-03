#include "cOrientTo.h"

#include <glm/gtx/easing.hpp>

void cOrientTo::Init(sPhysicsProperties* pObject, glm::vec3 eulerOrientation, double timeToOrient)
{
	Init(pObject, eulerOrientation, timeToOrient, false, false);
	return;
}

void cOrientTo::Init(sPhysicsProperties* pObject, glm::vec3 eulerOrientation, double timeToOrient, bool easeIn, bool easeOut)
{
	m_EndOrientation = eulerOrientation;
	m_PhysicsObject = pObject;
	m_TimeToOrient = timeToOrient;
	this->m_ElapsedTime = 0.0;

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

void cOrientTo::Start(void)
{
	m_StartOrientation = m_PhysicsObject->eulerOrientation;

    glm::vec3 totalRotation = m_EndOrientation - m_PhysicsObject->eulerOrientation;
    m_RotSpeed = totalRotation / (float)m_TimeToOrient;
}

void cOrientTo::Update(double deltaTime)
{
    m_ElapsedTime += deltaTime;

	double percentToDestiny = m_ElapsedTime / m_TimeToOrient;
	glm::vec3 totalDistance = m_EndOrientation - m_StartOrientation;

	float easedPercent;

	switch (m_MoveType)
	{
	case MoveType::Linear:
		m_PhysicsObject->rotationSpeed = m_RotSpeed;
		break;

	case MoveType::EasyIn:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseIn(percentToDestiny);
			m_PhysicsObject->eulerOrientation = m_StartOrientation + easedPercent * totalDistance;
		}
		else
		{
			m_RotSpeed = glm::vec3(0.0f);
			m_PhysicsObject->rotationSpeed = m_RotSpeed;
		}

		break;

	case MoveType::EasyOut:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseOut(percentToDestiny);
			m_PhysicsObject->eulerOrientation = m_StartOrientation + easedPercent * totalDistance;
		}
		else
		{
			m_RotSpeed = glm::vec3(0.0f);
			m_PhysicsObject->rotationSpeed = m_RotSpeed;
		}

		break;

	case MoveType::EasyInAndOut:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseInOut(percentToDestiny);
			m_PhysicsObject->eulerOrientation = m_StartOrientation + easedPercent * totalDistance;

		}
		else
		{
			m_RotSpeed = glm::vec3(0.0f);
			m_PhysicsObject->rotationSpeed = m_RotSpeed;
		}

		break;
	}


    return;
}

bool cOrientTo::isFinished(void)
{
	if (m_ElapsedTime >= m_TimeToOrient)
	{
		OnFinished();
		return true;
	}

	// Keep going...
	return false;
}

void cOrientTo::OnFinished(void)
{
	// TODO: 
	m_PhysicsObject->rotationSpeed = glm::vec3(0.0f);
	m_PhysicsObject->setRotationFromEuler(m_EndOrientation);

	return;
}