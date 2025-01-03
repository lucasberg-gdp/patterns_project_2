#pragma once

#include "cGameObject.h"

class cEnemy : public cGameObject
{

	void MoveTowards(glm::vec3 direction);
	void AdjustOrientation(glm::vec3 eulerOrientation);
public:

	enum EnemyState
	{
		Wandering,
		Seeking,
		Fleeing,
		Pursuing,
		Evading,
		Approaching,
		FollowingPath
	};

	float horizontalMovement = 0.0f;
	float verticalMovement = 0.0f;

	cGameObject* enemyTarget = nullptr;

	float mArrivalRadius = 0.0f;

	glm::vec3 m_wanderTargetDirection = glm::vec3(0.0f);
	double m_wanderRadius1 = 0.0f;
	double m_wanderRadius2 = 0.0f;
	double m_wanderDecisionTime = 0.0f;
	double m_wanderDecisionCountdown = 0.0f;

	cEnemy(cMesh* mesh);

	void SetHorizontalMovement(float horizontal);
	void SetVerticalMovement(float vertical);

	void Update(double deltaTime) override;

	EnemyState enemyState = EnemyState::Wandering;
};
