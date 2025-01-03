#include "cSphere.h"
#include <iostream>
#include "cPhysics.h"
#include "cScene.h"

//extern std::vector<cMesh*> g_vec_pMeshesToDraw;
//extern cPhysics* g_pPhysics;

extern cScene* g_currentScene;

// static
unsigned int cSphere::m_nextsphereUniqueID = cSphere::SPHERE_FIRST_UNIQUE_ID;

cSphere::cSphere()
{
	// Set uniqueID
	this->m_sphereUniqueID = cSphere::m_nextsphereUniqueID;
	cSphere::m_nextsphereUniqueID++;
	tag = "explosion";
	cMesh* sphereMesh = GetObjectMesh();
	sphereMesh->bIsVisible = true;
	sphereMesh->drawPosition = glm::vec3(0.0f);
	sphereMesh->bUseDebugColours = true;
	sphereMesh->wholeObjectDebugColourRGBA = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	sphereMesh->setUniformDrawScale(1.0f);
	sphereMesh->objectType = "sphere";

	std::string friendlyName = "sphere" + std::to_string(this->m_sphereUniqueID);
	sphereMesh->friendlyName = friendlyName;
	sphereMesh->meshName = "shapes/Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";

	sPhysicsProperties* spherePhysics = GetObjectPhysics();
	spherePhysics->pTheAssociatedMesh = sphereMesh;
	spherePhysics->shapeType = sPhysicsProperties::SPHERE;
	spherePhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
	spherePhysics->setRotationFromEuler(sphereMesh->drawOrientation);
	spherePhysics->position = sphereMesh->drawPosition;
	spherePhysics->friendlyName = sphereMesh->friendlyName;
	spherePhysics->objectType = sphereMesh->objectType;
	g_currentScene->m_scenePhysics->AddShape(spherePhysics);
}

cSphere::~cSphere()
{

}

cSphere::cSphere(std::string friendlyName)
{
	// Set uniqueID
	this->m_sphereUniqueID = cSphere::m_nextsphereUniqueID;
	cSphere::m_nextsphereUniqueID++;

	cMesh* sphereMesh = GetObjectMesh();
	sphereMesh->bIsVisible = true;
	sphereMesh->drawPosition = glm::vec3(0.0f);
	sphereMesh->bUseDebugColours = true;
	sphereMesh->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	sphereMesh->setUniformDrawScale(1.0f);
	sphereMesh->objectType = "sphere";

	std::string name = friendlyName;
	name += this->m_sphereUniqueID;

	sphereMesh->friendlyName = name;
	sphereMesh->meshName = "shapes/Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";

	sPhysicsProperties* spherePhysics = GetObjectPhysics();
	spherePhysics->pTheAssociatedMesh = sphereMesh;
	spherePhysics->shapeType = sPhysicsProperties::SPHERE;
	spherePhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
	spherePhysics->setRotationFromEuler(sphereMesh->drawOrientation);
	spherePhysics->position = sphereMesh->drawPosition;
	spherePhysics->friendlyName = sphereMesh->friendlyName;
	spherePhysics->objectType = sphereMesh->objectType;
	g_currentScene->m_scenePhysics->AddShape(spherePhysics);
}

glm::vec3 cSphere::GetPosition()
{
	return GetObjectMesh()->drawPosition;
}

glm::vec3 cSphere::GetScale()
{
	return GetObjectMesh()->GetScale();
}

std::string cSphere::GetFriendlyName()
{
	return GetObjectMesh()->friendlyName;
}

void cSphere::SetPosition(glm::vec3 position)
{
	GetObjectMesh()->drawPosition = position;
	GetObjectPhysics()->position = position;
}

void cSphere::SetScale(float uniformScale)
{
	GetObjectMesh()->setUniformDrawScale(uniformScale);

	cMesh* sphereMesh = GetObjectMesh();

	for (unsigned int i = 0; i < sphereMesh->vec_pChildMeshes.size(); i++)
	{
		float childUniformScale = GetObjectMesh()->vec_pChildMeshes[i]->GetInitialScale().x * uniformScale;
		GetObjectMesh()->vec_pChildMeshes[i]->setUniformDrawScale(childUniformScale);
	}
}

void cSphere::SetScale(glm::vec3 scale)
{
	GetObjectMesh()->SetScale(scale);
}

void cSphere::IsUsingPhysics(bool isUsingPhysics)
{
	m_isUsingPhysics = isUsingPhysics;
}

void cSphere::Update(double deltaTime)
{
	cGameObject::Update(deltaTime);
	//std::cout << "Running Sphere update" << std::endl;
	//if (tag == "spheretest")
	//{
	//	float currentScale = GetScale().x;
	//	this->SetScale(currentScale + 0.001);
	//}
}
