#include <glm/gtx/easing.hpp>
#include <iostream>
#include <map>

#include "cAnimationSystem.h"
#include "cColor.h"
#include "cGameManager.h"
#include "Bones.h"
#include "Character.h"
#include "AnimationData.h"
#include <glm/gtc/quaternion.hpp>
#include "Engine.h"
#include <glm/gtc/type_ptr.hpp>

double cAnimationSystem::keyFrameTimer = 0.0;

extern cVAOManager* g_pMeshManager;

extern Engine engine;

extern GLuint shaderProgramID;

void cAnimationSystem::Update(const std::vector<cGameObject*>& gameObjects) const
{
	if (!m_isAnimating)
	{
		return;
	}

	for (cGameObject* gameObject : gameObjects)
	{
		if (gameObject->animation != nullptr)
		{
			if (gameObject->animation->m_isAnimRunning)
			{
				cAnimation* animation = gameObject->animation;
				double time = animation->m_time;

				AnimatePosition(gameObject, animation, time);
				AnimateRotation(gameObject, animation, time);
				AnimateScale(gameObject, animation, time);
				AnimateEvents(gameObject, animation, time);
			}
		}

		Update(gameObject->m_childrenObjects);
	}
}

void cAnimationSystem::AnimateEvents(cGameObject*& gameObject, cAnimation*& animation, double time) const
{
	if (animation->m_eventKeyFrames.size() == 0)
	{
		return;
	}

	if (animation->m_eventKeyFrames[0].m_time < time)
	{
		cGameManager::GetGMInstance()->DeleteGameObjectById(gameObject->GetObjectMesh()->GetUniqueID());
		//gameObject->DeleteGameObject(true);
	}
}

void cAnimationSystem::AnimatePosition(cGameObject*& gameObject, cAnimation*& animation , double time) const
{
	if (animation->m_positionKeyFrames.size() == 0)
	{
		return;
	}

	if (animation->m_positionKeyFrames.size() == 1)
	{
		sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
		objectPhysics->position = animation->m_positionKeyFrames[0].m_position;
		return;
	}

	int KeyFrameEndIndex = 0;
	for (; KeyFrameEndIndex < animation->m_positionKeyFrames.size(); KeyFrameEndIndex++)
	{
		if (animation->m_positionKeyFrames[KeyFrameEndIndex].m_time > time)
		{
			break;
		}
	}

	if (KeyFrameEndIndex >= animation->m_positionKeyFrames.size())
	{
		sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
		objectPhysics->position = animation->m_positionKeyFrames[KeyFrameEndIndex - 1].m_position;
		return;
	}

	int KeyFrameStartIndex = KeyFrameEndIndex - 1;

	cAnimation::PositionKeyFrame startKeyFrame = animation->m_positionKeyFrames[KeyFrameStartIndex];
	cAnimation::PositionKeyFrame endKeyFrame = animation->m_positionKeyFrames[KeyFrameEndIndex];

	double percent = (time - startKeyFrame.m_time) / (endKeyFrame.m_time - startKeyFrame.m_time);

	double result = 0.0;

	cMesh* gameObejctMesh;
	glm::vec4 newColor;

	switch (endKeyFrame.m_type)
	{
	case cAnimation::EasingType::NoEasing:
		result = percent;

		newColor = cColor::COLOR_WHITE;
		gameObejctMesh = gameObject->GetObjectMesh();
		gameObejctMesh->wholeObjectDebugColourRGBA = newColor;
		gameObejctMesh->bUseDebugColours = true;
		//result = glm::clamp((result + 1.0f) * 0.5f, 0.0f, 1.0f);
		//std::cout << "sin = " << result << std::endl;
		break;
	case cAnimation::EasingType::EaseIn:
		result = glm::sineEaseIn(percent);

		newColor = cColor::COLOR_RED;
		gameObejctMesh = gameObject->GetObjectMesh();
		gameObejctMesh->wholeObjectDebugColourRGBA = newColor;
		gameObejctMesh->bUseDebugColours = true;
		//result = glm::clamp(result, 0.0f, 1.0f);
		//std::cout << "sineEaseIn = " << result << std::endl;
		break;
	case cAnimation::EasingType::EaseOut:
		result = glm::sineEaseOut(percent);

		newColor = cColor::ConvertHexToVec4("FFFF00");
		gameObejctMesh = gameObject->GetObjectMesh();
		gameObejctMesh->wholeObjectDebugColourRGBA = newColor;
		gameObejctMesh->bUseDebugColours = true;
		//std::cout << "sineEaseOut = " << result << std::endl;
		break;
	case cAnimation::EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);

		newColor = cColor::COLOR_GREEN;
		gameObejctMesh = gameObject->GetObjectMesh();
		gameObejctMesh->wholeObjectDebugColourRGBA = newColor;
		gameObejctMesh->bUseDebugColours = true;
		//std::cout << "sineEaseInOut = " << result << std::endl;
		break;
	}

	glm::vec3 delta = endKeyFrame.m_position - startKeyFrame.m_position;
	glm::vec3 newPosition = startKeyFrame.m_position + delta * (float)result;

	sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
	objectPhysics->position = newPosition;
}

void cAnimationSystem::AnimateRotation(cGameObject*& gameObject, cAnimation*& animation, double time) const
{
	if (animation->m_rotationKeyFrames.size() == 0)
	{
		return;
	}

	if (animation->m_rotationKeyFrames.size() == 1)
	{
		sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
		objectPhysics->SetRotation(animation->m_rotationKeyFrames[0].m_rotation);
		return;
	}

	int KeyFrameEndIndex = 0;
	for (unsigned int i = KeyFrameEndIndex; i < animation->m_rotationKeyFrames.size(); i++)
	{
		if (animation->m_rotationKeyFrames[i].m_time > time)
		{
			KeyFrameEndIndex = i;
			break;
		}
	}

	int KeyFrameStartIndex = KeyFrameEndIndex - 1;

	if (KeyFrameEndIndex >= animation->m_rotationKeyFrames.size())
	{
		sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
		objectPhysics->SetRotation(animation->m_rotationKeyFrames[KeyFrameEndIndex - 1].m_rotation);
		return;
	}

	cAnimation::RotationKeyFrame startKeyFrame = animation->m_rotationKeyFrames[KeyFrameStartIndex];
	cAnimation::RotationKeyFrame endKeyFrame = animation->m_rotationKeyFrames[KeyFrameEndIndex];

	double percent = (time - startKeyFrame.m_time) / (endKeyFrame.m_time - startKeyFrame.m_time);

	double result = 0.0;
	switch (endKeyFrame.m_type)
	{
	case cAnimation::EasingType::NoEasing:
		result = percent;
		//result = glm::clamp((result + 1.0f) * 0.5f, 0.0f, 1.0f);
		//std::cout << "sin = " << result << std::endl;
		break;
	case cAnimation::EasingType::EaseIn:
		result = glm::sineEaseIn(percent);

		//result = glm::clamp(result, 0.0f, 1.0f);
		//std::cout << "sineEaseIn = " << result << std::endl;
		break;
	case cAnimation::EasingType::EaseOut:
		result = glm::sineEaseOut(percent);
		//std::cout << "sineEaseOut = " << result << std::endl;
		break;
	case cAnimation::EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);
		//std::cout << "sineEaseInOut = " << result << std::endl;
		break;
	}

	glm::quat newRotation = glm::slerp(startKeyFrame.m_rotation, endKeyFrame.m_rotation, (float)result);

	sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
	objectPhysics->SetRotation(newRotation);
}

void cAnimationSystem::AnimateScale(cGameObject*& gameObject, cAnimation*& animation, double time)const
{
	time *= m_animationSpeed;

	if (animation->m_scaleKeyFrames.size() == 0)
	{
		return;
	}

	if (animation->m_scaleKeyFrames.size() == 1)
	{
		sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
		objectPhysics->SetRotation(animation->m_scaleKeyFrames[0].m_scale);
		return;
	}

	int KeyFrameEndIndex = 0;
	for (unsigned int i = KeyFrameEndIndex; i < animation->m_scaleKeyFrames.size(); i++)
	{
		if (animation->m_scaleKeyFrames[i].m_time > time)
		{
			KeyFrameEndIndex = i;
			break;
		}
	}

	int KeyFrameStartIndex = KeyFrameEndIndex - 1;

	if (KeyFrameEndIndex >= animation->m_scaleKeyFrames.size())
	{
		sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
		objectPhysics->SetRotation(animation->m_scaleKeyFrames[KeyFrameEndIndex - 1].m_scale);
		return;
	}

	cAnimation::ScaleKeyFrame startKeyFrame = animation->m_scaleKeyFrames[KeyFrameStartIndex];
	cAnimation::ScaleKeyFrame endKeyFrame = animation->m_scaleKeyFrames[KeyFrameEndIndex];

	double percent = (time - startKeyFrame.m_time) / (endKeyFrame.m_time - startKeyFrame.m_time);

	double result = 0.0;

	cMesh* gameObejctMesh;
	glm::vec4 newColor;

	switch (endKeyFrame.m_type)
	{
	case cAnimation::EasingType::NoEasing:
		result = percent;

		newColor = cColor::COLOR_WHITE;
		gameObejctMesh = gameObject->GetObjectMesh();
		gameObejctMesh->wholeObjectDebugColourRGBA = newColor;
		gameObejctMesh->bUseDebugColours = true;
		break;
	case cAnimation::EasingType::EaseIn:
		result = glm::sineEaseIn(percent);

		newColor = cColor::COLOR_RED;
		gameObejctMesh = gameObject->GetObjectMesh();
		gameObejctMesh->wholeObjectDebugColourRGBA = newColor;
		gameObejctMesh->bUseDebugColours = true;
		break;
	case cAnimation::EasingType::EaseOut:
		result = glm::sineEaseOut(percent);

		newColor = cColor::ConvertHexToVec4("FFFF00");
		gameObejctMesh = gameObject->GetObjectMesh();
		gameObejctMesh->wholeObjectDebugColourRGBA = newColor;
		gameObejctMesh->bUseDebugColours = true;

		break;
	case cAnimation::EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);

		newColor = cColor::COLOR_GREEN;
		gameObejctMesh = gameObject->GetObjectMesh();
		gameObejctMesh->wholeObjectDebugColourRGBA = newColor;
		gameObejctMesh->bUseDebugColours = true;
		break;
	}

	glm::vec3 delta = endKeyFrame.m_scale - startKeyFrame.m_scale;
	glm::vec3 newPosition = startKeyFrame.m_scale + delta * (float)result;

	cMesh* objectMesh = gameObject->GetObjectMesh();
	sPhysicsProperties* objectPhysics = gameObject->GetObjectPhysics();
	objectMesh->SetScale(newPosition);
}

void cAnimationSystem::StartAllAnimations(const std::vector<cGameObject*>& gameObjects) const
{
	for (cGameObject* gameObject : gameObjects)
	{
		if (gameObject->animation)
		{
			gameObject->animation->m_isAnimRunning = true;
		}
	}
}

void cAnimationSystem::StopAllAnimations(const std::vector<cGameObject*>& gameObjects) const
{
	for (cGameObject* gameObject : gameObjects)
	{
		if (gameObject->animation)
		{
			gameObject->animation->m_isAnimRunning = false;
		}
	}
}

void cAnimationSystem::SetAnimationSpeed(const float animationSpeed)
{
	std::cout << "Animation speed: " << animationSpeed << std::endl;
	m_animationSpeed = animationSpeed;
}

void cAnimationSystem::ReverseAnimations(std::vector<cGameObject*>& gameObjects)
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		cAnimation* animation = gameObjects[i]->animation;

		if (animation != nullptr)
		{
			std::vector<double> currentTimesPos;
			//read
			for (unsigned int j = 0; j < animation->m_positionKeyFrames.size(); j++)
			{
				if (animation->m_positionKeyFrames.size() > 1)
				{
					currentTimesPos.push_back(animation->m_positionKeyFrames[j].m_time);
				}
			}
			std::reverse(animation->m_positionKeyFrames.begin(), animation->m_positionKeyFrames.end());
			//load
			for (unsigned int j = 0; j < animation->m_positionKeyFrames.size(); j++)
			{
				animation->m_positionKeyFrames[j].m_time = currentTimesPos[animation->m_positionKeyFrames.size() - j];
			}


			std::vector<double> currentTimesRot;
			//read
			for (unsigned int j = 0; j < animation->m_rotationKeyFrames.size(); j++)
			{
				if (animation->m_rotationKeyFrames.size() > 1)
				{
					currentTimesRot.push_back(animation->m_rotationKeyFrames[j].m_time);
				}
			}
			std::reverse(animation->m_rotationKeyFrames.begin(), animation->m_rotationKeyFrames.end());
			//load
			for (unsigned int j = 0; j < animation->m_rotationKeyFrames.size(); j++)
			{
				animation->m_rotationKeyFrames[j].m_time = currentTimesRot[animation->m_rotationKeyFrames.size() - j];
			}

			std::vector<double> currentTimesScl;
			//read
			for (unsigned int j = 0; j < animation->m_scaleKeyFrames.size(); j++)
			{
				if (animation->m_scaleKeyFrames.size() > 1)
				{
					currentTimesScl.push_back(animation->m_scaleKeyFrames[j].m_time);
				}
			}
			std::reverse(animation->m_scaleKeyFrames.begin(), animation->m_scaleKeyFrames.end());
			//load
			for (unsigned int j = 0; j < animation->m_scaleKeyFrames.size(); j++)
			{
				animation->m_scaleKeyFrames[j].m_time = currentTimesScl[animation->m_scaleKeyFrames.size() - j];
			}

			//if (animation->m_rotationKeyFrames.size() > 1)
			//{
			//	std::reverse(animation->m_rotationKeyFrames.begin(), animation->m_rotationKeyFrames.end());
			//}

			//if (animation->m_scaleKeyFrames.size() > 1)
			//{
			//	std::reverse(animation->m_scaleKeyFrames.begin(), animation->m_scaleKeyFrames.end());
			//}
		}
	}
}

void cAnimationSystem::UpdateCharacter(const std::vector<cGameObject*>& gameObjects, double time)
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		glm::mat4 origin = glm::mat4(1.0f);
		glm::mat4 rootTransformation = glm::mat4(1.f);

		//sModelDrawInfo* modelDrawInfo = nullptr;
		//::g_pMeshManager->FindDrawInfoPointerByModelName("Captain.dae", modelDrawInfo, shaderProgramID);


		Bones::CharacterAnimation* meshAnimation = gameObjects[i]->currentCharacterAnimation;

		AnimationGroup* animationGroup = gameObjects[i]->currentAnimationGroup;

		CalculateMatrices(gameObjects[i], animationGroup, gameObjects[i]->GetObjectMesh(), meshAnimation, meshAnimation->RootNode, rootTransformation, time);

		if (gameObjects[i]->isBlending)
		{
			gameObjects[i]->currentBlendingTime -= time;

			std::cout << "gameObjects[i]->currentBlendingTime" << gameObjects[i]->currentBlendingTime << std::endl;

			if (gameObjects[i]->currentBlendingTime < 0.0f)
			{
				gameObjects[i]->isBlending = false;
				gameObjects[i]->currentBlendingTime = gameObjects[i]->blendingTime;
			}
		}

		//cMesh sledMesh;
		//sledMesh.friendlyName = "captain";
		//sledMesh.meshName = modelDrawInfo->meshName;
		//sledMesh.bUseDebugColours = false;
		//sledMesh.textureName[0] = "PolygonAncientWorlds_Texture_01_A.bmp";
		//sledMesh.textureRatios[0] = 1.0f;
		//sledMesh.UseBonesUL = true;
	}

	//for (int i = 0; i < modelDrawInfo->BoneInfoVec.size(); ++i)
	//{
	//	//glUniformMatrix4fv(::g_pMeshManager->, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));// gameObject->m_Model->BoneInfoVec[i].GlobalTransformation));
	//	//RenderBoneDebug();


	//}

	//for (GameObject* child : gameObject->m_Children)
	//{
	//	RenderGameObject(child, ModelMatrix);
	//}



}

AnimationData* cAnimationSystem::FindAnimationDat(AnimationGroup* animationGroup, std::string nodeName)
{
	for (unsigned int i = 0; i < animationGroup->animations.size(); i++)
	{
		if (animationGroup->animations[i]->name == nodeName)
		{
			return animationGroup->animations[i];
		}
	}

	return nullptr;
}


void cAnimationSystem::CalculateMatrices(cGameObject* gameObject, AnimationGroup* animationGroup, cMesh* model, Bones::CharacterAnimation* animation, Bones::Node* node, glm::mat4& parentTransformationMatrix, double keyFrameTime)
{
	std::string nodeName(node->Name);
	glm::mat4 transformationMatrix = node->Transformation;

	if (gameObject->isBlending)
	{
		//std::cout << "gameObject->blendingTime" << gameObject->currentBlendingTime << std::endl;

		float blendingForPrevious = (float)gameObject->currentBlendingTime / gameObject->blendingTime;
		float blendingForCurrent = 1.0f - blendingForPrevious;

		//std::cout << "blendingForPrevious: " << blendingForPrevious << std::endl;
		//std::cout << "blendingForCurrent: " << blendingForCurrent << std::endl;

		//if (blendingForCurrent + blendingForPrevious != 1.0f)
		//{
		//	std::cout << "(blendingForCurrent + blendingForPrevious != 1.0f)" << std::endl;
		//}

		AnimationData* previuosData = FindAnimationDat(gameObject->previousAnimationGroup, nodeName);

		glm::vec3 previousPosition = glm::vec3(0.0f);
		glm::vec3 previousScale = glm::vec3(0.0f);
		glm::quat previousRotation = glm::quat(glm::vec3(0.0f));

		if (previuosData != nullptr)
		{
			previuosData->timer += keyFrameTime * gameObject->previousCharacterAnimation->TicksPerSecond;

			if (previuosData->timer > previuosData->duration)
			{
				//std::cout << "Animation finished a cycle" << std::endl;
				previuosData->timer = 0.0;
			}

			previousPosition = GetAnimationPosition(previuosData, previuosData->timer);	/// POSITION update in previous function
			previousScale = GetAnimationScale(previuosData, previuosData->timer);	/// SCALE from your project
			previousRotation = GetAnimationRotation(previuosData, previuosData->timer);	/// ROTATION update from previous function
		}

		AnimationData* currentData = FindAnimationDat(gameObject->currentAnimationGroup, nodeName);

		glm::vec3 currentPosition = glm::vec3(0.0f);
		glm::vec3 currentScale = glm::vec3(0.0f);
		glm::quat currentRotation = glm::quat(glm::vec3(0.0f));

		if (currentData != nullptr)
		{
			currentData->timer += keyFrameTime * gameObject->currentCharacterAnimation->TicksPerSecond;

			if (currentData->timer > currentData->duration)
			{
				//std::cout << "Animation finished a cycle" << std::endl;
				currentData->timer = 0.0;
			}

			currentPosition = GetAnimationPosition(currentData, currentData->timer);	/// POSITION update in previous function
			currentScale = GetAnimationScale(currentData, currentData->timer);	/// SCALE from your project
			currentRotation = GetAnimationRotation(currentData, currentData->timer);	/// ROTATION update from previous function
		}

		if (currentData != nullptr && previuosData != nullptr)
		{
			glm::vec3 position = (previousPosition * blendingForPrevious) + (currentPosition * blendingForCurrent);
			glm::vec3 scale = (previousScale * blendingForPrevious) + (currentScale * blendingForCurrent);
			glm::quat rotation = (blendingForPrevious * previousRotation) + (blendingForCurrent * currentRotation);

			// calculate the matrices
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
			glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);

			transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		}
		else if (currentData != nullptr)
		{
			glm::vec3 position = currentPosition;
			glm::vec3 scale = currentScale;
			glm::quat rotation = currentRotation;

			// calculate the matrices
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
			glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);

			transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		}


	}
	else
	{
		AnimationData* data = FindAnimationDat(gameObject->currentAnimationGroup, nodeName);

		if (data != nullptr)
		{
			data->timer += keyFrameTime * gameObject->currentCharacterAnimation->TicksPerSecond;

			if (data->timer > data->duration)
			{
				//std::cout << "Animation finished a cycle" << std::endl;
				data->timer = 0.0;
			}

			//std::cout << "Animation timer: " << data->timer << std::endl;

			glm::vec3 position = GetAnimationPosition(data, data->timer);	/// POSITION update in previous function
			glm::vec3 scale = GetAnimationScale(data, data->timer);	/// SCALE from your project
			glm::quat rotation = GetAnimationRotation(data, data->timer);	/// ROTATION update from previous function

			// calculate the matrices
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
			glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);

			transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		}


	}

	// Calculate the global transformation
	glm::mat4 globalTransformation = parentTransformationMatrix * transformationMatrix;

	// If there is a bone associated with this name, assign the global transformation
	std::map<std::string, int>::iterator boneMapIt = model->BoneNameToIdMap.find(nodeName);
	if (boneMapIt != model->BoneNameToIdMap.end())
	{
		Bones::BoneInfo& boneInfo = model->BoneInfoVec[boneMapIt->second];
		boneInfo.FinalTransformation = model->GlobalInverseTransformation * globalTransformation * boneInfo.BoneOffset;
		boneInfo.GlobalTransformation = globalTransformation;
	}

	// Calculate all children
	for (int i = 0; i < node->Children.size(); ++i)
	{
		CalculateMatrices(gameObject, animationGroup, model, animation, node->Children[i], globalTransformation, keyFrameTime);
	}

}

glm::vec3 cAnimationSystem::GetAnimationPosition(AnimationData*& animationData, double time)
{
	if (animationData->m_positionKeyFrames.size() == 0)
	{
		return glm::vec3(0.0f);
	}

	if (animationData->m_positionKeyFrames.size() == 1)
	{
		return animationData->m_positionKeyFrames[0].m_position;
	}

	int KeyFrameEndIndex = 0;
	for (; KeyFrameEndIndex < animationData->m_positionKeyFrames.size(); KeyFrameEndIndex++)
	{
		if (animationData->m_positionKeyFrames[KeyFrameEndIndex].m_time > time)
		{
			break;
		}
	}

	if (KeyFrameEndIndex >= animationData->m_positionKeyFrames.size())
	{
		return animationData->m_positionKeyFrames[KeyFrameEndIndex - 1].m_position;;
	}

	int KeyFrameStartIndex = KeyFrameEndIndex - 1;

	if (KeyFrameStartIndex < 0)
	{
		return animationData->m_positionKeyFrames[0].m_position;
	}

	cAnimation::PositionKeyFrame startKeyFrame = animationData->m_positionKeyFrames[KeyFrameStartIndex];
	cAnimation::PositionKeyFrame endKeyFrame = animationData->m_positionKeyFrames[KeyFrameEndIndex];

	double percent = (time - startKeyFrame.m_time) / (endKeyFrame.m_time - startKeyFrame.m_time);
	double result = 0.0;

	switch (endKeyFrame.m_type)
	{
	case cAnimation::EasingType::NoEasing:
		result = percent;
		break;
	case cAnimation::EasingType::EaseIn:
		result = glm::sineEaseIn(percent);
		break;
	case cAnimation::EasingType::EaseOut:
		result = glm::sineEaseOut(percent);
		break;
	case cAnimation::EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);
		break;
	}

	glm::vec3 delta = endKeyFrame.m_position - startKeyFrame.m_position;
	glm::vec3 newPosition = startKeyFrame.m_position + delta * (float)result;

	return newPosition;
}

glm::quat cAnimationSystem::GetAnimationRotation(AnimationData*& animationData, double time)
{
	if (animationData->m_rotationKeyFrames.size() == 0)
	{
		return glm::quat(1.0f,0.0f,0.0f,0.0f);
	}

	if (animationData->m_rotationKeyFrames.size() == 1)
	{
		return animationData->m_rotationKeyFrames[0].m_rotation;
	}

	int KeyFrameEndIndex = 0;
	for (unsigned int i = KeyFrameEndIndex; i < animationData->m_rotationKeyFrames.size(); i++)
	{
		if (animationData->m_rotationKeyFrames[i].m_time > time)
		{
			KeyFrameEndIndex = i;
			break;
		}
	}

	int KeyFrameStartIndex = KeyFrameEndIndex - 1;

	if (KeyFrameEndIndex > animationData->m_rotationKeyFrames.size())
	{
		return animationData->m_rotationKeyFrames[KeyFrameEndIndex].m_rotation;
	}

	if (KeyFrameStartIndex < 0)
	{
		return animationData->m_rotationKeyFrames[0].m_rotation;
	}

	cAnimation::RotationKeyFrame startKeyFrame = animationData->m_rotationKeyFrames[KeyFrameStartIndex];
	cAnimation::RotationKeyFrame endKeyFrame = animationData->m_rotationKeyFrames[KeyFrameEndIndex];

	double percent = (time - startKeyFrame.m_time) / (endKeyFrame.m_time - startKeyFrame.m_time);

	double result = 0.0;
	switch (endKeyFrame.m_type)
	{
	case cAnimation::EasingType::NoEasing:
		result = percent;
		break;
	case cAnimation::EasingType::EaseIn:
		result = glm::sineEaseIn(percent);
		break;
	case cAnimation::EasingType::EaseOut:
		result = glm::sineEaseOut(percent);
		break;
	case cAnimation::EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);
		break;
	}

	glm::quat newRotation = glm::slerp(startKeyFrame.m_rotation, endKeyFrame.m_rotation, (float)result);

	return newRotation;
}

glm::vec3 cAnimationSystem::GetAnimationScale(AnimationData*& animationData, double time)
{
	if (animationData->m_scaleKeyFrames.size() == 0)
	{
		return glm::vec3(1.0f);
	}

	if (animationData->m_scaleKeyFrames.size() == 1)
	{
		return animationData->m_scaleKeyFrames[0].m_scale;
	}

	int KeyFrameEndIndex = 0;
	for (unsigned int i = KeyFrameEndIndex; i < animationData->m_scaleKeyFrames.size(); i++)
	{
		if (animationData->m_scaleKeyFrames[i].m_time > time)
		{
			KeyFrameEndIndex = i;
			break;
		}
	}

	int KeyFrameStartIndex = KeyFrameEndIndex - 1;

	if (KeyFrameEndIndex >= animationData->m_scaleKeyFrames.size())
	{
		return animationData->m_scaleKeyFrames[KeyFrameEndIndex - 1].m_scale;
	}

	if (KeyFrameStartIndex < 0)
	{
		return animationData->m_scaleKeyFrames[0].m_scale;
	}

	cAnimation::ScaleKeyFrame startKeyFrame = animationData->m_scaleKeyFrames[KeyFrameStartIndex];
	cAnimation::ScaleKeyFrame endKeyFrame = animationData->m_scaleKeyFrames[KeyFrameEndIndex];

	double percent = (time - startKeyFrame.m_time) / (endKeyFrame.m_time - startKeyFrame.m_time);

	double result = 0.0;

	//cMesh* gameObejctMesh;
	//glm::vec4 newColor;

	switch (endKeyFrame.m_type)
	{
	case cAnimation::EasingType::NoEasing:
		result = percent;

		break;
	case cAnimation::EasingType::EaseIn:
		result = glm::sineEaseIn(percent);

		break;
	case cAnimation::EasingType::EaseOut:
		result = glm::sineEaseOut(percent);

		break;
	case cAnimation::EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);

		break;
	}

	glm::vec3 delta = endKeyFrame.m_scale - startKeyFrame.m_scale;
	glm::vec3 newPosition = startKeyFrame.m_scale + delta * (float)result;

	return newPosition;
}

bool cAnimationSystem::SetAnimationByName(std::string animationName)
{
	for (AnimationGroup* animation : animationGroups)
	{
		if (animation->name == animationName)
		{
			//currentAnimationGroup = animation;
			return true;
		}
	}

	return false;
}

Bones::CharacterAnimation* cAnimationSystem::GetCharacterAnimationByName(std::string animationName)
{
	for (Bones::CharacterAnimation* characterAnimation : characterAnimations)
	{
		if (characterAnimation->Name == animationName)
		{
			return characterAnimation;
		}
	}

	return nullptr;
}

AnimationGroup* cAnimationSystem::GetAnimationGroupByName(std::string animationName)
{
	for (AnimationGroup* animationGroup : animationGroups)
	{
		if (animationGroup->name == animationName)
		{
			return animationGroup;
		}
	}

	return nullptr;
}

void cAnimationSystem::Awake(GLFWwindow& window)
{
}

void cAnimationSystem::Start(GLFWwindow& window)
{
}

void cAnimationSystem::EarlyUpdate(GLFWwindow& window, double deltaTime)
{
}

void cAnimationSystem::Update(GLFWwindow& window, double deltaTime)
{
}

void cAnimationSystem::LateUpdate(GLFWwindow& window, double deltaTime)
{
}

void cAnimationSystem::End(GLFWwindow& window)
{
}

std::string cAnimationSystem::GetSystemType()
{
	return "cAnimationSystem";
}

