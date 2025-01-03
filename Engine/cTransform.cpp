#include <glm/vec3.hpp>

#include "cTransform.h"

cTransform::cTransform()
{

}

cTransform::~cTransform()
{

}

void cTransform::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void cTransform::SetScale(glm::vec3 scale)
{
	m_scale = scale;
}

void cTransform::SetOrientation(glm::quat orientation)
{
	m_orientation = orientation;
}

void cTransform::SetLocalPosition(glm::vec3 localPosition)
{
	m_localPosition = localPosition;
}

void cTransform::SetUniformScale(float scale)
{
	m_scale.x = scale;
	m_scale.y = scale;
	m_scale.z = scale;
}

void cTransform::SetLocalOrientation(glm::quat orientation)
{
	m_orientation = orientation;
}

void cTransform::SetLocalScale(glm::vec3 scale)
{
	m_scale = scale;
}

void cTransform::LookAt(glm::vec3 target, glm::vec3 upVector)
{
	glm::vec3 direction = glm::normalize(target - m_position);
	glm::vec3 crosVector = glm::cross(upVector, direction);
	glm::mat3 matrix = glm::mat3(direction.z, -direction.y, -direction.x,
								 0, 1, 0,
								 direction.x, direction.y, direction.z);

	m_orientation = glm::quat(matrix);

	for (int i = 0; i < m_childTransform.size(); i++)
	{
		m_childTransform[i].SetLocalOrientation(m_orientation);
	}
}

glm::vec3 cTransform::GetPosition()
{
	return m_position;
}

glm::vec3 cTransform::GetLocalPosition()
{
	return m_localPosition;
}

glm::quat cTransform::GetOrientation()
{
	return m_orientation;
}

glm::quat cTransform::GetLocalOrientation()
{
	return m_localOrientation;
}

void cTransform::SetUniformLocalScale(float scale)
{
	m_localScale = glm::vec3(scale);
}

void cTransform::SetParent(cTransform& parent)
{
	*m_parentTransform = parent;
}

glm::vec3 cTransform::GetEulerRotation()
{
	return glm::eulerAngles(GetOrientation());
}

glm::vec3 cTransform::GetEulerLocalRotation()
{
	return glm::eulerAngles(GetLocalOrientation());
}
