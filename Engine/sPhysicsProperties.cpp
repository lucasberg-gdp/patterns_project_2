#include "sPhysicsProperties.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "MathUtils.h"

sPhysicsProperties::sPhysicsProperties()
{
	m_qOrientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
	this->inverse_mass = 1.0f;
	this->pTheAssociatedMesh = NULL;
	this->shapeType = UNKNOWN_OR_UNDEFINED;
	this->m_UniqueID = sPhysicsProperties::m_nextUniqueID;
	sPhysicsProperties::m_nextUniqueID++;

}

sPhysicsProperties::sPhysicsProperties(eShape shapeType)
{
	m_qOrientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
	this->inverse_mass = 1.0f;
	this->pTheAssociatedMesh = NULL;
	this->shapeType = shapeType;
	this->m_UniqueID = sPhysicsProperties::m_nextUniqueID;
	sPhysicsProperties::m_nextUniqueID++;
}

std::string sPhysicsProperties::getShapeTypeAsString(void)
{
	switch (this->shapeType)
	{
	case SPHERE:
		return "SPHERE";
		break;
	case PLANE:
		return "PLANE";
		break;
	case TRIANGLE:
		return "TRIANGLE";
		break;
	case AABB:
		return "AABB";
		break;
	case CAPSULE:
		return "CAPSULE";
		break;
	case MESH_OF_TRIANGLES_INDIRECT:
		return "MESH_OF_TRIANGLES_INDIRECT";
		break;
	case MESH_OF_TRIANGLES_LOCAL_VERTICES:
		return "MESH_OF_TRIANGLES_LOCAL_VERTICES";
		break;
	case UNKNOWN_OR_UNDEFINED:
		return "UNKNOWN_OR_UNDEFINED";
		break;
	}

	// ERROR: UNKNOWN_OR_UNDEFINED
	return "ERROR: Invalid shape type";
}


unsigned int sPhysicsProperties::getUniqueID(void)
{
	return this->m_UniqueID;
}

glm::vec3 sPhysicsProperties::SumOfForces()
{
	glm::vec3 finalAcceleration = glm::vec3(0.0f,0.0f,0.0f);

	std::map<std::string, glm::vec3>::iterator allForcesIterator;
	for (allForcesIterator = forcesAppliedToObject.begin(); allForcesIterator != forcesAppliedToObject.end(); ++allForcesIterator)
	{
		finalAcceleration += allForcesIterator->second;
	}

	return finalAcceleration;
}

bool sPhysicsProperties::RemoveForce(std::string friendlyName)
{
	if (forcesAppliedToObject.erase(friendlyName) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void sPhysicsProperties::ClearForces()
{
	forcesAppliedToObject.clear();
}

void sPhysicsProperties::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

void sPhysicsProperties::SetGravity(float gravity)
{
	m_gravity = gravity;
}

float sPhysicsProperties::GetGravity()
{
	return m_gravity;
}

const std::string sPhysicsProperties::PhysicsInfo()
{
	return "Position: (" 
		+ MathUtils::RoundFloat(position.x, 2) + ","
		+ MathUtils::RoundFloat(position.y, 2) + ","
		+ MathUtils::RoundFloat(position.z, 2) + ")\n"
		"Velocity: ( "
		+ MathUtils::RoundFloat(velocity.x, 2) + ","
		+ MathUtils::RoundFloat(velocity.y, 2) + ","
		+ MathUtils::RoundFloat(velocity.z, 2) + ")\n";
}

void sPhysicsProperties::AssociateMesh(iPhysicsMeshTransformAccess* mesh)
{
	pTheAssociatedMesh = mesh;

	// = pTheAssociatedMesh->getDrawPosition();
}

glm::vec3 sPhysicsProperties::GetForward()
{
	return glm::toMat3(m_qOrientation) * glm::vec3(0.f, 0.f, 1.f);
}

sPhysicsProperties* sPhysicsProperties::CreateSphere(float radius)
{
	sPhysicsProperties* spherePhyics = new sPhysicsProperties();
	spherePhyics->shapeType = sPhysicsProperties::SPHERE;
	spherePhyics->setShape(new sPhysicsProperties::sSphere(radius));
	spherePhyics->setRotationFromEuler(glm::vec3(0.0f));
	spherePhyics->friendlyName = "sphere";
	spherePhyics->objectType = "sphere";

	return spherePhyics;
}

void sPhysicsProperties::StopPhysicsObject()
{
	velocity = glm::vec3(0.0f);
	acceleration = glm::vec3(0.0f);
	rotationSpeed = glm::vec3(0.0f);
	rotationAcceleration = glm::vec3(0.0f);
}

//static 
unsigned int sPhysicsProperties::m_nextUniqueID = sPhysicsProperties::FIRST_UNIQUE_ID;

void sPhysicsProperties::setShape(sSphere* pSphereProps)
{
	this->pShape = (void*)pSphereProps;
	return;
}

void sPhysicsProperties::setShape(sPlane* pPlaneProps)
{
	this->pShape = (void*)pPlaneProps;
	return;
}

void sPhysicsProperties::setShape(sTriangle* pTriangleProps)
{
	this->pShape = (void*)pTriangleProps;
	return;
}

void sPhysicsProperties::setShape(sCapsule* pCapsuleProps)
{
	this->pShape = (void*)pCapsuleProps;
	return;
}

void sPhysicsProperties::setShape(sMeshOfTriangles_Indirect* pTriangleMeshProps)
{
	this->pShape = (void*)pTriangleMeshProps;
	return;
}

void sPhysicsProperties::setShape(sMeshOfTriangles_LocalVertices* pTriangleMeshProps)
{
	this->pShape = (void*)pTriangleMeshProps;
	return;
}
