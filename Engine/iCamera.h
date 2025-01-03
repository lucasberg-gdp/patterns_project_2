#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

class iCamera
{
public:
	virtual ~iCamera() {};

	virtual glm::vec3 GetPosition(void) = 0;
	virtual void SetPosition(glm::vec3 newPosition) = 0;

	virtual glm::vec3 getAt(void) = 0;
	virtual void setAt(glm::vec3 newAt) = 0;

	virtual glm::vec3 GetUp(void) = 0;
	virtual void SetUp(glm::vec3 newUp) = 0;

	virtual glm::vec3 getData(std::string command) = 0;

	virtual std::string GetCameraType(void) = 0;

	virtual glm::vec3 GetForward(void) = 0;


	virtual glm::vec3 GetLeft(void) = 0;

	virtual float GetNearClip(void) = 0;

	virtual float GetFarClip(void) = 0;

	virtual std::string GetFriendlyName(void) = 0;
	virtual void SetFriendlyName(std::string name) = 0;

	virtual float GetCameraSpeed(void) = 0;
	virtual void SetCameraSpeed(float speed) = 0;

	virtual glm::quat GetOrientation(void) = 0;
	virtual glm::vec3 GetTarget(void) = 0;

	virtual void SetOrientation(glm::vec3 eulerOrientation) = 0;
	virtual void Start() = 0;

	// Used to set various commands on the camera
	virtual bool Update(std::string command, float data) = 0;
	virtual bool Update(std::string command, glm::vec2 data) = 0;
	virtual bool Update(std::string command, glm::vec3 data) = 0;
	// For cameras that need multiple updates per frame
	virtual bool Update(double deltaTime) = 0;
	virtual void Update() = 0;

	virtual void MoveCameraUp(void) = 0;
	virtual void MoveCameraDown() = 0;
	virtual void StopMovingInADirection(glm::vec3 direction) = 0;
	virtual void StopCameraUp() = 0;
	virtual void LookAtTarget(glm::vec3 target) = 0;

	virtual void AddVelocityToCamera(char key, glm::vec3 velocity) = 0;
	virtual void RemoveVelocityFromCamera(char key) = 0;
	virtual void ResetCameraInitialConfig() = 0;

	virtual void StopCameraDown() = 0;

	virtual void MoveToDirection(glm::vec3 direction) = 0;

	virtual std::string CameraInfo() = 0;

	virtual glm::vec3 GetInitialPosition() = 0;
};