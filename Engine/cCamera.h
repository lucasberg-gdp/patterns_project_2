#pragma once
#include <glm/gtc/quaternion.hpp>
#include <string>
#include "cMesh.h"
#include "sPhysicsProperties.h"
#include "cJsonHandler.h"
#include "iCamera.h"

class cCamera : public iCamera
{
private:
	bool m_isLookingAtTarget = false;

	float m_cameraSpeed = 2.0f;
	float m_cameraAcceleration = 6.0f;
	
	float m_cameraNearClip = 0.0f;
	float m_cameraFarClip = 0.0f;

	glm::vec3 m_cameraPosition = glm::vec3(0.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f);
	glm::vec3 m_cameraForward = glm::vec3(0.0f);
	glm::quat m_cameraOrientation = glm::quat(glm::vec3(0.0f));
	glm::vec3 m_cameraEulerOrientation = glm::vec3(0.0f);

	glm::vec3 m_initialCameraPosition = glm::vec3(0.0f);
	glm::quat m_initialCameraOrientation = glm::quat(glm::vec3(0.0f));
	glm::vec3 m_initialCameraTarget = glm::vec3(0.0f);

	// TODO: remove or relate it to camera speed perhaps
	glm::vec3 m_cameraScale = glm::vec3(0.0f);
	glm::vec3 m_cameraTarget = glm::vec3(0.0f);
	std::map<char, glm::vec3> m_VelocitiesAppliedToCamera;

	std::string cameraType = "camera";

	
public:
	cMesh* m_cameraMesh = nullptr;
	sPhysicsProperties* m_cameraPhysics;
	std::string cameraFriendlyName;

	const glm::vec3 LEFT_VECTOR = glm::vec3(1.f, 0.f, 0.f);

	//cCamera();

	//cCamera(std::string);
	cCamera(glm::vec3 cameraPosition, glm::quat cameraOrientation, glm::vec3 cameraScale, glm::vec3 cameraTarget);
	cCamera(sCameraInfo cameraInfo);
	cCamera(sCameraInfo cameraInfo, std::string meshName);

	cCamera(glm::vec3 cameraPosition, glm::vec3 cameraForward, glm::vec3 cameraUp, float nearClip, float farClip);

	glm::vec3 GetCameraPosition() const;
	glm::quat GetCameraOrientation() const;
	glm::vec3 GetCameraScale() const;
	glm::vec3 GetCameraTarget() const;

	glm::vec3 GetCameraUp() const;
	glm::vec3 GetCameraForward() const;

	float GetNearClip(void);
	float GetFarClip(void);
	std::string GetFriendlyName(void) override;

	void SetCameraPosition(glm::vec3 cameraPosition);
	void SetCameraOrientation(glm::quat cameraOrientation);
	void SetCameraScale(glm::vec3 cameraScale);
	void SetCameraTarget(glm::vec3 cameraTarget);
	void SetIsLookingAtTarget(bool isLookingAtTarget);

	void MoveToDirection(glm::vec3 direction) override;

	void MoveCameraUp();

	void MoveCameraDown();
	void StopCameraDown() override;
	
	// editor mode
	bool MoveToDirection3D(glm::vec3 direction);
	bool StopMoveToDirection3D(glm::vec3 direction);

	void ResetCameraInitialConfig();

	void LookAtTarget(glm::vec3 target);

	std::string CameraInfo() override;

	void Update() override;

	void AddVelocityToCamera(char key, glm::vec3 velocity);
	void RemoveVelocityFromCamera(char key);
	void UpdateVelocity();

	// Inherited via iCamera
	//glm::vec3 getEye(void) override;
	//void setEye(glm::vec3 newEye) override;

	glm::vec3 getAt(void) override;

	void setAt(glm::vec3 newAt) override;

	glm::vec3 GetUp(void) override;
	void SetUp(glm::vec3 newUp) override;
	//glm::vec3 getUp(void) override;

	//void setUp(glm::vec3 newUp) override;

	glm::vec3 getData(std::string command) override;

	bool Update(std::string command, float data) override;
	bool Update(std::string command, glm::vec2 data) override;
	bool Update(std::string command, glm::vec3 data) override;
	bool Update(double deltaTime) override;

	// Inherited via iCamera
	std::string GetCameraType(void) override;
	glm::vec3 GetPosition(void) override;
	glm::vec3 GetForward(void) override;
	glm::vec3 GetLeft(void) override;
	glm::quat GetOrientation(void) override;
	void SetOrientation(glm::vec3 eulerOrientation) override;

	glm::vec3 GetTarget(void)override;

	void SetPosition(glm::vec3 newPosition) override;

	float GetCameraSpeed(void) override;
	void SetCameraSpeed(float speed) override;

	void StopMovingInADirection(glm::vec3 direction) override;
	void StopCameraUp() override;

	glm::vec3 GetInitialPosition() override;

	// Inherited via iCamera
	void SetFriendlyName(std::string name) override;

	// Inherited via iCamera
	void Start() override;
};