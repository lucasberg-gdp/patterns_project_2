#include "cEnemy.h"
#include "cPhysics.h"
#include "cScene.h"


extern cScene* g_currentScene;

void cEnemy::MoveTowards(glm::vec3 direction)
{
}

void cEnemy::AdjustOrientation(glm::vec3 eulerOrientation)
{
}

cEnemy::cEnemy(cMesh* mesh):
	cGameObject(mesh)
{
    sPhysicsProperties* enemyPhysics = new sPhysicsProperties(sPhysicsProperties::SPHERE);
    enemyPhysics->pTheAssociatedMesh = GetObjectMesh();
    enemyPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
    enemyPhysics->setRotationFromEuler(GetObjectMesh()->drawOrientation);
    enemyPhysics->position = GetObjectMesh()->drawPosition;
    enemyPhysics->objectType = GetObjectMesh()->objectType;
    enemyPhysics->friendlyName = GetObjectMesh()->friendlyName;
    enemyPhysics->objectType = GetObjectMesh()->objectType;
    m_objectPhysics = enemyPhysics;
    g_currentScene->m_scenePhysics->AddShape(enemyPhysics);
}

void cEnemy::SetHorizontalMovement(float horizontal)
{
}

void cEnemy::SetVerticalMovement(float vertical)
{
}

void cEnemy::Update(double deltaTime)
{

}
