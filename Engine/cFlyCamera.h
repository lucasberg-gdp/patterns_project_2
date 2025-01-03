#pragma once
#ifndef _cFlyCamera_HG_
#define _cFlyCamera_HG_

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL		
#include <glm/gtx/quaternion.hpp>

#include "iCamera.h"
#include "cJsonHandler.h"
#include "sPhysicsProperties.h"

class cFlyCamera : public iCamera
{
private:
	float m_lastMouse_X;
	float m_lastMouse_Y;
	float m_lastMouse_Wheel;
	float m_Mouse_X;
	float m_Mouse_Y;
	float m_MouseWheel;
	float m_nearClip;
	float m_farClip;

	float m_slowCameraSpeed = 10.0f;
	float m_fastCameraSpeed = 50.0f;

	bool m_Mouse_Initial_State_Is_Set;

	bool m_isShiftDown = false;

	std::string cameraType = "flyCamera";

	glm::vec3 m_initialPosition;

	float m_leftVelocity = 0.0f;
	float m_upVelocity = 0.0f;

	glm::vec3 m_at;			// A direction, not a location
	glm::vec3 m_up = glm::vec3(0.0f,1.0f,0.0f);
	// THESE DON'T LIKELY CHANGE
	glm::vec3 m_frontOfCamera;// = glm::vec3(0,0,1);
	glm::vec3 m_upIsYVector;	// = glm::vec3(0,1,0);

	// This will lead to direction, etc.
	glm::quat qOrientation;
	glm::vec3 m_cameraEulerOrientation = glm::vec3(0.0f);

	void m_UpdateAtFromOrientation(void);
	void m_UpdateUpFromOrientation(void);

	// Inherited via iCamera
	void MoveCameraUp(void) override;
	void MoveCameraDown() override;

	// Inherited via iCamera
	void Start() override;

	std::map<char, bool> m_cameraMovements;

public:
	sPhysicsProperties* flyCameraPhysics = nullptr;

	cFlyCamera();
	cFlyCamera(sCameraInfo cameraInfo);

	virtual glm::vec3 getEye(void);
	virtual void setEye(glm::vec3 newEye);
	virtual glm::vec3 getAt(void);
	virtual void setAt(glm::vec3 newAt);
	virtual glm::vec3 getUp(void);
	virtual void setUp(glm::vec3 newUp);

	virtual bool Update(double deltaTime);
	virtual bool Update(std::string command, float data);
	virtual bool Update(std::string command, glm::vec2 data);
	virtual bool Update(std::string command, glm::vec3 data);

	virtual glm::vec3 getData(std::string command);

	// **************************************

	// Use these to allow us to use the LookAt() transform
	glm::vec3 eye;
	bool bKeepCameraFacingUp = false;

	glm::vec3 getAtInWorldSpace(void);
	glm::vec3 getCameraDirection(void);
	glm::vec3 getUpVector(void);

	std::string cameraFriendlyName;

	// These set methods will also clear the "last" mouse positions
	void setMouseXY(double newX, double newY);
	// The mouse wheel (on windows) only returns the delta, or the amount you scrolled
	void setMouseWheelDelta(double deltaWheel);

	float getMouseX(void);				// ...but we only need floats
	float getMouseY(void);

	float getDeltaMouseX(void);
	float getDeltaMouseY(void);
	float getMouseWheel(void);
	void clearMouseWheelValue(void);

	float movementSpeed = m_slowCameraSpeed;

	void MoveForward_Z(float amount);
	void MoveLeftRight_X(float amount);
	void MoveUpDown_Y(float amount);

	void Pitch_UpDown(float angleDegrees);	// around X
	void Yaw_LeftRight(float angleDegrees);	// around y
	void Roll_CW_CCW(float angleDegrees);	// around z

	glm::quat getQOrientation(void) const { return this->qOrientation; };
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);

	// Inherited via iCamera
	std::string GetCameraType(void) override;
	glm::vec3 GetPosition(void) override;
	glm::vec3 GetForward(void) override;

	glm::vec3 GetUp(void) override;
	void SetUp(glm::vec3 newUp) override;

	glm::vec3 GetLeft(void) override;
	std::string GetFriendlyName(void) override;

	float GetCameraSpeed(void) override;
	void SetCameraSpeed(float speed) override;

	void UseSlowCamera();
	void UseFastCamera();

	float GetNearClip(void) override;
	float GetFarClip(void) override;
	glm::quat GetOrientation(void) override;
	void SetOrientation(glm::vec3 eulerOrientation) override;

	glm::vec3 GetTarget(void)override;

	void AddVelocityToCamera(char key, glm::vec3 velocity) override;
	void RemoveVelocityFromCamera(char key)override; 

	void StopMovingInADirection(glm::vec3 direction) override;
	void StopMovingInADirection(char direction);
	void StopCameraUp() override;
	void LookAtTarget(glm::vec3 target) override;
	std::string CameraInfo() override;

	void SetFriendlyName(std::string name) override;
	void ResetCameraInitialConfig() override;

	void StopCameraDown() override;

	void Update() override;

	void SetPosition(glm::vec3 newPosition) override;

	void MoveToDirection(glm::vec3 direction) override;

	glm::vec3 GetInitialPosition() override;

};
#endif 
