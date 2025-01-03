#include "cCommandManager.h"

cCommandManager::cCommandManager()
{

}

void cCommandManager::Start()
{
	if (m_CommandGroups.size() > 0)
	{
		m_CommandGroups[0]->Start();
	}

	m_IsActive = true;

	for (cCommandGroup* loopingCommandGroup : m_SceneLoopingCommandGroups)
	{
		loopingCommandGroup->Start();
	}
}

void cCommandManager::Update(double deltaTime)
{
	if (!m_IsActive)
	{
		return;
	}
	
	if (m_CommandGroups.size() > 0)
	{
		cCommandGroup* currentCommand = m_CommandGroups[m_CurrentGroupIndex];

		if (!currentCommand->isFinished())
		{
			currentCommand->Update(deltaTime);
		}
		else
		{
			m_CurrentGroupIndex++;

			if (m_CurrentGroupIndex >= m_CommandGroups.size())
			{
				m_IsActive = false;
			}
			else
			{
				m_CommandGroups[m_CurrentGroupIndex]->Start();
			}
		}
	}

	CheckLocationTriggers();

	for (cCommandGroup* loopingCommandGroup : m_SceneLoopingCommandGroups)
	{
		if (!loopingCommandGroup->isFinished())
		{
			loopingCommandGroup->Update(deltaTime);
		}
		else
		{
			loopingCommandGroup->Start();
		}
	}
}

void cCommandManager::AddCommandGroup(cCommandGroup* commandGroup)
{
	m_CommandGroups.push_back(commandGroup);
}

void cCommandManager::AddSceneParallelCommandGroup(cCommandGroup* commandGroup, bool isLooping)
{
	m_SceneParallelCommandGroups.push_back(commandGroup);
}

void cCommandManager::AddTriggerCommandGroups(glm::vec3 position, cCommandGroup* commandGroup, std::string friendlyName, sPhysicsProperties* physicsToCheck, float triggerDistance)
{
	LocationTrigger* newLocationTrigger = new LocationTrigger(
		position, friendlyName, commandGroup, physicsToCheck, triggerDistance
	);

	m_TriggerCommandGroups.push_back(newLocationTrigger);
}

void cCommandManager::AddLoopingCommandGroup(cCommandGroup* commandGroup)
{
	m_SceneLoopingCommandGroups.push_back(commandGroup);
}

void cCommandManager::CheckLocationTriggers()
{
	for (LocationTrigger* locationTrigger : m_TriggerCommandGroups)
	{
		if (!locationTrigger->m_IsTriggered)
		{
			float distance = glm::distance(locationTrigger->m_Position, locationTrigger->m_PhysicsToCheck->position);

			if (distance < locationTrigger->m_TriggerDistance)
			{
				this->AddCommandGroup(locationTrigger->m_CommandGroup);
				locationTrigger->m_IsTriggered = true;
				//locationTrigger->m_CommandGroup->Start();
			}
		}
	}
}

bool cCommandManager::AreAllCommandsFinished()
{
	return !m_IsActive;
}
