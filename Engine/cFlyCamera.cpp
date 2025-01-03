#include "cFlyCamera.h"

//#include <glm/glm.hpp>
//#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

cFlyCamera::cFlyCamera()
{
	m_initialPosition = glm::vec3(0.0f, 70.0f, -40.0f);

	this->eye = m_initialPosition;

	// This will be constant
	this->m_frontOfCamera = glm::vec3(0.0f, 0.0f, 1.0f);

	this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->movementSpeed = m_slowCameraSpeed;

	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	//// Initial "at" is 1 unit in front of the camera, along z
	//this->at = this->m_frontOfCamera;

	// If we are using the mouse
	this->setMouseXY(0.0, 0.0);
	this->m_MouseWheel = 0.0f;

	this->m_Mouse_Initial_State_Is_Set = false;

	this->bKeepCameraFacingUp = false;
}

cFlyCamera::cFlyCamera(sCameraInfo cameraInfo)
{
	//this->eye = glm::vec3(0.0f, 70.0f, -40.0f);

	//// This will be constant
	//this->m_frontOfCamera = glm::vec3(0.0f, 0.0f, 1.0f);

	//this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);
	//this->movementSpeed = 0.1f;

	//// Set initial orientation (all zero on Euler axes)
	//this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	eye = cameraInfo.Position;
	m_frontOfCamera = cameraInfo.Forward;
	m_upIsYVector = cameraInfo.Up;
	movementSpeed = m_slowCameraSpeed;
	m_nearClip = cameraInfo.NearClip;
	m_farClip = cameraInfo.FarClip;
	//m_at = glm::vec3(cameraInfo.Orientation);

	m_initialPosition = eye;

	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	qOrientation = cameraInfo.Orientation;

	this->bKeepCameraFacingUp = true;


	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	//// Initial "at" is 1 unit in front of the camera, along z
	//this->at = this->m_frontOfCamera;

	// If we are using the mouse
	this->setMouseXY(0.0, 0.0);
	this->m_MouseWheel = 0.0f;

	this->m_Mouse_Initial_State_Is_Set = false;

	this->bKeepCameraFacingUp = true;

	flyCameraPhysics = sPhysicsProperties::CreateSphere(1.0f);
	flyCameraPhysics->friendlyName = "flyCamera";
	flyCameraPhysics->position = eye;
	flyCameraPhysics->maxVelocity = 100.0f;
	flyCameraPhysics->SetRotation(cameraInfo.Orientation);
}

glm::vec3 cFlyCamera::getAtInWorldSpace(void)
{
	return this->eye + this->m_at;
}

glm::vec3 cFlyCamera::getCameraDirection(void)
{
	this->m_UpdateAtFromOrientation();
	return this->m_at;
}

void cFlyCamera::MoveForward_Z(float amount)
{
	// We take the vector from the at to eye (i.e. direction)
	// Add this forward velocity along this direction.

	//glm::vec3 direction = this->getAtInWorldSpace() - this->eye;

	//// Make direction a "unit length"
	//direction = glm::normalize(direction);

	//// Generate a "forward" adjustment value 
	//glm::vec3 amountToMove = direction * amount;

	//// Add this to the eye
	//eye += amountToMove * movementSpeed;

	//return;

	if (amount > 0)
	{
		m_cameraMovements['f'] = true;
	}
	else
	{
		m_cameraMovements['b'] = true;
	}
}

void cFlyCamera::MoveUpDown_Y(float amount)
{
	// We use the Up vector to determine which direction (in world space) we need to go

	glm::vec3 vecAmountToMove = glm::normalize(this->getUpVector()) * amount;

	this->eye += vecAmountToMove * movementSpeed;

	return;
}

void cFlyCamera::MoveLeftRight_X(float amount)
{
	// left and right is a little tricky, as we can "roll", 
	// because left and right change in world space.
	// 
	// If you perform a cross product between the "forward" and "up" vectors, you'll
	//	get a vector perpendicular to both of these, so left and right.
	// Example: Assuming "forward" is the Z axis, and "up" is the Y axis, then 
	//	performing the cross product of these gives you the X axis. 

	//glm::vec3 vecLeft = glm::cross(this->getCameraDirection(), this->getUpVector());

	//glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;

	////m_leftVelocity = amount;

	//eye += vecAmountToMove * movementSpeed;

	if (amount > 0)
	{
		m_cameraMovements['r'] = true;
	}
	else
	{
		m_cameraMovements['l'] = true;
	}
}

void cFlyCamera::Pitch_UpDown(float angleDegrees)	// around X
{
	adjMeshOrientationEulerAngles(glm::vec3(angleDegrees, 0.0f, 0.0f), true);

	m_UpdateAtFromOrientation();
	m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::Yaw_LeftRight(float angleDegrees)	// around y
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, angleDegrees, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::Roll_CW_CCW(float angleDegrees)	// around z
{
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, 0.0f, angleDegrees), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x),
			glm::radians(newAnglesEuler.y),
			glm::radians(newAnglesEuler.z));
	}

	this->qOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cFlyCamera::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x),
			glm::radians(adjAngleEuler.y),
			glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Per frame update (SLERP)
	//glm::quat rotatThisFrame = glm::slerp(rotationAdjust, deltaTime );

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->qOrientation *= rotationAdjust;
	this->qOrientation = this->qOrientation * rotationAdjust;

	return;
}

void cFlyCamera::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationQ(glm::quat adjOrientQ)
{
	this->qOrientation *= adjOrientQ;
	return;
}

void cFlyCamera::m_UpdateAtFromOrientation(void)
{
	// Have a rotation around the origin (eye)

	// vec4 new = mat4Transform * (vec4)Location

	// c'tor of mat4 can take a quaternion and generate 
	//	a rotation matrix from it... 
	glm::mat4 matRotation = glm::mat4(this->qOrientation);

	// Need to make these 4x4
	// NOTE: You could just keep these as 3x3 matrix values
	// (so you could keep them as vec3's....)

	glm::vec4 frontOfCamera = glm::vec4(this->m_frontOfCamera, 1.0f);

	glm::vec4 newAt = matRotation * frontOfCamera;

	// Update the "At"
	this->m_at = glm::vec3(newAt);

	return;
}

void cFlyCamera::m_UpdateUpFromOrientation(void)
{
	if (this->bKeepCameraFacingUp)
	{
		this->m_up = this->m_upIsYVector;
	}
	else
	{
		glm::mat4 matRotation = glm::mat4(this->qOrientation);

		glm::vec4 upVector = glm::vec4(this->m_upIsYVector, 1.0f);

		glm::vec4 newUp = matRotation * upVector;

		// Update the "At"
		this->m_up = glm::vec3(newUp);
	}

	return;
}

void cFlyCamera::MoveCameraUp(void)
{
}

void cFlyCamera::MoveCameraDown()
{
}

void cFlyCamera::Start()
{
}

std::string cFlyCamera::GetCameraType(void)
{
	return cameraType;
}

glm::vec3 cFlyCamera::GetPosition(void)
{
	return eye;
}

glm::vec3 cFlyCamera::GetForward(void)
{
	return m_frontOfCamera;
}

glm::vec3 cFlyCamera::GetUp(void)
{
	return m_upIsYVector;
}

void cFlyCamera::SetUp(glm::vec3 newUp)
{
	m_up = newUp;
}

glm::vec3 cFlyCamera::GetLeft(void)
{
	return glm::vec3(1.0f, 0.0f, 0.0f);
}

std::string cFlyCamera::GetFriendlyName(void)
{
	return cameraFriendlyName;
}

float cFlyCamera::GetCameraSpeed(void)
{
	return movementSpeed;
}

void cFlyCamera::SetCameraSpeed(float speed)
{
	movementSpeed = speed;
}

void cFlyCamera::UseSlowCamera()
{
	movementSpeed = this->m_slowCameraSpeed;
}

void cFlyCamera::UseFastCamera()
{
	movementSpeed = m_fastCameraSpeed;
}

float cFlyCamera::GetNearClip(void)
{
	return m_nearClip;
}

float cFlyCamera::GetFarClip(void)
{
	return m_farClip;
}

glm::quat cFlyCamera::GetOrientation(void)
{
	return qOrientation;
}

void cFlyCamera::SetOrientation(glm::vec3 eulerOrientation)
{
	m_cameraEulerOrientation = eulerOrientation;
	qOrientation = glm::quat(m_cameraEulerOrientation);
}

glm::vec3 cFlyCamera::GetTarget(void)
{
	return m_at;
}

void cFlyCamera::AddVelocityToCamera(char key, glm::vec3 velocity)
{

}

void cFlyCamera::RemoveVelocityFromCamera(char key)
{

}

void cFlyCamera::StopMovingInADirection(glm::vec3 direction)
{

}

void cFlyCamera::StopMovingInADirection(char direction)
{
	m_cameraMovements[direction] = false;
}

void cFlyCamera::StopCameraUp()
{

}

void cFlyCamera::LookAtTarget(glm::vec3 target)
{

}

std::string cFlyCamera::CameraInfo()
{
	return std::string();
}

void cFlyCamera::SetFriendlyName(std::string name)
{
	cameraFriendlyName = name;
}

void cFlyCamera::ResetCameraInitialConfig()
{
}

void cFlyCamera::StopCameraDown()
{
}

void cFlyCamera::Update()
{

}

void cFlyCamera::SetPosition(glm::vec3 newPosition)
{
	//eye = newPosition;
}

void cFlyCamera::MoveToDirection(glm::vec3 direction)
{
}

glm::vec3 cFlyCamera::GetInitialPosition()
{
	return m_initialPosition;
}

glm::vec3 cFlyCamera::getUpVector(void)
{
	return this->m_up;
}

void cFlyCamera::setMouseXY(double newX, double newY)
{
	this->m_lastMouse_X = this->m_Mouse_X;
	this->m_Mouse_X = (float)newX;

	this->m_lastMouse_Y = this->m_Mouse_Y;
	this->m_Mouse_Y = (float)newY;

	// Can return value numbers
	this->m_Mouse_Initial_State_Is_Set = true;

	return;
}

void cFlyCamera::setMouseWheelDelta(double deltaWheel)
{
	this->m_lastMouse_Wheel = this->m_MouseWheel;
	this->m_MouseWheel += (float)deltaWheel;
	return;
}

float cFlyCamera::getMouseX(void)
{
	return this->m_Mouse_X;
}

float cFlyCamera::getMouseY(void)
{
	return this->m_Mouse_Y;
}

float cFlyCamera::getDeltaMouseX(void)
{
	if (!this->m_Mouse_Initial_State_Is_Set)
	{
		return 0.0f;
	}
	return this->m_Mouse_X - this->m_lastMouse_X;
}

float cFlyCamera::getDeltaMouseY(void)
{
	if (!this->m_Mouse_Initial_State_Is_Set)
	{
		return 0.0f;
	}
	return this->m_Mouse_Y - this->m_lastMouse_Y;
}

float cFlyCamera::getMouseWheel(void)
{
	return this->m_MouseWheel;
}

void cFlyCamera::clearMouseWheelValue(void)
{
	m_MouseWheel = 0.0f;
}

// From iCamera
glm::vec3 cFlyCamera::getEye(void)
{
	return this->eye;
}

void cFlyCamera::setEye(glm::vec3 newEye)
{
	//eye = newEye;
}

glm::vec3 cFlyCamera::getAt(void)
{
	return getAtInWorldSpace();
}

void cFlyCamera::setAt(glm::vec3 newAt)
{
	m_at = newAt;
}

glm::vec3 cFlyCamera::getUp(void)
{
	return getUpVector();
}

void cFlyCamera::setUp(glm::vec3 newUp)
{
	m_up = newUp;
}

bool cFlyCamera::Update(std::string command, float data)
{
	if (command == "setMouseWheelDelta")
	{
		this->setMouseWheelDelta(data);
		return true;
	}
	else if (command == "Yaw_LeftRight")
	{
		this->Yaw_LeftRight(data);
		return true;
	}
	else if (command == "Pitch_UpDown")
	{
		this->Pitch_UpDown(data);
		return true;
	}
	else if (command == "setMovementSpeed")
	{
		this->movementSpeed = data;
		return true;
	}
	else if (command == "MoveForward_Z")
	{
		this->MoveForward_Z(data);
		return true;
	}
	else if (command == "MoveLeftRight_X")
	{
		this->MoveLeftRight_X(data);
		return true;
	}
	else if (command == "MoveUpDown_Y")
	{
		this->MoveUpDown_Y(data);
		return true;
	}
	else if (command == "Roll_CW_CCW")
	{
		this->Roll_CW_CCW(data);
		return true;
	}

	return false;
}

bool cFlyCamera::Update(std::string command, glm::vec2 data)
{
	if (command == "setMouseXY")
	{
		this->setMouseXY(data.x, data.y);
		return true;
	}

	return false;
}

bool cFlyCamera::Update(std::string command, glm::vec3 data)
{
	return true;
}

bool cFlyCamera::Update(double deltaTime)
{
	glm::vec3 vecLeft = glm::cross(this->getCameraDirection(), this->getUpVector());

	float horizontaMovement = 0.0f;

	if (m_cameraMovements['r'])
	{
		horizontaMovement += 1.0f;
	}
	if (m_cameraMovements['l'])
	{
		horizontaMovement -= 1.0f;
	}

	float frontBackMovement = 0.0f;

	if (m_cameraMovements['f'])
	{
		frontBackMovement += 1.0f;
	}
	if (m_cameraMovements['b'])
	{
		frontBackMovement -= 1.0f;
	}

	glm::vec3 vecAmountToMove = glm::vec3(0.0f);

	glm::vec3 direction = this->getAtInWorldSpace() - this->eye;

	direction = glm::normalize(direction);

	glm::vec3 amountToMove = direction * (float)deltaTime * frontBackMovement;

	vecAmountToMove = glm::normalize(vecLeft) * horizontaMovement * (float)deltaTime;

	glm::vec3 addedVectors = amountToMove + vecAmountToMove;

	if (addedVectors != glm::vec3(0.0f))
	{
		glm::vec3 finalMovement = glm::normalize(addedVectors);

		eye += finalMovement * movementSpeed;
	}

	return true;
}

glm::vec3 cFlyCamera::getData(std::string command)
{
	if (command == "getDeltaMouseX")
	{
		return glm::vec3(this->getDeltaMouseX(), 0.0f, 0.0f);
	}
	else if (command == "getDeltaMouseY")
	{
		return glm::vec3(this->getDeltaMouseY(), 0.0f, 0.0f);
	}
	else if (command == "getMovementSpeed")
	{
		return glm::vec3(this->movementSpeed, 0.0f, 0.0f);
	}

	return glm::vec3(0.0f);
}
