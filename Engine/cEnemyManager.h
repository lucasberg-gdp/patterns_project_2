#pragma once
#include "cGameObject.h"
#include "cEnemy.h"

class cEnemyManager
{
	// Project 2
	// Grazing around a point
	void WanderGrazing(cEnemy& gameObject, double time) const;

	// Move collectively with equal elements
	void WanderFlocking(cEnemy& gameObject, double time) const;

	// Move collectively keeping some in the center
	void WanderHerding(cEnemy& gameObject, double time) const;

	// Project 3
	void Flock(cEnemy& gameObject, double time) const;

	// Project 4
	void CreatePath(unsigned int numberOfPoints, glm::vec3 minPosition, glm::vec3 maxPosition);
	void FollowPath(cEnemy& gameObject, double time);

	bool IsPlayerInsideInnerCircle();
	bool IsPlayerInsideOuterCircle();


	cGameObject* player;

	void AssignPlayer(cGameObject* playerObject);

	float innerRadius = 30.0f;
	float outerRadius = 60.0f;

	std::vector<glm::vec3> points;
	glm::vec3 minPosition = glm::vec3(0.0f);
	glm::vec3 maxPosition = glm::vec3(0.0f);

	void Wander(cGameObject& gameObject) const;
	void Seek(cGameObject& gameObject) const;
	void Flee(cGameObject& gameObject) const;
	void Pursue(cGameObject& gameObject);
	void Evade(cGameObject& gameObject) const;
	void Approach(cGameObject& gameObject) const;

	float m_approachRadius = 5.0f;
	float m_seekRadius = 1.0f;
	float m_evadeRadius = 5.0f;
	float m_fleeDistance = 5.0f;

	glm::vec3 flockCenter = glm::vec3(-100.0f, 1.4f, 75.0f);
	float flockMinX = -100.0f;
	float flockMaxX = 100.0f;
	float flockSpeed = 1.0f;
	bool isFlockMovingLeft = true;

	glm::vec3 herdCenter = glm::vec3(-100.0f, 1.4f, 10.0f);
	float herdMinX = -100.0f;
	float herdMaxX = 100.0f;
	float herdSpeed = 1.0f;
	bool isHerdMovingLeft = true;

	void UpdateFlockPosition(double deltaTime);
	void UpdateHerdPosition(double deltaTime);
public:


	void Init();

	void Update(double deltaTime);


	
};

