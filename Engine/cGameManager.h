#pragma once

#include <glm/vec3.hpp>
#include <string>

#include "cMesh.h"
#include "sPhysicsProperties.h"
#include "cGameObject.h"
#include <map>

class cGameManager
{
private:
    //float m_gameMinimumX;
    //float m_gameMaximumX;
    //float m_gameMinimumY;
    //float m_gameMaximumY;

    static cGameManager* gmInstance;
    //glm::vec3 centerOfLevel;

    cGameManager()
    {

    }

public:
    cGameManager(const cGameManager& obj) = delete;

    static cGameManager* GetGMInstance()
    {
        if (gmInstance == nullptr)
        {
            gmInstance = new cGameManager();
            return gmInstance;
        }
        else
        {
            return gmInstance;
        }
    }

    bool showUi = true;

    float GetMinX();
    float GetMaxX();

    float GetMinY();
    float GetMaxY();

    //void SetBorders(float minX, float maxX, float minY, float maxY);

    glm::vec3 GetCenterOfLevel();

    cMesh* FindMeshById(unsigned id);
    cMesh* FindMeshByFriendlyName(std::string friendlyNameToFind);
    std::vector<cMesh*> FindAllMeshesByObjectType(std::string objectType);

    cGameObject* FindGameObjectByFriendlyTag(std::string objectTag, std::vector<cGameObject*>& gameObjects);


    std::vector<std::string> GetNameOfAllObjectsInVector();

    const char** GetNameOfAllObjects();
    const char** GetNameOfAllCameras();

    unsigned int GetNumberOfObjects();
    unsigned int GetNumberOfCameras();

    sPhysicsProperties* FindPhysicsByFriendlyName(std::string friendlyName);
    std::vector<sPhysicsProperties*> FindAllPhysicsByTag(std::string objectType);

    //void SetCanInteractWithCow(bool canInteract);
    //void SetCanInteractWithLion(bool canInteract);

    //void SetNotInteracting();
    static bool DeleteGameObjectByFriendlyName(std::string friendlyName);
    static bool DeleteGameObjectById(unsigned int id);

    static bool DeleteMeshByFriendlyName(std::string friendlyName);

    static glm::vec4 NormalizeRGBA(glm::vec4 RGBA);

    //void SortMeshesByName();
};