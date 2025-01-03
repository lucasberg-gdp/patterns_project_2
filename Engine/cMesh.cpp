
#include <iostream>			// For KillAllHumans() cout

#ifdef _DEBUG
#include "cGlobal.h"	 

#endif

#include "cGlobal.h"	 

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "OpenGLCommon.h"
#include "MathUtils.h"
#include "cMesh.h"
#include "cGameManager.h"
#include "cVAOManager/cVAOManager.h"

#include "cAnimationSystem.h"

#include "cColor.h"
#include "Engine.h"

extern cVAOManager* g_pMeshManager;

extern Engine engine;

// static
unsigned int cMesh::m_nextUniqueID = cMesh::FIRST_UNIQUE_ID;

extern cAnimationSystem* g_animationSystem;

// Constructor: Called on creation   c'tor
cMesh::cMesh()
{
	this->drawPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->setRotationFromEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	this->drawScale = glm::vec3(1.0f);

	this->bIsWireframe = false;
	this->bDoNotLight = false;

	this->bIsVisible = true;

	this->bUseDebugColours = false;
	this->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Set uniqueID
	this->m_UniqueID = cMesh::m_nextUniqueID;
	cMesh::m_nextUniqueID++;
}

cMesh::cMesh(cMesh* mesh)
{
	this->drawPosition = mesh->drawPosition;
	this->setRotationFromEuler(mesh->getDrawOrientation());
	this->meshName = mesh->meshName;
	this->drawScale = mesh->drawScale;
	this->bIsWireframe = mesh->bIsWireframe;
	this->bDoNotLight = mesh->bDoNotLight;
	this->bIsVisible = mesh->bIsVisible;
	this->bUseDebugColours = mesh->bUseDebugColours;
	this->wholeObjectDebugColourRGBA = mesh->wholeObjectDebugColourRGBA;
	this->bIsScaleUniform = true;


	// Set uniqueID
	this->m_UniqueID = cMesh::m_nextUniqueID;
	cMesh::m_nextUniqueID++;

	std::string name = objectType;
	unsigned int numberOfObjectOfAType = (unsigned int)cGameManager::GetGMInstance()->FindAllMeshesByObjectType(objectType).size() + 1;
	name = name + std::to_string(numberOfObjectOfAType);
	this->friendlyName = name;
}

// Destructor: Called on deletion   d'tor
cMesh::~cMesh()
{

}

cMesh::cMesh(std::string objectType)
{
	this->drawPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->setRotationFromEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	this->drawScale = glm::vec3(1.0f);

	this->bIsWireframe = false;
	this->bDoNotLight = false;
	this->objectType = objectType;

	this->bIsVisible = true;

	this->bUseDebugColours = false;
	this->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Set uniqueID
	this->m_UniqueID = cMesh::m_nextUniqueID;
	cMesh::m_nextUniqueID++;

	std::string name = objectType;
	unsigned int numberOfObjectOfAType = (unsigned int)cGameManager::GetGMInstance()->FindAllMeshesByObjectType(objectType).size() + 1;
	name = name + std::to_string(numberOfObjectOfAType);
	this->friendlyName = name;
}

//void cMesh::AddAnimation(std::string animationName)
//{
//	for (AnimationGroup* animationgroup : g_animationSystem->animationGroups)
//	{
//		if (animationgroup->name == animationName)
//		{
//			this->animationGroups.push_back(animationgroup);
//		}
//	}
//}
//
//void cMesh::SetAnimation(std::string animationName)
//{
//}

glm::vec3 removeScaleAndRotation(const glm::vec3& position, const glm::mat4& transformationMatrix)
{
	glm::mat3 rotationMatrix = glm::mat3(transformationMatrix);
	glm::mat3 inverseRotationMatrix = glm::inverse(rotationMatrix);

	glm::vec3 translation = glm::vec3(transformationMatrix[3]);
	glm::vec3 newPosition = inverseRotationMatrix * translation;

	return newPosition;
}

glm::vec3 addRotationAndScale(const glm::vec3& position, const glm::vec3& rotationAngles, const glm::vec3& scaleFactors) {
	// Create a transformation matrix with rotation and scale
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), scaleFactors);

	// Apply the transformation matrix to the position vector
	glm::vec4 newPosition = transformationMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Apply to origin

	return glm::vec3(newPosition);
}


float cMesh::IsAboveMesh(glm::vec3 position, bool& isAboveMesh)
{
	sModelDrawInfo drawInfo;
	::g_pMeshManager->FindDrawInfoByModelName(this->meshName, drawInfo);

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
	glm::mat4 rotationMatrix = glm::mat4_cast(GetOrientation());
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), drawScale);

	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	glm::vec3 newPosition = removeScaleAndRotation(position, transformationMatrix);

	newPosition = drawInfo.IsAboveModel(newPosition, isAboveMesh);

	glm::vec3 finalPosition = newPosition * drawScale;

	if (!isAboveMesh)
	{
		return finalPosition.y;
	}

	return 0.0f;
}

glm::vec3 cMesh::GetRandomPointOnSurface()
{
	sModelDrawInfo drawInfo;
	::g_pMeshManager->FindDrawInfoByModelName(this->meshName, drawInfo);

	glm::vec3 pointOnSurface = drawInfo.GetRandomPointOnSurface();

	glm::vec3 finalPosition = pointOnSurface * drawScale;

	engine.g_DrawDebugSphere(
		finalPosition,
		100.0f,
		cColor::COLOR_BLUE, 0.0f);

	return glm::vec3();
}

void cMesh::setUniformDrawScale(float scale)
{
	if (initialScale == glm::vec3(0.0f))
	{
		initialScale = glm::vec3(scale, scale, scale);
	}

	drawScale.x = drawScale.y = drawScale.z = scale;
}

void cMesh::SetScale(glm::vec3 scale)
{
	drawScale = scale;
}

// STARTOF: From: iPhysicsMeshTransformAccess interface
glm::vec3 cMesh::getDrawPosition(void)
{
	return this->drawPosition;
}

void cMesh::setDrawPosition(const glm::vec3& newPosition)
{
	this->drawPosition = newPosition;
	return;
}

glm::vec3 cMesh::getDrawOrientation(void)
{
//	return this->drawOrientation;

	// Be a little careful because quaterion to Euler can return unexpected things
	return glm::eulerAngles(this->GetOrientation());
}

glm::vec3 cMesh::GetScale() const
{
	return drawScale;
}

float cMesh::GetUniformScale() const
{
	if (bIsScaleUniform)
	{
		return this->drawScale.x;
	}

	return 0.0f;
}

std::vector<cMesh::Texture> cMesh::GetAllTextures()
{
	std::vector<Texture> textureVector;

	for (unsigned int i = 0; i < NUM_TEXTURES; i++)
	{
		if (!textureName[i].empty())
		{
			Texture texture(textureName[i], textureRatios[i]);

			textureVector.push_back(texture);
		}
	}

	return textureVector;
}

glm::vec3 cMesh::GetInitialScale()
{
	return initialScale;
}

void cMesh::setDrawOrientation(const glm::vec3& newOrientation)
{
	this->setRotationFromEuler(newOrientation);
}

void cMesh::setDrawOrientation(const glm::quat& newOrientation)
{
	this->m_qOrientation = newOrientation;
}

void cMesh::setTransformInfo(glm::vec3 transformInfo)
{
	this->drawPosition = transformInfo;
}

void cMesh::RotateAroundX(float degrees)
{
	float radians = glm::radians(degrees);
	drawOrientation.x += radians;
	this->setRotationFromEuler(drawOrientation);
}

void cMesh::RotateAroundY(float degrees)
{
	drawOrientation = getDrawOrientation();

	float radians = glm::radians(degrees);
	drawOrientation.y += radians;
	this->setRotationFromEuler(drawOrientation);

	drawOrientation = getDrawOrientation();
}

void cMesh::RotateAroundZ(float degrees)
{
	float radians = glm::radians(degrees);
	drawOrientation.z += radians;
	this->setRotationFromEuler(drawOrientation);
}

void cMesh::LookAt(glm::vec3 target)
{
	glm::vec3 direction = glm::normalize(target - drawPosition);
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 crosVector = glm::cross(upVector, direction);
	glm::mat3 matrix = glm::mat3(direction.z, -direction.y, -direction.x,
		0, 1, 0,
		direction.x, direction.y, direction.z);

	m_qOrientation = glm::quat(matrix);
}

glm::mat4 cMesh::GetRotation()
{
	return this->m_rotation;
}

std::string cMesh::GetFriendlyName()
{
	return friendlyName;
}

std::string cMesh::MeshInfo()
{
	// position
	std::string meshPositionX = MathUtils::RoundFloat(drawPosition.x, 2);
	std::string meshPositionY = MathUtils::RoundFloat(drawPosition.y, 2);
	std::string meshPositionZ = MathUtils::RoundFloat(drawPosition.z, 2);

	// orientation
	glm::quat meshOrientation = GetOrientation();

	std::string meshOrientationW = MathUtils::RoundFloat(meshOrientation.w, 2);
	std::string meshOrientationX = MathUtils::RoundFloat(meshOrientation.x, 2);
	std::string meshOrientationY = MathUtils::RoundFloat(meshOrientation.y, 2);
	std::string meshOrientationZ = MathUtils::RoundFloat(meshOrientation.z, 2);

	return "Object name: " + friendlyName + " Position: ("
		+ meshPositionX + ", "
		+ meshPositionY + ", "
		+ meshPositionZ + ") "
		+ "Orientation (w,x,y,z): ("
		+ meshOrientationW + ", "
		+ meshOrientationX + ", "
		+ meshOrientationY + ", "
		+ meshOrientationZ + ") ";

	return std::string();
}

float cMesh::GetVertexYByXAndZ(float xValue, float zValue)
{
	sModelDrawInfo drawInfo;
	::g_pMeshManager->FindDrawInfoByModelName(this->meshName, drawInfo);

	glm::vec3 position(xValue / drawScale.x , 0.0f, zValue / drawScale.z);

	// Rotation angle in radians
	float angle = glm::radians(-90.0f); // Convert degrees to radians

	// Axis of rotation (in this case, rotate around the z-axis)
	glm::vec3 axis(0.0f, 1.0f, 0.0f);

	// Create a rotation matrix
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);

	// Apply the rotation to the position
	glm::vec4 newPosition = rotationMatrix * glm::vec4(position, 1.0f); // Use a vec4 for homogeneous coordinates

	// Convert the result back to a vec3
	glm::vec3 rotatedPosition = glm::vec3(newPosition);

	float drawInfoHeight = drawInfo.GetYByXAndZ(rotatedPosition.x , rotatedPosition.z );

	//engine.g_DrawDebugSphere(glm::vec3(xValue, drawInfoHeight * drawScale.y, zValue), 10.0f, cColor::COLOR_RED, 0.0f);

	return drawInfoHeight * drawScale.y;
}
unsigned int cMesh::GetUniqueID(void)
{
	return this->m_UniqueID;
}

// ENDOF: iPhysicsMeshTransformAccess interface


//void cMesh::Update(double deltaTime)
//{
//#ifdef _DEBUG
////	::g_pDebugRenderer->AddSphere();
//#endif
//	return;
//}
