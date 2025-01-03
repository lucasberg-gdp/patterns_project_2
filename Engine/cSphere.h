#pragma once
#include "cMesh.h"
#include "sPhysicsProperties.h"
#include "cGameObject.h"

class cSphere : public cGameObject
{
private:
	bool m_isUsingPhysics = false;
	unsigned int m_sphereUniqueID;
	static const unsigned int SPHERE_FIRST_UNIQUE_ID = 1;
	static unsigned int m_nextsphereUniqueID;
public:
	cSphere();
	~cSphere();

	cSphere(std::string friendlyName);

	glm::vec3 GetPosition();
	glm::vec3 GetScale();

	// objectType + ID
	std::string GetFriendlyName();

	void SetPosition(glm::vec3 position);
	void SetScale(float uniformScale);
	void SetScale(glm::vec3 scale);

	void IsUsingPhysics(bool isUsingPhysics);

	void Update(double deltaTime) override;
};
