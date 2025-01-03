#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

//TODO: replace draw positions and rotation for cTransform
class cTransform
{
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
	glm::quat m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec3 m_localPosition = glm::vec3(0.0f);
	glm::vec3 m_localScale = glm::vec3(1.0f);
	glm::quat m_localOrientation = glm::vec3(0.0f);

	cTransform* m_parentTransform = nullptr;
	std::vector<cTransform> m_childTransform;
public:
	cTransform();
	~cTransform();

	void SetPosition(glm::vec3 position);
	void SetScale(glm::vec3 scale);
	void SetOrientation(glm::quat orientation);

	void SetLocalPosition(glm::vec3 localPosition);
	void SetLocalScale(glm::vec3 scale);
	void SetLocalOrientation(glm::quat orientation);

	void SetUniformScale(float scale);
	void SetUniformLocalScale(float scale);

	void SetParent(cTransform& parent);

	void LookAt(glm::vec3 target, glm::vec3 upVector);

	glm::vec3 GetPosition();
	glm::vec3 GetLocalPosition();

	glm::quat GetOrientation();
	glm::quat GetLocalOrientation();

	glm::vec3 GetEulerRotation();
	glm::vec3 GetEulerLocalRotation();
};