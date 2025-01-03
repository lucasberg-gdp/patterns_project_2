#pragma once
#include "cJsonHandler.h"
#include "iCamera.h"
#include "cGameObject.h"

class cCameraManager
{
public:
	static void LoadCameras(std::vector<sCameraInfo>& cameraInfo, std::vector< cGameObject* >& g_gameObjects);

	static iCamera* CreateCamera(sCameraInfo);
};

