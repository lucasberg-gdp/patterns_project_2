#include "cFlockEntity.h"
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include "LogUtils.h"
#include "MathUtils.h"
#include "cScene.h"

extern cScene* g_currentScene;

void cFlockEntity::BuildNeighbourLists(cFlockMember* entity, cFlockMember* neighbour)
{
	// TODO: Get the distance between member A & B
	// is it less than the neighbour radius?
	// Yes->Add them to each other's lists 
	glm::vec3 posA(entity->gameObject->GetObjectMesh()->drawPosition);
	glm::vec3 posB(neighbour->gameObject->GetObjectMesh()->drawPosition);

	if (glm::length2(posB - posA) < m_neighbourRadius2)
	{
		entity->neighbours.push_back(neighbour);
		neighbour->neighbours.push_back(entity);
	}
}

cFlockEntity::cFlockEntity(const sFlockEntityDef& def) :
	cGameObject(def.Name, def.Mesh),
	m_minBoundary(def.Min),
	m_maxBoundary(def.Max),
	m_neighbourRadius2(def.NeighbourRadius * def.NeighbourRadius)
{
	m_flockMembers.resize(def.FlockCount);

	for (unsigned int i = 0; i < def.FlockCount; i++)
	{
		cMesh* condorMesh = new cMesh(def.Mesh);

		m_flockMembers[i] = new cFlockMember(condorMesh);
		// TODO: initialize position/orientations/etc...
		// Scatter them in random positions using the boundaries

		sPhysicsProperties* condorPhysics = sPhysicsProperties::CreateSphere(10.0f);
		condorPhysics->position = condorMesh->drawPosition;
		condorPhysics->pTheAssociatedMesh = condorMesh;
		condorPhysics->setRotationFromEuler(glm::vec3(0.0f));
		condorPhysics->maxVelocity = 70.0f;

		cGameObject* condorGameObject = new cGameObject(condorMesh, true);

		m_flockMembers[i]->gameObject = condorGameObject;
		m_flockMembers[i]->gameObject->m_objectPhysics = condorPhysics;

		bool isPositionValid = false;
		glm::vec3 position = MathUtils::GetRandomVec3(m_minBoundary, m_maxBoundary);

		while (!isPositionValid)
		{
			if (CheckIfPositionIsAvailable(40.0f, position))
			{
				isPositionValid = true;
			}
			else
			{
				position = MathUtils::GetRandomVec3(m_minBoundary, m_maxBoundary);
			}
		}

		condorPhysics->position = position;
		condorMesh->drawPosition = position;
		condorMesh->bIsVisible = true;
		condorMesh->isToSave = false;
		condorMesh->bDoNotLight = true;

		g_currentScene->AddMesh(condorMesh);
		g_currentScene->m_scenePhysics->AddShape(condorPhysics);
		//g_currentScene->AddGameObject(condorGameObject);
	}

	// TODO: Initilize the neighbour lists
	for (unsigned int i = 0; i < m_flockMembers.size(); i++)
	{
		for (unsigned int j = i + 1; j < m_flockMembers.size(); j++)
		{
			BuildNeighbourLists(m_flockMembers[i], m_flockMembers[j]);
		}
	}

}

cFlockEntity::~cFlockEntity()
{
	for (unsigned int i = 0; i < m_flockMembers.size(); i++)
	{
		delete m_flockMembers[i];
	}

	m_flockMembers.clear();
}

void cFlockEntity::Update(double deltaTime)
{
	// Clear the neighbour lists
	for (unsigned int i = 0; i < m_flockMembers.size(); i++)
	{
		m_flockMembers[i]->neighbours.clear();
	}

	// Update neighbour lists
	for (unsigned int i = 0; i < m_flockMembers.size(); i++)
	{
		for (unsigned int j = i + 1; j < m_flockMembers.size(); j++)
		{
			BuildNeighbourLists(m_flockMembers[i], m_flockMembers[j]);
		}
	}

	// Update behaviours
	for (unsigned int i = 0; i < m_flockMembers.size(); i++)
	{
		m_flockMembers[i]->Flock(1.0f,1.0f,1.0f);
	}
}

bool cFlockEntity::CheckIfPositionIsAvailable(float maxDistance, glm::vec3 position)
{
	for (unsigned int i = 0; i < m_flockMembers.size(); i++)
	{
		if (m_flockMembers[i] == nullptr)
		{
			return true;
		}

		float distance = glm::distance(position, m_flockMembers[i]->gameObject->m_objectPhysics->position);
		//std::cout << "distance: " << distance << std::endl;

		if (distance < maxDistance)
		{
			std::cout << "Too close" << std::endl;
			return false;
		}
	}

	return true;
}

cFlockEntity::cFlockMember::cFlockMember(cMesh* mesh)
{
	gameObject = new cGameObject("flock", mesh);
}

cFlockEntity::cFlockMember::~cFlockMember()
{
	delete gameObject;
}

void cFlockEntity::cFlockMember::Flock(float separationFactor, float cohesionFactor, float alignmentFactor)
{
	if (neighbours.size() == 0)
	{
		std::cout << "No neighbours!" << std::endl;
		return;
	}

	// TODO: flock!

	// Calculate a separation
	//	Look at all the neighbours

	glm::vec3 averagePosition;
	float numberOfNeighbours = (float)neighbours.size();

	for (unsigned int i = 0; i < numberOfNeighbours; i++)
	{
		averagePosition += neighbours[i]->gameObject->GetObjectMesh()->drawPosition;
	}

	//	Calculate an average position

	averagePosition /= numberOfNeighbours;

	cMesh* objectMesh = gameObject->GetObjectMesh();
	glm::vec3 oppositeDirection = averagePosition - objectMesh->drawPosition;

	//	Turn away from that average position!
	sPhysicsProperties* objectPhysics = gameObject->m_objectPhysics;

	if (objectPhysics)
	{
		//std::cout << "oppositeDirection: ";
		//LogUtils::Vector3Log(oppositeDirection);

		glm::vec3 normalizedVelocity = glm::normalize(oppositeDirection) * separationFactor;

		objectPhysics->velocity.x += normalizedVelocity.x;
		objectPhysics->velocity.z += normalizedVelocity.z;

		//objectPhysics->velocity = glm::normalize(objectPhysics->velocity);

		gameObject->FaceDirection(glm::vec3(objectPhysics->velocity.x, 0.0f, objectPhysics->velocity.z));
	}

	// Calculate a cohesion
	//	Look at all the neighbours
	//	Calculate an average position
	//	Turn toward that average position!
	averagePosition = glm::vec3(0.0f);

	for (unsigned int i = 0; i < numberOfNeighbours; i++)
	{
		averagePosition += neighbours[i]->gameObject->GetObjectMesh()->drawPosition;
	}

	averagePosition /= numberOfNeighbours;

	glm::vec3 cohesionDirection = objectMesh->drawPosition - averagePosition;

	if (objectPhysics)
	{
		//std::cout << "cohesionDirection: ";
		//LogUtils::Vector3Log(cohesionDirection);

		glm::vec3 normalizedVelocity = glm::normalize(cohesionDirection) * cohesionFactor;

		objectPhysics->velocity.x += normalizedVelocity.x;
		objectPhysics->velocity.z += normalizedVelocity.z;

		//objectPhysics->velocity = glm::normalize(objectPhysics->velocity);

		gameObject->FaceDirection(glm::vec3(objectPhysics->velocity.x, 0.0f, objectPhysics->velocity.z));
	}

	// Calculate an alignment
	//	Look at all the neighbours
	//	Calculate an average forward direction
	//	Turn toward that forward direction!
	glm::vec3 averageForward = glm::vec3(0.0f);

	for (unsigned int i = 0; i < numberOfNeighbours; i++)
	{
		averageForward += neighbours[i]->gameObject->m_objectPhysics->GetForward();
	}
	averageForward /= numberOfNeighbours;

	if (objectPhysics)
	{
		//std::cout << "alignment: ";
		LogUtils::Vector3Log(averageForward);

		glm::vec3 normalizedVelocity = glm::normalize(glm::normalize(averageForward) * alignmentFactor);

		objectPhysics->velocity.x += normalizedVelocity.x;
		objectPhysics->velocity.z += normalizedVelocity.z;

		gameObject->FaceDirection(glm::vec3(objectPhysics->velocity.x, 0.0f, objectPhysics->velocity.z));
	}

}

bool cFlockEntity::cFlockMember::HasNeighbour(cFlockMember* neighbour)
{
	for (cFlockMember* flockEntity : this->neighbours)
	{
		if (flockEntity == neighbour)
		{
			return true;
		}
	}

	return false;
}

void cFlockEntity::cFlockMember::LookAt(cFlockMember* neighbour)
{

}
