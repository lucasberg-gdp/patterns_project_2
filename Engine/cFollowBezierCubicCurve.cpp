#include "cFollowBezierCubicCurve.h"

void cFollowBezierCubicCurve::Init(sPhysicsProperties* pMeshToControl, std::vector<glm::vec3> controlPoints, float timeToMove)
{
	m_ControlPoints = controlPoints;
	m_TimeToMove = timeToMove;
	m_endXYZ = controlPoints[controlPoints.size() - 1];

	m_physicsToMove = pMeshToControl;
	m_ElapsedTime = 0.0;

	return;
}

glm::vec3 cFollowBezierCubicCurve::GetBezierPosition()
{
    double t = m_ElapsedTime / m_TimeToMove;

    double u = 1.0 - t;

    glm::vec3 bezierPosition =
        (float)(u * u * u) * m_ControlPoints[0] +                    // (1-t)^3 * P0
        (float)(3 * u * u * t) * m_ControlPoints[1] +                // 3(1-t)^2 * t * P1
        (float)(3 * u * t * t) * m_ControlPoints[2] +                // 3(1-t) * t^2 * P2
        (float)(t * t * t) * m_ControlPoints[3];                     // t^3 * P3

    return bezierPosition;
}

void cFollowBezierCubicCurve::Start(void)
{

}

void cFollowBezierCubicCurve::Update(double deltaTime)
{
	this->m_ElapsedTime += deltaTime;



	m_physicsToMove->position = GetBezierPosition();


	return;
}

bool cFollowBezierCubicCurve::isFinished(void)
{
	// Has the amount of time passed
	if (this->m_ElapsedTime >= this->m_TimeToMove)
	{
		OnFinished();
		return true;
	}

	return false;
}

void cFollowBezierCubicCurve::OnFinished(void)
{
	m_physicsToMove->velocity = glm::vec3(0.0f);
	m_physicsToMove->acceleration = glm::vec3(0.0f);
	return;
}
