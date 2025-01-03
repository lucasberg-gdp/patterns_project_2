#include "cLionPlayer.h"
#include "cCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

extern iCamera* g_camera;

void cLionPlayer::MoveTowards(glm::vec3 direction)
{
	if (direction != glm::vec3(0.0f))
	{
		if (glm::normalize(direction).z == -1.0f)
		{
			GetObjectMesh()->setDrawOrientation(glm::quat(0.0f,0.0f,1.0f,0.0f));
		}
		else
		{
			direction = glm::normalize(direction);

			glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 axis = glm::cross(forward, direction);
			float angle = glm::acos(glm::dot(forward, direction));
			glm::quat newOrientation = glm::angleAxis(angle, axis);
			GetObjectMesh()->setDrawOrientation(glm::normalize(newOrientation));
		}
	}

	GetObjectPhysics()->velocity = direction * objectSpeed;
}

void cLionPlayer::AdjustOrientation(glm::vec3 eulerOrientation)
{
	GetObjectMesh()->setRotationFromEuler(eulerOrientation);
}

void cLionPlayer::SetHorizontalMovement(float horizontal)
{
	horizontalMovement = horizontal;
}

void cLionPlayer::SetVerticalMovement(float vertical)
{
	verticalMovement = vertical;
}

void cLionPlayer::Update(double deltaTime)
{
	g_camera->setAt(GetObjectMesh()->drawPosition);

	glm::vec3 newVelocity = glm::vec3(horizontalMovement, 0.0f, verticalMovement );
	MoveTowards(newVelocity);
}
