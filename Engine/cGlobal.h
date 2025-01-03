#pragma once

#include "cMesh.h"
#include "cDebugRender.h"
#include "cCamera.h"
#include "cMediaPlayer.h"
#include "cFlyCamera.h"
#include "cGameManager.h"

extern cDebugRenderer* g_pDebugRenderer;
extern cMesh* g_objectSelected;
extern iCamera* g_camera;
extern cMediaPlayer* g_mediaPlayer;
extern bool bUseFlyCamera;
extern cFlyCamera* g_pFlyCamera;

extern bool g_drawDebugLightSpheres;
extern bool g_drawColliders;

void g_DrawDebugSphere(glm::vec3 position, float scale, glm::vec4 colourRGBA);

