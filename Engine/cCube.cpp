#include "cCube.h"
#include "cScene.h"

extern cScene* g_currentScene;

// static
unsigned int cCube::m_nextcubeUniqueID = cCube::CUBE_FIRST_UNIQUE_ID;

cCube::cCube()
{
}

cCube::~cCube()
{
}

cCube::cCube(std::string friendlyName)
{
	// Set uniqueID
	this->m_cubeUniqueID = cCube::m_nextcubeUniqueID;
	cCube::m_nextcubeUniqueID++;

	cMesh* cubeMesh = GetObjectMesh();
	cubeMesh->bIsVisible = true;
	cubeMesh->drawPosition = glm::vec3(0.0f);
	cubeMesh->bUseDebugColours = true;
	cubeMesh->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	cubeMesh->setUniformDrawScale(1.0f);
	cubeMesh->objectType = "cube";
	cubeMesh->drawScale = glm::vec3(1.0f);

	std::string name = friendlyName;
	name += this->m_cubeUniqueID;

	cubeMesh->friendlyName = name;
	cubeMesh->meshName = "shapes/Cube_1x1x1_xyz_n_rgba_uv.ply";

	sPhysicsProperties* cubePhysics = GetObjectPhysics();
	cubePhysics->pTheAssociatedMesh = cubeMesh;
	cubePhysics->shapeType = sPhysicsProperties::SPHERE;
	cubePhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
	cubePhysics->setRotationFromEuler(cubeMesh->drawOrientation);
	cubePhysics->position = cubeMesh->drawPosition;
	cubePhysics->friendlyName = cubeMesh->friendlyName;
	cubePhysics->objectType = cubeMesh->objectType;
	g_currentScene->m_scenePhysics->AddShape(cubePhysics);
}

glm::vec3 cCube::GetPosition()
{
    return glm::vec3();
}

glm::vec3 cCube::GetScale()
{
    return glm::vec3();
}

std::string cCube::GetFriendlyName()
{
    return std::string();
}

void cCube::SetPosition(glm::vec3 position)
{
}

void cCube::SetScale(float uniformScale)
{
}

void cCube::SetScale(glm::vec3 scale)
{
}

void cCube::IsUsingPhysics(bool isUsingPhysics)
{
}

void cCube::Update(double deltaTime)
{
}
