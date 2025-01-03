#include "cAnimation.h"

cAnimation::cAnimation(glm::vec3 start, glm::vec3 end, EasingType type)
{
	//m_startPoint = start;
	//m_endPoint = end;
	//m_easingType = type;
}

cAnimation::cAnimation(const cAnimation& animation)
{
	m_positionKeyFrames = animation.m_positionKeyFrames;
	m_rotationKeyFrames = animation.m_rotationKeyFrames;
	m_scaleKeyFrames = animation.m_scaleKeyFrames;
	m_time = animation.m_time;
	m_percent = animation.m_percent;
	m_isAnimRunning = animation.m_isAnimRunning;
}

