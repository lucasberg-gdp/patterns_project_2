#include "cCameraManager.h"
#include "cFlyCamera.h"
#include "cCamera.h"
#include "cGameManager.h"
#include "cPhysics.h"
#include "cScene.h"

extern cFlyCamera* g_pFlyCamera;
extern cMesh* g_objectSelected;

extern cScene* g_currentScene;
extern iCamera* g_camera;
extern cPhysics* g_pPhysics;

void cCameraManager::LoadCameras(
    std::vector<sCameraInfo>& cameraInfo,
    std::vector< cGameObject* >& g_gameObjects)
{
    std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();

    for (unsigned int i = 0; i < cameraInfo.size(); i++)
    {
        g_currentScene->AddCamera(cCameraManager::CreateCamera(cameraInfo[i]));
    }

    std::vector<iCamera*> sceneCameras = g_currentScene->GetCameras();

    ::g_pFlyCamera = new cFlyCamera(cameraInfo[0]);
    sceneCameras[0] = ::g_pFlyCamera;
    g_objectSelected = meshesToDraw[0];

    g_camera = g_pFlyCamera;

    for (unsigned int i = 1; i < cameraInfo.size(); i++)
    {
        sceneCameras[i] = new cCamera(cameraInfo[i]);
        sceneCameras[i]->SetFriendlyName("Camera " + std::to_string(i));
    }
}

iCamera* cCameraManager::CreateCamera(sCameraInfo cameraInfo)
{
    if (cameraInfo.CameraType == "flyCamera")
    {
        cFlyCamera* flyCamera = new cFlyCamera(cameraInfo);
        g_currentScene->m_scenePhysics->AddShape(flyCamera->flyCameraPhysics);
        return flyCamera;
    }

    if (cameraInfo.CameraType == "camera")
    {
        return new cCamera(cameraInfo);
    }

    return nullptr;
}
