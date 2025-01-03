#include "CharacterController.h"
#include "cScene.h"
#include "Engine.h"
#include "cColor.h"
#include "cCamera.h"

extern cScene* g_currentScene;

extern Engine engine;

extern iCamera* g_camera;

CharacterController::CharacterController()
{

}

float CharacterController::GetHeightInTerrain(glm::vec3 position)
{
	float vertexY = terrainMesh->GetVertexYByXAndZ(position.x, position.z);

	//engine.g_DrawDebugSphere(glm::vec3(position.x, vertexY, position.z), 10.0f, cColor::COLOR_RED, 0.0f);

	return vertexY + height;
}

void CharacterController::MoveToDirection(glm::vec3 direction)
{
	characterGameObject->FaceDirection(direction);
}

void CharacterController::UpdateCharacterAnimation(double deltaTime)
{
	glm::vec3 velocity = characterGameObject->GetObjectPhysics()->velocity;

	AnimationGroup* currentAnimation = characterGameObject->currentAnimationGroup;

	float vectorSize = glm::length(velocity);

	float xVelocity = velocity.x;
	float zVelocity = velocity.z;


	if (xVelocity == 0.0f && zVelocity == 0.0f)
	{
		if (currentAnimation->name == "Captain_Idle.dae")
		{
			return;
		}

		characterGameObject->SetAnimation("Captain_Idle.dae");
	}
	else
	{
		if (isPressingShift)
		{
			if (currentAnimation->name == "Captain_Running.dae")
			{
				return;
			}

			characterGameObject->SetAnimation("Captain_Running.dae");
			characterGameObject->objectSpeed = runningSpeed;
		}
		else
		{
			if (currentAnimation->name == "Captain_Walking.dae")
			{
				return;
			}

			characterGameObject->SetAnimation("Captain_Walking.dae");
			characterGameObject->objectSpeed = walkingSpeed;
		}


	}
}

void CharacterController::Awake(GLFWwindow& window)
{
}

void CharacterController::Start(GLFWwindow& window)
{
	characterGameObject = g_currentScene->GetPlayer();
	terrainMesh = g_currentScene->GetMeshByFriendlyName("island");

	g_camera->Start();
}

void CharacterController::EarlyUpdate(GLFWwindow& window, double deltaTime)
{

}

void CharacterController::Update(GLFWwindow& window, double deltaTime)
{
	g_camera->Update(deltaTime);

	sPhysicsProperties* characterPhysics = characterGameObject->GetObjectPhysics();

	g_currentScene->GetCameras()[1]->LookAtTarget(characterPhysics->position);


	bool isAboveMesh = true;

	float newHeight = terrainMesh->IsAboveMesh(characterPhysics->position, isAboveMesh);

	if (isAboveMesh)
	{
		if (characterPhysics->position.y >= 0)
		{
			characterPhysics->acceleration.y -= 10.0f;
			//characterPhysics->velocity.y = -40.0f;
		}
		else
		{
			characterPhysics->acceleration.y = 0.0f;

			characterPhysics->velocity.y = 0.0f;
		}

		g_currentScene->GetCameras()[1]->AddVelocityToCamera('S', glm::vec3(0.0f, -40.0f, 0.0f));

		//characterPhysics->position.y -= 1.0f * deltaTime;
	}
	else
	{
		//std::cout << "Collided with terrain" << std::endl;

		characterPhysics->position.y = newHeight;
		characterPhysics->velocity.y = 0.0f;
	}

	characterPhysics->velocity.x = horizontalMove * characterGameObject->objectSpeed;
	characterPhysics->velocity.z = verticalMove * characterGameObject->objectSpeed;

	g_camera->RemoveVelocityFromCamera('c');
	g_camera->AddVelocityToCamera('c', characterPhysics->velocity);

	if ((horizontalMove > 0.1f || horizontalMove < -0.1f) || (verticalMove > 0.1f || verticalMove < -0.1f))
	{
		MoveToDirection(glm::vec3(characterPhysics->velocity.x,
			0.0f,
			characterPhysics->velocity.z));
	}

	UpdateCharacterAnimation(deltaTime);

	if (characterPhysics->position.y < 0.0f)
	{
		characterPhysics->position.y = 0.0f;
		characterPhysics->velocity.y = 0.0f;
		characterPhysics->acceleration.y = 0.0f;
	}
}

void CharacterController::LateUpdate(GLFWwindow& window, double deltaTime)
{
}

void CharacterController::End(GLFWwindow& window)
{
}

std::string CharacterController::GetSystemType()
{
	return "CharacterController";
}
