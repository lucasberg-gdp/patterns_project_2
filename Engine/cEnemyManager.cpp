#include "cEnemyManager.h"
#include <vector>
#include "cLionPlayer.h"
#include <iostream>
#include <glm/gtc/random.hpp>

#include "cScene.h"
#include "cGameManager.h"
#include "Engine.h"
#include "cColor.h"
#include "MathUtils.h"

//extern std::vector< cGameObject* > g_gameObjects;
extern cLionPlayer* lionPlayer;
extern cScene* g_currentScene;
extern Engine engine;

void cEnemyManager::WanderGrazing(cEnemy& enemy, double time) const
{
	enemy.m_wanderDecisionCountdown -= time;

	if (enemy.m_wanderDecisionCountdown < 0.0f)
	{
		// Decision time!

		enemy.m_wanderDecisionCountdown = enemy.m_wanderDecisionTime;

		sPhysicsProperties* enemyPhysics = enemy.GetObjectPhysics();

		glm::vec3 direction(enemyPhysics->GetForward());

		glm::vec3 wanderPoint(enemyPhysics->position + direction * (float)enemy.m_wanderRadius1);

		float theta = glm::linearRand(0.0f, glm::two_pi<float>());

		float x = (float)enemy.m_wanderRadius2 * glm::cos(theta);
		float z = (float)enemy.m_wanderRadius2 * glm::sin(theta);

		wanderPoint.x += x;
		wanderPoint.z += z;

		// Set the new wander direction
		enemy.m_wanderTargetDirection = glm::normalize(wanderPoint - enemyPhysics->position);
	}

	enemy.FaceDirection(enemy.m_wanderTargetDirection);
	enemy.GetObjectPhysics()->velocity = enemy.GetObjectPhysics()->GetForward() * enemy.GetObjectPhysics()->maxVelocity;
}

void cEnemyManager::WanderFlocking(cEnemy& enemy, double time) const
{
	enemy.m_wanderDecisionCountdown -= (time / 10.0);

	if (enemy.m_wanderDecisionCountdown < 0.0f)
	{
		enemy.m_wanderDecisionCountdown = enemy.m_wanderDecisionTime;

		sPhysicsProperties* enemyPhysics = enemy.GetObjectPhysics();
		glm::vec3 direction(enemyPhysics->GetForward());
		glm::vec3 wanderPoint(enemyPhysics->position + direction * (float)enemy.m_wanderRadius1);

		wanderPoint = flockCenter;

		float theta = glm::linearRand(0.0f, glm::two_pi<float>());

		float x = (float)enemy.m_wanderRadius2 * glm::cos(theta);
		float z = (float)enemy.m_wanderRadius2 * glm::sin(theta);

		wanderPoint.x += x;
		wanderPoint.z += z;

		// Set the new wander direction
		enemy.m_wanderTargetDirection = glm::normalize(wanderPoint - enemyPhysics->position);
	}
	enemy.FaceDirection(enemy.m_wanderTargetDirection);
	enemy.GetObjectPhysics()->velocity = enemy.GetObjectPhysics()->GetForward() * enemy.GetObjectPhysics()->maxVelocity;
}

void cEnemyManager::WanderHerding(cEnemy& enemy, double time) const
{
	enemy.m_wanderDecisionCountdown -= (time / 10.0);

	if (enemy.m_wanderDecisionCountdown < 0.0f)
	{
		if (enemy.tag == "bisoncalf")
		{
			enemy.m_wanderDecisionCountdown = enemy.m_wanderDecisionTime;
			sPhysicsProperties* enemyPhysics = enemy.GetObjectPhysics();
			glm::vec3 direction(enemyPhysics->GetForward());
			glm::vec3 wanderPoint(enemyPhysics->position + direction * (float)enemy.m_wanderRadius1);

			wanderPoint = herdCenter;

			float theta = glm::linearRand(0.0f, glm::two_pi<float>());

			float x = (float)enemy.m_wanderRadius2 * glm::cos(theta);
			float z = (float)enemy.m_wanderRadius2 * glm::sin(theta);

			wanderPoint.x += x;
			wanderPoint.z += z;

			// Set the new wander direction
			enemy.m_wanderTargetDirection = glm::normalize(wanderPoint - enemyPhysics->position);
		}
		else
		{
			enemy.m_wanderDecisionCountdown = enemy.m_wanderDecisionTime;
			sPhysicsProperties* enemyPhysics = enemy.GetObjectPhysics();
			glm::vec3 direction(enemyPhysics->GetForward());
			glm::vec3 wanderPoint(enemyPhysics->position + direction * (float)enemy.m_wanderRadius1);
			wanderPoint = cGameManager::GetGMInstance()->FindMeshByFriendlyName("bisoncalf")->drawPosition;
			float theta = glm::linearRand(0.0f, glm::two_pi<float>());
			float x = (float)(enemy.m_wanderRadius2 * glm::cos(theta));
			float z = (float)(enemy.m_wanderRadius2 * glm::sin(theta));
			wanderPoint.x += x;
			wanderPoint.z += z;

			// Set the new wander direction
			enemy.m_wanderTargetDirection = glm::normalize(wanderPoint - enemyPhysics->position);
		}
	}

	enemy.FaceDirection(enemy.m_wanderTargetDirection);
	enemy.GetObjectPhysics()->velocity = enemy.GetObjectPhysics()->GetForward() * enemy.GetObjectPhysics()->maxVelocity;
}

void cEnemyManager::Flock(cEnemy& gameObject, double time) const
{



}

void cEnemyManager::CreatePath(unsigned int numberOfPoints, glm::vec3 minPosition, glm::vec3 maxPosition)
{
	for (unsigned int i = 0; i < numberOfPoints; i++)
	{
		glm::vec3 pathPoint = minPosition;

		glm::vec3 vectorIncrement = (maxPosition - minPosition) / (float)(numberOfPoints - 1) * (float)i;

		pathPoint += vectorIncrement;
		points.push_back(pathPoint);
	}
}

void cEnemyManager::FollowPath(cEnemy& enemy, double time)
{
	float shortestDistance = std::numeric_limits<float>::max();

	float pathRadius = 0.0f; // defined with the path

	glm::vec3 futurePosition; // Determined using current position and velocity

	glm::vec3 objectVelocity = enemy.GetObjectPhysics()->velocity;

	futurePosition = enemy.GetObjectMesh()->drawPosition + objectVelocity;

	glm::vec3 targetPosition = glm::vec3(0.0f); // to be determined below

	//loop through all the points on the path
	for (unsigned int i = 0; i < (points.size() - 1); i++)
	{
		// look at each segment
		glm::vec3 a = points[i];

		const unsigned int nextIndex = i + 1;

		glm::vec3 b = points[nextIndex];


		float t(0.0f);
		glm::vec3 closestPoint = MathUtils::ClosestPoint(a, b, futurePosition, t);

		float distance = glm::distance(futurePosition, closestPoint);

		if (distance < shortestDistance)
		{
			shortestDistance = distance;
			targetPosition = closestPoint;

			// move the target position along the line an amount scaled by the distance
			// also scaling by velocity might be a good idea
			targetPosition += glm::normalize(b - a) * shortestDistance;

			enemy.target = targetPosition;
		}
	}

	// All said and done we only want to do anything 
	// if we are projected to be outside the line's radius
	if (shortestDistance > pathRadius)
	{
		if (IsPlayerInsideInnerCircle())
		{
			//std::cout << "IsPlayerInsideInnerCircle" << std::endl;
			enemy.target = player->GetObjectMesh()->drawPosition;
			Pursue(enemy);
		}
		else if (IsPlayerInsideOuterCircle())
		{
			//std::cout << "IsPlayerInsideOuterCircle" << std::endl;
			Wander(enemy);
			enemy.FaceDirection(player->GetObjectMesh()->drawPosition - enemy.GetObjectMesh()->drawPosition);
		}
		else
		{
			// Do the seek thing
			// seek the targetPosition
			Seek(enemy);
		}
	}
	else
	{
		Wander(enemy);
	}
}

bool cEnemyManager::IsPlayerInsideInnerCircle()
{
	float squaredDistance = glm::dot(player->GetObjectMesh()->drawPosition, player->GetObjectMesh()->drawPosition);
	return squaredDistance <= (innerRadius * innerRadius);
}

bool cEnemyManager::IsPlayerInsideOuterCircle()
{
	float squaredDistance = glm::dot(player->GetObjectMesh()->drawPosition, player->GetObjectMesh()->drawPosition);
	return squaredDistance <= (outerRadius * outerRadius);
}


void cEnemyManager::AssignPlayer(cGameObject* playerObject)
{
	player = playerObject;
}

void cEnemyManager::Wander(cGameObject& gameObject) const
{
	gameObject.GetObjectPhysics()->velocity = glm::vec3(0.0f);
}

void cEnemyManager::Seek(cGameObject& gameObject) const
{
	glm::vec3 gameObjectTarget = gameObject.target;

	glm::vec3 targetDirection = gameObjectTarget - gameObject.GetObjectPhysics()->position;

	targetDirection = glm::normalize(glm::vec3(
		gameObjectTarget.x - gameObject.GetObjectMesh()->drawPosition.x,
		0.0f,
		gameObjectTarget.z - gameObject.GetObjectMesh()->drawPosition.z
	));

	gameObject.GetObjectPhysics()->velocity = targetDirection * gameObject.objectSpeed;
	gameObject.FaceDirection(targetDirection);
}

void cEnemyManager::Flee(cGameObject& gameObject) const
{
	if (!lionPlayer)
	{
		return;
	}

	if (glm::distance(gameObject.GetObjectMesh()->drawPosition, lionPlayer->GetObjectMesh()->drawPosition) < m_fleeDistance)
	{
		glm::vec3 oppositeDirection = gameObject.GetObjectPhysics()->position
			- lionPlayer->GetObjectMesh()->drawPosition;

		oppositeDirection = glm::normalize(glm::vec3(
			gameObject.GetObjectMesh()->drawPosition.x - lionPlayer->GetObjectMesh()->drawPosition.x,
			0.0f,
			gameObject.GetObjectMesh()->drawPosition.z - lionPlayer->GetObjectMesh()->drawPosition.z
		));

		gameObject.GetObjectPhysics()->velocity = oppositeDirection;
		gameObject.FaceDirection(oppositeDirection);
	}
	else
	{
		gameObject.GetObjectPhysics()->velocity = glm::vec3(0.0f);
	}
}

void cEnemyManager::Pursue(cGameObject& gameObject)
{
	glm::vec3 targetPosition = gameObject.target;

	glm::vec3 targetDirection = targetPosition - gameObject.GetObjectPhysics()->position;

	glm::vec3 futurePosition = targetPosition +
		player->m_objectPhysics->velocity * 5.0f;

	targetDirection = glm::normalize(glm::vec3(
		futurePosition.x - gameObject.GetObjectMesh()->drawPosition.x,
		0.0f,
		futurePosition.z - gameObject.GetObjectMesh()->drawPosition.z
	));

	glm::vec3 lionVelocity = player->GetObjectPhysics()->velocity;

	gameObject.GetObjectPhysics()->velocity = targetDirection * gameObject.objectSpeed;
	gameObject.FaceDirection(targetDirection);
}

void cEnemyManager::Evade(cGameObject& gameObject) const
{
	glm::vec3 futurePosition = lionPlayer->GetObjectMesh()->drawPosition +
		lionPlayer->GetObjectPhysics()->velocity * 5.0f;

	if (glm::distance(lionPlayer->GetObjectMesh()->drawPosition, gameObject.GetObjectMesh()->drawPosition) < m_evadeRadius)
	{
		glm::vec3 oppositeDirection = gameObject.GetObjectPhysics()->position
			- lionPlayer->GetObjectMesh()->drawPosition;

		oppositeDirection = glm::normalize(glm::vec3(
			gameObject.GetObjectMesh()->drawPosition.x - lionPlayer->GetObjectMesh()->drawPosition.x,
			0.0f,
			gameObject.GetObjectMesh()->drawPosition.z - lionPlayer->GetObjectMesh()->drawPosition.z
		));

		gameObject.GetObjectPhysics()->velocity = oppositeDirection;
		gameObject.FaceDirection(oppositeDirection);
	}
	else if (glm::distance(futurePosition, gameObject.GetObjectMesh()->drawPosition) < m_evadeRadius)
	{
		glm::vec3 lionDirection = lionPlayer->GetObjectMesh()->drawPosition - gameObject.GetObjectPhysics()->position;

		glm::vec3 normalToObject = glm::cross(gameObject.GetObjectMesh()->drawPosition, futurePosition);
		normalToObject.y = 0.0f;

		normalToObject = glm::normalize(normalToObject);

		gameObject.GetObjectPhysics()->velocity = normalToObject;
		gameObject.FaceDirection(normalToObject);
	}
	else
	{
		gameObject.GetObjectPhysics()->velocity = glm::vec3(0.0f);
	}
}

void cEnemyManager::Approach(cGameObject& gameObject) const
{
	glm::vec3 lionDirection = lionPlayer->GetObjectMesh()->drawPosition - gameObject.GetObjectPhysics()->position;

	lionDirection = glm::normalize(glm::vec3(
		lionPlayer->GetObjectMesh()->drawPosition.x - gameObject.GetObjectMesh()->drawPosition.x,
		0.0f,
		lionPlayer->GetObjectMesh()->drawPosition.z - gameObject.GetObjectMesh()->drawPosition.z
	));

	if (glm::distance(gameObject.GetObjectMesh()->drawPosition, lionPlayer->GetObjectMesh()->drawPosition) > m_approachRadius)
	{
		gameObject.GetObjectPhysics()->velocity = lionDirection;
	}
	else
	{
		Wander(gameObject);
	}

	gameObject.FaceDirection(lionDirection);
}

void cEnemyManager::UpdateFlockPosition(double deltaTime)
{
}

void cEnemyManager::UpdateHerdPosition(double deltaTime)
{
}

void cEnemyManager::Init()
{
	CreatePath(5, glm::vec3(0.0f, 0.0f, -70.0f), glm::vec3(0.0f, 0.0f, 70.0f));
}

void cEnemyManager::Update(double time)
{
	//// Show path points
	//for (unsigned int i = 0; i < points.size(); i++)
	//{
	//	engine.g_DrawDebugSphere(points[i], 1.0f, cColor::COLOR_BLUE, 0.0f);
	//}

	//// Show inner radius
	//engine.g_DrawDebugSphere(glm::vec3(0.0f), innerRadius, cColor::COLOR_RED, time);


	//// Show outer radius
	//engine.g_DrawDebugSphere(glm::vec3(0.0f), outerRadius, cColor::COLOR_WHITE, time);


	if (!player)
	{
		player = g_currentScene->GetPlayer();
	}

	if (!g_currentScene->isScenePlaying)
	{
		return;
	}

	for (unsigned int i = 0; i < g_currentScene->g_enemies.size(); i++)
	{
		switch (g_currentScene->g_enemies[i]->enemyState)
		{
		case cEnemy::EnemyState::Wandering:
			Wander(*g_currentScene->g_enemies[i]);
			break;

		case cEnemy::EnemyState::FollowingPath:
			FollowPath(*g_currentScene->g_enemies[i], time);
			break;

		case cEnemy::EnemyState::Seeking:
			Seek(*g_currentScene->g_enemies[i]);
			break;
		}
	}
}

