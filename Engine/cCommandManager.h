#pragma once

#include <vector>
#include "cCommandGroup.h"
#include <string>
#include "sPhysicsProperties.h"

class cCommandManager
{
	struct LocationTrigger {
		bool m_IsTriggered = false;
		glm::vec3 m_Position = glm::vec3(0.0f);
		std::string m_FriendlyName;
		cCommandGroup* m_CommandGroup = nullptr;
		sPhysicsProperties* m_PhysicsToCheck = nullptr;
		float m_TriggerDistance = 0.0f;

		LocationTrigger(
			glm::vec3 position, 
			std::string friendlyName,
			cCommandGroup* commandGroup,
			sPhysicsProperties* m_physicsToCheck, 
			float triggerDistance
		)
		{
			m_Position = position;
			m_FriendlyName = friendlyName;
			m_CommandGroup = commandGroup;
			m_PhysicsToCheck = m_physicsToCheck;
			m_TriggerDistance = triggerDistance;
		}
	};

	int m_CurrentGroupIndex = 0;
	std::vector< cCommandGroup* > m_CommandGroups;
	std::vector< LocationTrigger* > m_TriggerCommandGroups;
	std::vector< cCommandGroup* > m_SceneParallelCommandGroups;
	std::vector< cCommandGroup* > m_SceneLoopingCommandGroups;

	bool m_IsActive = false;

public:
	bool m_IsCreatingCommandGroup = false;
	cCommandGroup* m_CommandGroupBeingCreated = nullptr;

	cCommandManager();

	void Start();
	void Update(double deltaTime);

	void AddCommandGroup(cCommandGroup* commandGroup);
	void AddSceneParallelCommandGroup(cCommandGroup* commandGroup, bool isLooping);
	void AddTriggerCommandGroups(
		glm::vec3 position,
		cCommandGroup* commandGroup,
		std::string friendlyName,
		sPhysicsProperties* physicsToCheck,
		float triggerDistance
		);

	void AddLoopingCommandGroup(cCommandGroup* commandGroup);

	void CheckLocationTriggers();

	bool AreAllCommandsFinished();
};

