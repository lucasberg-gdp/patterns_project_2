#include "cMoveRelativeTime.h"

#include <glm/gtx/easing.hpp>

void cMoveRelativeTime::Init(sPhysicsProperties* pObject, glm::vec3 endXYZ, double timeToMove)
{
	Init(pObject, endXYZ, timeToMove, false, false);
	return;
}

void cMoveRelativeTime::Init(sPhysicsProperties* pObject, glm::vec3 endXYZ, double timeToMove, bool easeIn, bool easeOut)
{
	this->m_endXYZ = endXYZ;
	this->m_timeToMove = timeToMove;
	this->m_pObject = pObject;
	this->m_elapsedTime = 0.0;

	if (easeIn && !easeOut) 
	{
		m_moveType = MoveType::EasyIn;
	}
	else if (!easeIn && easeOut)
	{
		m_moveType = MoveType::EasyOut;
	}
	else if (easeIn && easeOut)
	{
		m_moveType = MoveType::EasyInAndOut;
	}
}

void cMoveRelativeTime::Start(void)
{
	m_startXYZ = m_pObject->position;
	
	m_velocity = glm::vec3(0.0f);

	glm::vec3 totalDistance = m_endXYZ - m_pObject->position;
	switch (m_moveType)
	{
	case MoveType::Linear:
		m_velocity = totalDistance / (float)m_timeToMove;
		m_pObject->velocity = m_velocity;
		break;

	case MoveType::EasyIn:
		m_pObject->velocity = m_velocity;
		break;	
	
	case MoveType::EasyOut:
		m_pObject->velocity = m_velocity;
		break;	
	
	case MoveType::EasyInAndOut:
		m_pObject->velocity = m_velocity;
		break;
	}

	return;
}

void cMoveRelativeTime::Update(double deltaTime)
{
	this->m_elapsedTime += deltaTime;

	double percentToDestiny = m_elapsedTime / m_timeToMove;
	glm::vec3 totalDistance = m_endXYZ - m_startXYZ;

	float easedPercent;

	switch (m_moveType)
	{
	case MoveType::Linear:
		break;

	case MoveType::EasyIn:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseIn(percentToDestiny);
			m_pObject->position = m_startXYZ + easedPercent * totalDistance;
		}
		else
		{
			m_velocity = glm::vec3(0.0f);
			m_pObject->velocity = m_velocity;
		}

		break;

	case MoveType::EasyOut:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseOut(percentToDestiny);
			m_pObject->position = m_startXYZ + easedPercent * totalDistance;
		}
		else
		{
			m_velocity = glm::vec3(0.0f);
			m_pObject->velocity = m_velocity;
		}

		break;

	case MoveType::EasyInAndOut:
		if (percentToDestiny < 1.0)
		{
			easedPercent = (float)glm::sineEaseInOut(percentToDestiny);
			m_pObject->position = m_startXYZ + easedPercent * totalDistance;
		}
		else
		{
			m_velocity = glm::vec3(0.0f);
			m_pObject->velocity = m_velocity;
		}

		break;
	}

	return;
}

bool cMoveRelativeTime::isFinished(void)
{
	if (this->m_elapsedTime >= this->m_timeToMove)
	{
		OnFinished();
		return true;
	}

	return false;
}	

void cMoveRelativeTime::OnFinished(void)
{
	m_pObject->velocity = glm::vec3(0.0f);
	m_pObject->acceleration = glm::vec3(0.0f);
}