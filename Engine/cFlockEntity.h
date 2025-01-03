#pragma once
#include "cGameObject.h"

struct sFlockEntityDef
{
	std::string Name;
	glm::vec3 Min = glm::vec3(0.0f);
	glm::vec3 Max = glm::vec3(0.0f);
	float NeighbourRadius = 0.0f;
	unsigned int FlockCount = 0;
	cMesh* Mesh;
};

class cFlockEntity : public cGameObject
{
	class cFlockMember
	{
	public:
		cFlockMember(cMesh* gameObject);
		~cFlockMember();

		void Flock(float separationFactor, float cohesionFactor, float alignmentFactor);

		cGameObject* gameObject;
		std::vector <cFlockMember*> neighbours;

		bool HasNeighbour(cFlockMember* neighbour);
		void LookAt(cFlockMember* neighbour);
	};

	void BuildNeighbourLists(cFlockMember* entety, cFlockMember* neighbour);

public:
	cFlockEntity(const sFlockEntityDef& def);
	~cFlockEntity();

	virtual void Update(double deltaTime) override;

	bool CheckIfPositionIsAvailable(float maxDistance, glm::vec3 position);

protected:
	std::vector<cFlockMember*> m_flockMembers;
	glm::vec3 m_minBoundary;
	glm::vec3 m_maxBoundary;
	float m_neighbourRadius2;
	int m_updateAfterNumSteps;
	int m_updateStepCounter;
};

