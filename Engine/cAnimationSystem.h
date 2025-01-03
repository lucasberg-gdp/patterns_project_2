#pragma once
#include <vector>
#include "cGameObject.h"
#include "cVAOManager/cVAOManager.h"
#include "Bones.h"
#include "AnimationData.h"
#include "iSystem.h"
#include "cMesh.h"

class cAnimationSystem : public iSystem
{
	float m_animationSpeed = 1.0f;

	void AnimatePosition(cGameObject*& gameObject, cAnimation*& animation, double time) const;
	void AnimateRotation(cGameObject*& gameObject, cAnimation*& animation, double time) const;
	void AnimateScale(cGameObject*& gameObject, cAnimation*& animation, double time) const;
	void AnimateEvents(cGameObject*& gameObject, cAnimation*& animation, double time) const;

	static double keyFrameTimer;
public:

	AnimationData* FindAnimationDat(AnimationGroup* animationGroup, std::string nodeName);

	bool m_isAnimating = false;

	void Update(const std::vector<cGameObject*>& gameObjects) const;

	void StartAllAnimations(const std::vector<cGameObject*>& gameObjects) const;
	void StopAllAnimations(const std::vector<cGameObject*>& gameObjects)const;

	void SetAnimationSpeed(const float animationSpeed);
	void ReverseAnimations(std::vector<cGameObject*>& gameObjects);

	void UpdateCharacter(const std::vector<cGameObject*>& gameObjects, double time);

	void CalculateMatrices(cGameObject* gameObject, AnimationGroup* animationGroup, cMesh* model, Bones::CharacterAnimation* animation, Bones::Node* node, glm::mat4& parentTransformationMatrix, double keyFrameTime) ;

	glm::vec3 GetAnimationPosition(AnimationData*& animationData, double frame);
	glm::quat GetAnimationRotation(AnimationData*& animationData, double frame);
	glm::vec3 GetAnimationScale(AnimationData*& animationData, double frame);

	bool SetAnimationByName(std::string animationName);

	Bones::CharacterAnimation* GetCharacterAnimationByName(std::string animationName);

	AnimationGroup* GetAnimationGroupByName(std::string animationName);

	std::vector< Bones::CharacterAnimation* > characterAnimations;
	
	std::vector< AnimationGroup* > animationGroups;
	AnimationGroup* currentAnimationGroup = nullptr;

	// Inherited via iSystem
	void Awake(GLFWwindow& window) override;
	void Start(GLFWwindow& window) override;
	void EarlyUpdate(GLFWwindow& window, double deltaTime) override;
	void Update(GLFWwindow& window, double deltaTime) override;
	void LateUpdate(GLFWwindow& window, double deltaTime) override;
	void End(GLFWwindow& window) override;

	// Inherited via iSystem
	std::string GetSystemType() override;
};
