#include <cstddef>
#include <vector>
#include <iostream>
#include <set>

#include "cGameManager.h"
#include "sPhysicsProperties.h"
#include "cPhysics.h"
#include "iCamera.h"
#include <algorithm>
#include "cScene.h"

extern cScene* g_currentScene;
//extern std::vector< cMesh* > g_vec_pMeshesToDraw;
//extern cPhysics* g_pPhysics;
//extern std::vector<iCamera*> cameras;

cMesh* cGameManager::FindMeshById(unsigned id)
{
	std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	for (unsigned int index = 0; index != meshesToDraw.size(); index++)
	{
		if (meshesToDraw[index]->GetUniqueID() == id)
		{
			return meshesToDraw[index];
		}
	}

	return nullptr;
}

cMesh* cGameManager::FindMeshByFriendlyName(std::string friendlyNameToFind)
{
	std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	for (unsigned int index = 0; index != meshesToDraw.size(); index++)
	{
		if (meshesToDraw[index]->friendlyName == friendlyNameToFind)
		{
			// Found it
			return meshesToDraw[index];
		}
	}
	return NULL;
}

std::vector<cMesh*> cGameManager::FindAllMeshesByObjectType(std::string objectType)
{
	std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	std::vector<cMesh*> allObjectOfType;

	for (int i = 0; i < meshesToDraw.size(); i++)
	{
		if (meshesToDraw[i]->objectType == objectType)
		{
			allObjectOfType.push_back(meshesToDraw[i]);
		}
	}

	return allObjectOfType;
}

cGameObject* cGameManager::FindGameObjectByFriendlyTag(std::string objectTag, std::vector<cGameObject*>& gameObjects)
{
	for (cGameObject* gameObject: gameObjects)
	{
		if (gameObject->tag == objectTag)
		{
			return gameObject;
		}
	}

	return nullptr;
}

std::vector<std::string> cGameManager::GetNameOfAllObjectsInVector()
{
	std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	std::vector<std::string> objectNames;

	for (int i = 0; i < meshesToDraw.size(); i++)
	{
		objectNames.push_back(meshesToDraw[i]->friendlyName);
	}

	return objectNames;
}

const char** cGameManager::GetNameOfAllObjects()
{
	//g_currentScene->SortMeshesByName();
	std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	unsigned int size = 0;

	for (unsigned int i = 0; i < meshesToDraw.size(); i++)
	{
		size += (unsigned int)meshesToDraw[i]->friendlyName.size();
	}

	const char** items = new const char* [size];
	for (unsigned int i = 0; i < meshesToDraw.size(); ++i)
	{
		items[i] = meshesToDraw[i]->friendlyName.c_str();
	}

	return items;
}

const char** cGameManager::GetNameOfAllCameras()
{
	unsigned int size = 0;

	std::string camera1Type = g_currentScene->GetCameras()[0]->GetCameraType();
	std::string camera2Type = g_currentScene->GetCameras()[1]->GetCameraType();

	for (unsigned int i = 0; i < g_currentScene->GetCameras().size(); i++)
	{
		if (g_currentScene->GetCameras()[i]->GetCameraType() != "flyCamera")
		{
			size += (unsigned int)g_currentScene->GetCameras()[i]->GetCameraType().size();
		}
	}

	const char** items = new const char* [size];

	int index = 0;
	// Copy each string from the vector to the array
	for (unsigned int i = 0; i < g_currentScene->GetCameras().size(); ++i)
	{
		std::string cameraType = g_currentScene->GetCameras()[i]->GetCameraType();
		if (cameraType != "flyCamera")
		{
			std::string cameraFriendlyName = g_currentScene->GetCameras()[i]->GetFriendlyName();
			items[index] = _strdup(cameraFriendlyName.c_str());
			++index;
		}
	}

	return items;
}

unsigned int cGameManager::GetNumberOfObjects()
{
	unsigned int numberOfMeshes = (unsigned int)g_currentScene->GetAllMeshes().size();
	return numberOfMeshes;
}

unsigned int cGameManager::GetNumberOfCameras()
{
	return (unsigned int)(g_currentScene->GetCameras().size() - 1);
}

sPhysicsProperties* cGameManager::FindPhysicsByFriendlyName(std::string friendlyName)
{
	for (unsigned int index = 0; index != g_currentScene->m_scenePhysics->m_vec_pPhysicalProps.size(); index++)
	{
		if (g_currentScene->m_scenePhysics->m_vec_pPhysicalProps[index]->friendlyName == friendlyName)
		{
			return g_currentScene->m_scenePhysics->m_vec_pPhysicalProps[index];
		}
	}
	return nullptr;
}

std::vector<sPhysicsProperties*> cGameManager::FindAllPhysicsByTag(std::string objectType)
{
	std::vector<sPhysicsProperties*> allObjectOfType;

	for (int i = 0; i < g_currentScene->m_scenePhysics->m_vec_pPhysicalProps.size(); i++)
	{
		if (g_currentScene->m_scenePhysics->m_vec_pPhysicalProps[i]->objectType == objectType)
		{
			allObjectOfType.push_back(g_currentScene->m_scenePhysics->m_vec_pPhysicalProps[i]);
		}
	}

	return allObjectOfType;
}

bool cGameManager::DeleteGameObjectByFriendlyName(std::string friendlyName)
{
	std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	for (int i = 0; i < meshesToDraw.size(); i++)
	{
		if (meshesToDraw[i]->friendlyName == friendlyName)
		{
			meshesToDraw.erase(meshesToDraw.begin() + i);
			return true;
		}
	}

	return false;
}

bool cGameManager::DeleteGameObjectById(unsigned int id)
{


	std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	for (int i = 0; i < meshesToDraw.size(); i++)
	{
		if (meshesToDraw[i]->GetUniqueID() == id)
		{
			meshesToDraw.erase(meshesToDraw.begin() + i);
			return true;
		}
	}

	return false;
}

bool cGameManager::DeleteMeshByFriendlyName(std::string friendlyName)
{
	//std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

	//for (int i = 0; i < meshesToDraw.size(); i++)
	//{
	//	if (meshesToDraw[i]->friendlyName == friendlyName)
	//	{
	//		gRemoveMeshByFriendlyName

	//		meshesToDraw.erase(meshesToDraw.begin() + i);
	//		return true;
	//	}
	//}

	return false;
}

glm::vec4 cGameManager::NormalizeRGBA(glm::vec4 RGBA)
{
	return (glm::vec4(RGBA.r / 255.0f, RGBA.g / 255.0f, RGBA.b / 255.0f, RGBA.a / 255.0f));
}

//bool CompareMeshesByFriendlyName(const cMesh* mesh1, const cMesh* mesh2) 
//{
//	return mesh1->friendlyName < mesh2->friendlyName;
//}

//void cGameManager::SortMeshesByName()
//{
//	std::sort(g_vec_pMeshesToDraw.begin(), g_vec_pMeshesToDraw.end(), CompareMeshesByFriendlyName);
//}
