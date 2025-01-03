#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

#include "cCamera.h"
#include "cGameManager.h"
#include "MathUtils.h"
#include "cScene.h"

extern cScene* g_currentScene;

extern std::vector< sPhysicsProperties* > g_vec_pPhysicalProps;

//cCamera::cCamera(std::string cameraFile):
//	m_cameraPhysics(new sPhysicsProperties())
//{
//	std::ifstream cameraInfoFile(cameraFile);
//
//	float w, x, y, z;
//	cameraInfoFile >> x >> y >> z;
//	m_cameraPosition = glm::vec3(x, y, z);
//
//	cameraInfoFile >> w >> x >> y >> z;
//	m_cameraOrientation = glm::quat(w, x, y, z);
//
//	cameraInfoFile >> x >> y >> z;
//	m_cameraScale = glm::vec3(x, y, z);
//
//	cameraInfoFile >> x >> y >> z;
//	m_cameraTarget = glm::vec3(x, y, z);
//
//	m_initialCameraPosition = m_cameraPosition;
//	m_initialCameraOrientation = m_cameraOrientation;
//	m_initialCameraTarget = m_cameraTarget;
//
//	m_cameraMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName("camera");
//	if (m_cameraMesh)
//	{
//		m_cameraMesh->bIsVisible = false;
//		m_cameraPhysics = new sPhysicsProperties();
//		m_cameraPhysics->pTheAssociatedMesh = m_cameraMesh;
//		m_cameraPhysics->shapeType = sPhysicsProperties::SPHERE;
//		m_cameraPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
//		m_cameraPhysics->setRotationFromEuler(m_cameraMesh->drawOrientation);
//		m_cameraPhysics->position = m_cameraPosition;
//		m_cameraPhysics->friendlyName = "camera";
//		m_cameraPhysics->objectType = "camera";
//		g_currentScene->m_scenePhysics->AddShape(m_cameraPhysics);
//	}
//}

cCamera::cCamera(glm::vec3 cameraPosition, glm::quat cameraOrientation, glm::vec3 cameraScale, glm::vec3 cameraTarget)
	:
	m_cameraPhysics(new sPhysicsProperties()),
	m_cameraPosition(cameraPosition),
	m_cameraOrientation(cameraOrientation),
	m_cameraScale(cameraScale),
	m_cameraTarget(cameraTarget)
{
	m_initialCameraPosition = m_cameraPosition;
	m_initialCameraOrientation = m_cameraOrientation;
	m_initialCameraTarget = m_cameraTarget;


	m_cameraMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName("camera");

	if (m_cameraMesh)
	{
		m_cameraMesh->bIsVisible = false;

		m_cameraPhysics->pTheAssociatedMesh = m_cameraMesh;
		m_cameraPhysics->shapeType = sPhysicsProperties::SPHERE;
		m_cameraPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
		m_cameraPhysics->setRotationFromEuler(m_cameraMesh->drawOrientation);
		m_cameraPhysics->position = m_cameraPosition;
		m_cameraPhysics->friendlyName = "camera";
		m_cameraPhysics->objectType = "camera";
		g_currentScene->m_scenePhysics->AddShape(m_cameraPhysics);
	}
}

cCamera::cCamera(sCameraInfo cameraInfo):
	m_cameraPhysics(new sPhysicsProperties()),
	m_cameraScale(glm::vec3(1.0f)),
	m_cameraTarget(cameraInfo.Target),
	m_cameraForward(cameraInfo.Forward),
	m_cameraNearClip(cameraInfo.NearClip),
	m_cameraFarClip(cameraInfo.FarClip),
	m_cameraUp(cameraInfo.Up),
	m_cameraPosition(cameraInfo.Position)
{

	m_initialCameraPosition = m_cameraPosition;
	m_initialCameraOrientation = m_cameraOrientation;
	m_initialCameraTarget = m_cameraTarget;

	//m_cameraMesh = cGameManager::getGmInstance()->FindMeshByFriendlyName(meshName);

	//if (m_cameraMesh)
	//{
	//	m_cameraMesh->bIsVisible = false;

	//	m_cameraPhysics->pTheAssociatedMesh = m_cameraMesh;
	//	m_cameraPhysics->shapeType = sPhysicsProperties::SPHERE;
	//	m_cameraPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
	//	m_cameraPhysics->setRotationFromEuler(m_cameraMesh->drawOrientation);
	//	m_cameraPhysics->position = m_cameraPosition;
	//	m_cameraPhysics->friendlyName = meshName;
	//	m_cameraPhysics->objectType = meshName;
	//	::g_pPhysics->AddShape(m_cameraPhysics);
	//}
}


cCamera::cCamera(sCameraInfo cameraInfo, std::string meshName) :
	m_cameraPhysics(new sPhysicsProperties()),
	m_cameraScale(glm::vec3(1.0f)),
	m_cameraForward(cameraInfo.Forward),
	m_cameraNearClip(cameraInfo.NearClip),
	m_cameraFarClip(cameraInfo.FarClip),
	m_cameraUp(cameraInfo.Up),
	m_cameraPosition(cameraInfo.Position)
{

	m_initialCameraPosition = m_cameraPosition;
	m_initialCameraOrientation = m_cameraOrientation;
	m_initialCameraTarget = m_cameraTarget;

	m_cameraMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName(meshName);

	if (m_cameraMesh)
	{
		m_cameraMesh->bIsVisible = false;

		m_cameraPhysics->pTheAssociatedMesh = m_cameraMesh;
		m_cameraPhysics->shapeType = sPhysicsProperties::SPHERE;
		m_cameraPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
		m_cameraPhysics->setRotationFromEuler(m_cameraMesh->drawOrientation);
		m_cameraPhysics->position = m_cameraMesh->drawPosition;
		m_cameraPhysics->friendlyName = meshName;
		m_cameraPhysics->objectType = meshName;
		g_currentScene->m_scenePhysics->AddShape(m_cameraPhysics);
	}
}

cCamera::cCamera(glm::vec3 cameraPosition, glm::vec3 cameraForward, glm::vec3 cameraUp, float nearClip, float farClip):
	m_cameraPhysics(new sPhysicsProperties()),
	m_cameraPosition(cameraPosition),
	m_cameraForward(cameraForward),
	m_cameraUp(cameraUp),
	m_cameraNearClip(nearClip),
	m_cameraFarClip(farClip)
{
	m_cameraMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName("maincamera");

	m_cameraOrientation = glm::quat(glm::vec3(0.0f));

	if (m_cameraMesh)
	{
		m_cameraMesh->bIsVisible = false;

		m_cameraPhysics->pTheAssociatedMesh = m_cameraMesh;
		m_cameraPhysics->shapeType = sPhysicsProperties::SPHERE;
		m_cameraPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
		m_cameraPhysics->setRotationFromEuler(m_cameraMesh->drawOrientation);
		m_cameraPhysics->position = m_cameraPosition;
		m_cameraPhysics->friendlyName = "camera";
		m_cameraPhysics->objectType = "camera";
		g_currentScene->m_scenePhysics->AddShape(m_cameraPhysics);
	}
}

glm::vec3 cCamera::GetCameraPosition() const 
{
	return m_cameraPosition;
}

glm::quat cCamera::GetCameraOrientation() const
{
	return m_cameraOrientation;
}

glm::vec3 cCamera::GetCameraScale() const
{
	return m_cameraScale;
}

glm::vec3 cCamera::GetCameraTarget() const
{
	return m_cameraTarget;
}

glm::vec3 cCamera::GetCameraUp() const
{
	return m_cameraUp;
}

glm::vec3 cCamera::GetCameraForward() const
{
	return m_cameraForward;
}

float cCamera::GetNearClip()
{
	return m_cameraNearClip;
}

float cCamera::GetFarClip()
{
	return m_cameraFarClip;
}

std::string cCamera::GetFriendlyName(void)
{
	return cameraFriendlyName;
}

void cCamera::SetCameraSpeed(float cameraSpeed)
{
	m_cameraSpeed = cameraSpeed;
}

void cCamera::SetCameraPosition(glm::vec3 cameraPosition)
{
	this->m_cameraPosition = cameraPosition;
}

void cCamera::SetCameraOrientation(glm::quat cameraOrientation)
{
	this->m_cameraOrientation = cameraOrientation;
}

void cCamera::SetCameraScale(glm::vec3 cameraScale)
{
	this->m_cameraScale = cameraScale;
}

void cCamera::SetCameraTarget(glm::vec3 cameraTarget)
{
	m_cameraTarget = cameraTarget;
	m_isLookingAtTarget = true;
}

void cCamera::SetIsLookingAtTarget(bool isLookingAtTarget)
{
	m_isLookingAtTarget = isLookingAtTarget;
}

void cCamera::MoveToDirection(glm::vec3 direction)
{
	float leftDirection = direction.x;
	float forwardDirection = direction.z;

	glm::vec3 newPosition = m_cameraPosition;

	newPosition += m_cameraOrientation* LEFT_VECTOR * m_cameraSpeed* leftDirection;
	newPosition += m_cameraOrientation * m_cameraForward * m_cameraSpeed * forwardDirection;
	glm::vec3 normalizedDirection = glm::normalize(newPosition - m_cameraPosition);

	m_cameraPhysics->velocity = normalizedDirection * m_cameraAcceleration;
}

void cCamera::StopMovingInADirection(glm::vec3 direction)
{
	m_cameraPhysics->velocity = glm::vec3(0.0f);

	/*glm::vec3 velocityInDirection = 
		glm::vec3(m_cameraPhysics->velocity.x * direction.x, 
				  m_cameraPhysics->velocity.y * direction.y, 
			      m_cameraPhysics->velocity.z * direction.z);

	if (m_cameraPhysics->velocity.x >= velocityInDirection.x)
	{
		m_cameraPhysics->velocity.x -= velocityInDirection.x;
	}
	else
	{
		m_cameraPhysics->velocity.x += velocityInDirection.x;
	}


	if (m_cameraPhysics->velocity.y >= velocityInDirection.y)
	{
		m_cameraPhysics->velocity.y -= velocityInDirection.y;
	}
	else
	{
		m_cameraPhysics->velocity.y += velocityInDirection.y;
	}

	if (m_cameraPhysics->velocity.z >= velocityInDirection.z)
	{
		m_cameraPhysics->velocity.z -= velocityInDirection.z;
	}
	else
	{
		m_cameraPhysics->velocity.z += velocityInDirection.z;
	}

	std::cout << m_cameraPhysics->PhysicsInfo();*/
	//return true;
}

void cCamera::MoveCameraUp()
{
	m_cameraPhysics->velocity.y = m_cameraSpeed;
}

void cCamera::StopCameraUp()
{
	m_cameraPhysics->velocity.y = 0.0f;
}

glm::vec3 cCamera::GetInitialPosition()
{
	return m_initialCameraPosition;
}

void cCamera::SetFriendlyName(std::string name)
{
	cameraFriendlyName = name;
}

void cCamera::Start()
{
	cMesh* cameraMesh = g_currentScene->GetMeshByFriendlyName("camera");
	m_cameraPhysics->pTheAssociatedMesh = cameraMesh;
	this->SetCameraPosition(cameraMesh->drawPosition);
}

void cCamera::MoveCameraDown()
{
	m_cameraPhysics->velocity.y = -m_cameraSpeed;
}

void cCamera::StopCameraDown()
{
	m_cameraPhysics->velocity.y = 0.0f;
}

void cCamera::ResetCameraInitialConfig() 
{
	m_cameraPhysics->velocity = glm::vec3(0.0f);
	m_cameraPhysics->acceleration = glm::vec3(0.0f);

	m_cameraPhysics->position = m_initialCameraPosition;
	m_cameraPhysics->setRotationFromEuler(glm::vec3(m_initialCameraOrientation.x, m_initialCameraOrientation.y, m_initialCameraOrientation.z));
	m_cameraTarget = m_initialCameraTarget;
	m_cameraOrientation = m_cameraPhysics->get_qOrientation();
}

bool cCamera::MoveToDirection3D(glm::vec3 direction)
{
	m_cameraPhysics->velocity += direction * m_cameraSpeed;
	return true;
}

bool cCamera::StopMoveToDirection3D(glm::vec3 direction)
{
	m_cameraPhysics->velocity -= direction * m_cameraSpeed;
	return true;
}

void cCamera::LookAtTarget(glm::vec3 target)
{
	m_cameraTarget = target;
}

std::string cCamera::CameraInfo()
{
	// position
	std::string cameraPositionX = MathUtils::RoundFloat(m_cameraPosition.x, 2);
	std::string cameraPositionY = MathUtils::RoundFloat(m_cameraPosition.y, 2);
	std::string cameraPositionZ = MathUtils::RoundFloat(m_cameraPosition.z, 2);

	// orientation
	std::string cameraOrientationW = MathUtils::RoundFloat(m_cameraOrientation.w, 2);
	std::string cameraOrientationX = MathUtils::RoundFloat(m_cameraOrientation.x, 2);
	std::string cameraOrientationY = MathUtils::RoundFloat(m_cameraOrientation.y, 2);
	std::string cameraOrientationZ = MathUtils::RoundFloat(m_cameraOrientation.z, 2);

	return "Camera Position: (" 
		+ cameraPositionX + ", "
		+ cameraPositionY + ", "
		+ cameraPositionZ + ") "
		+ "Orientation (w,x,y,z): (" 
		+ cameraOrientationW + ", "
		+ cameraOrientationX + ", "
		+ cameraOrientationY + ", "
		+ cameraOrientationZ + ") ";
}

void cCamera::Update()
{
	if ("xwingcamera" == cameraFriendlyName)
	{
		m_cameraPosition = m_cameraPhysics->position;

		sPhysicsProperties* xwing = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName("xwing1");

		if (xwing)
		{
			SetCameraTarget(xwing->position);

			if (xwing->position.z - m_cameraPhysics->position.z > 20.0f)
			{
				//m_cameraPhysics->position.z = xwing->position.z;
			}
		}
	}
	else
	{
		UpdateVelocity();

		//if (m_cameraPhysics)
		//{
		//	m_cameraPosition = m_cameraPhysics->position;
		//}
	}
}

// f - forward, b - backward, l - left, r - right
void cCamera::AddVelocityToCamera(char key, glm::vec3 velocity)
{
	std::map<char, glm::vec3>::iterator velocityIt =  m_VelocitiesAppliedToCamera.find(key);

	if (velocityIt != m_VelocitiesAppliedToCamera.end())
	{
		velocity += m_VelocitiesAppliedToCamera[key];
	}

	m_VelocitiesAppliedToCamera.insert(std::make_pair(key, velocity));
}

void cCamera::RemoveVelocityFromCamera(char key)
{
	m_VelocitiesAppliedToCamera.erase(key);
}

void cCamera::UpdateVelocity()
{
	glm::vec3 cameraVelocity = glm::vec3(0.0f);

	for (const std::pair<char, glm::vec3> velocity : m_VelocitiesAppliedToCamera)
	{
		cameraVelocity += velocity.second;
	}

	if (glm::length(cameraVelocity) > 0.0f)
	{
		m_cameraPhysics->velocity = glm::normalize(cameraVelocity) * m_cameraSpeed;
	}

	else
	{
		m_cameraPhysics->velocity = glm::vec3(0.0f);
	}
}

glm::vec3 cCamera::getAt(void)
{
	return glm::vec3();
}

void cCamera::setAt(glm::vec3 newAt)
{
	m_cameraTarget = newAt;
}

glm::vec3 cCamera::getData(std::string command)
{
	return glm::vec3();
}

bool cCamera::Update(std::string command, float data)
{
	return false;
}

bool cCamera::Update(std::string command, glm::vec2 data)
{
	return false;
}

bool cCamera::Update(std::string command, glm::vec3 data)
{
	return false;
}

bool cCamera::Update(double deltaTime)
{
	this->SetPosition(m_cameraPosition);

	//UpdateVelocity();

	return false;
}

std::string cCamera::GetCameraType(void)
{
	return cameraType;
}

glm::vec3 cCamera::GetPosition(void)
{
	return m_cameraPosition;
}

glm::vec3 cCamera::GetForward(void)
{
	return m_cameraForward;
}

glm::vec3 cCamera::GetUp(void)
{
	return m_cameraUp;
}

void cCamera::SetUp(glm::vec3 newUp)
{
	m_cameraUp = newUp;
}

glm::vec3 cCamera::GetLeft(void)
{
	return LEFT_VECTOR;
}

glm::quat cCamera::GetOrientation(void)
{
	return m_cameraOrientation;
}

void cCamera::SetOrientation(glm::vec3 eulerOrientation)
{
	m_cameraEulerOrientation = eulerOrientation;
	m_cameraOrientation = glm::quat(m_cameraEulerOrientation);
}

void setRotationFromEuler(glm::vec3 newEulerAngleXYZ)
{

}

glm::vec3 cCamera::GetTarget(void)
{
	return m_cameraTarget;
}

void cCamera::SetPosition(glm::vec3 newPosition)
{
	//if (m_cameraPhysics)
	//{
	//	m_cameraPhysics->position = newPosition;
	//}
	//else
	//{
	//	if (m_cameraMesh)
	//	{
	//		m_cameraMesh->drawPosition = newPosition;
	//	}
	//}

	m_cameraPosition = newPosition;
}

float cCamera::GetCameraSpeed(void)
{
	return m_cameraSpeed;
}

