#pragma once
#include "cMesh.h"
#include "sPhysicsProperties.h"
#include "cAnimation.h"
#include "iComponent.h"
#include "AnimationData.h"
#include "cLuaManager.h"

class cGameObject
{
	bool m_deleteGameObject = false;

	glm::vec3 m_InitialPosition = glm::vec3(0.0f);
	glm::quat m_InitialOrientation = glm::quat(glm::vec3(0.0f));;
	glm::vec3 m_InitialEulerOrientation = glm::vec3(0.0f);
public:
	float objectSpeed = 50.0f;

	cMesh* m_objectMesh = nullptr;
	sPhysicsProperties* m_objectPhysics = nullptr;
	cGameObject* m_parentObject = nullptr;
	std::string tag;

	glm::vec3 target = glm::vec3(0.0f);

	std::vector<cGameObject* > m_childrenObjects;

	cGameObject(bool hasPhysics = false);
	cGameObject(cMesh* mesh, bool hasPhysics = false);
	cGameObject(std::string name, cMesh* mesh, bool hasPhysics = false);

	cMesh* GetObjectMesh();
	sPhysicsProperties* GetObjectPhysics();

	std::vector< iComponent* > components;

	cAnimation* animation = nullptr;

	cAnimation* currentAnimation = nullptr;
	bool isAnimating = false;

	Bones::CharacterAnimation* previousCharacterAnimation = nullptr;
	Bones::CharacterAnimation* currentCharacterAnimation = nullptr;

	std::vector < AnimationGroup* > animationGroups;
	AnimationGroup* currentAnimationGroup = nullptr;
	AnimationGroup* previousAnimationGroup = nullptr;

	cLuaManager* m_LuaManager = nullptr;
	bool CreateLuaManager(std::vector< cGameObject* >* p_vecGOs);
	void SetLuaBrainObjectID(void);

	void AddAnimation(std::string animationName);
	void SetAnimation(std::string animationName);

	bool isBlending = false;
	float blendingTime = 1.0f;
	double currentBlendingTime = 0.0;

	void SetParent(cGameObject* parent);
	void SetParentWithWorldPosition(cGameObject* parent);
	void AddChild(cGameObject* child);

	std::vector<cGameObject*> GetChildren();

	void DeleteGameObject(bool deleteObject);

	bool IsToDelete();

	void FaceDirection(glm::vec3 direction);

	void RestartTransform();

	// Game cycle
	//virtual void Awake();
	//virtual void Start();
	//virtual void EarlyUpdate();
	virtual void Update(double deltaTime);
	//virtual void LateUpdate();
	//virtual void ShutDown();
};

