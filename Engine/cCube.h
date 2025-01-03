#pragma once
#include "cMesh.h"
#include "sPhysicsProperties.h"
#include "cGameObject.h"

class cCube : public cGameObject
{
private:
	bool m_isUsingPhysics = false;
	unsigned int m_cubeUniqueID;
	static const unsigned int CUBE_FIRST_UNIQUE_ID = 1;
	static unsigned int m_nextcubeUniqueID;
public:
	cCube();
	~cCube();

	cCube(std::string friendlyName);

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
