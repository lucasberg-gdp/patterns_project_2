#include <vector>
#include <glm/gtx/quaternion.hpp>

#include "cMesh.h"
//#include "sPhysicsProperties.h"
#include "cVAOManager/cVAOManager.h"
#include "cPhysics.h"
#include "MathUtils.h"
#include "cGameManager.h"
#include "cSphere.h"
#include "cGameObject.h"
#include "cColor.h"
#include "cJsonHandler.h"
#include "cLionPlayer.h"
#include "cEnemy.h"
#include "cScene.h"

//extern std::vector< cMesh* > g_vec_pMeshesToDraw;
extern std::vector< sPhysicsProperties* > g_vec_pPhysicalProps;

extern cScene* g_currentScene;

extern cMesh* g_pDebugSphereMesh;
extern cVAOManager* g_pMeshManager;

extern cLionPlayer* lionPlayer;

extern std::vector< cGameObject* > g_gameObjects;

bool LoadModels(std::vector<sModel> sceneModelsInfo)
{
    //for (unsigned int i = 0; i < sceneModelsInfo.size(); i++)
    //{
    //    cMesh* mesh = new cMesh();
    //    mesh->drawPosition = sceneModelsInfo[i].Position;
    //    mesh->drawScale = sceneModelsInfo[i].DrawScale;
    //    mesh->setDrawOrientation(sceneModelsInfo[i].Orientation);
    //    mesh->bUseDebugColours = sceneModelsInfo[i].IsUsingDebugColors;
    //    mesh->isUsingVertexColors = sceneModelsInfo[i].IsUsingVertexColors;
    //    mesh->friendlyName = sceneModelsInfo[i].FriendlyName;
    //    mesh->objectType = sceneModelsInfo[i].ObjectType;
    //    mesh->transparencyAlpha = sceneModelsInfo[i].Alpha;
    //    mesh->wholeObjectDebugColourRGBA = sceneModelsInfo[i].DebugColourRGBA;

    //    mesh->meshName = sceneModelsInfo[i].MeshName;
    //    
    //    for (unsigned int j = 0; j < sceneModelsInfo[i].ObjectTextures.size(); j++)
    //    {
    //        mesh->textureName[j] = sceneModelsInfo[i].ObjectTextures[j].Name;
    //        mesh->textureRatios[j] = sceneModelsInfo[i].ObjectTextures[j].Ratio;
    //    }

    //    //g_vec_pMeshesToDraw.push_back(mesh);
    //    g_currentScene->AddMesh(*mesh);
    //}

    //::g_pDebugSphereMesh = new cMesh();
    //::g_pDebugSphereMesh->meshName = "shapes/Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";
    //::g_pDebugSphereMesh->bIsWireframe = true;
    //::g_pDebugSphereMesh->bDoNotLight = true;
    //::g_pDebugSphereMesh->setUniformDrawScale(1.0f);
    //::g_pDebugSphereMesh->bIsVisible = false;
    //::g_pDebugSphereMesh->friendlyName = "DEBUG_SPHERE";

    // testing mathutils
    //unsigned int numOfSpheres = 1000;
    //for (unsigned int i = 0; i < numOfSpheres; i++)
    //{
    //    cSphere* sphere = new cSphere();

    //    glm::vec3 randomPositionOnSurface = MathUtils::GetRandomOnSurfaceOfSphere(glm::vec3(0.0f, 10.0f,0.0f), 10.0f);
    //    sphere->SetScale(0.2f);
    //    sphere->SetPosition(randomPositionOnSurface);

    //    g_vec_pMeshesToDraw.push_back(sphere->GetObjectMesh());
    //}

    //cSphere* sphere1 = new cSphere();

    //// lion object
    cMesh* lionMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName("lion");
    lionPlayer = new cLionPlayer(lionMesh);

    sPhysicsProperties* lionPhysics = new sPhysicsProperties(sPhysicsProperties::SPHERE);
    lionPhysics->pTheAssociatedMesh = lionMesh;
    lionPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
    lionPhysics->setRotationFromEuler(lionMesh->drawOrientation);
    lionPhysics->position = lionMesh->drawPosition;
    lionPhysics->objectType = lionMesh->objectType;
    lionPhysics->friendlyName = lionMesh->friendlyName;
    lionPhysics->objectType = lionMesh->objectType;
    lionPlayer->m_objectPhysics = lionPhysics;
    g_currentScene->m_scenePhysics->AddShape(lionPhysics);
    g_currentScene->AddGameObject(lionPlayer);

    //// deer
    //std::vector<cMesh*> allDeer = cGameManager::getGmInstance()->FindAllMeshesByObjectType("deer");
    //for (unsigned int i = 0; i < allDeer.size(); i++)
    //{
    //    cMesh* deerMesh = allDeer[i];
    //    cEnemy* deerEnemy = new cEnemy(deerMesh);
    //    deerEnemy->tag = "deer";
    //    g_gameObjects.push_back(deerEnemy);
    //}

    //// bison
    //std::vector<cMesh*> allBisons = cGameManager::getGmInstance()->FindAllMeshesByObjectType("bison");
    //for (unsigned int i = 0; i < allBisons.size(); i++)
    //{
    //    cMesh* bisonMesh = allBisons[i];
    //    cEnemy* bisonEnemy = new cEnemy(bisonMesh);
    //    bisonEnemy->tag = "bison";
    //    g_gameObjects.push_back(bisonEnemy);
    //}

    //// unicorn
    //std::vector<cMesh*> allUnicorns = cGameManager::getGmInstance()->FindAllMeshesByObjectType("unicorn");
    //for (unsigned int i = 0; i < allUnicorns.size(); i++)
    //{
    //    cMesh* unicornMesh = allUnicorns[i];
    //    cEnemy* unicornEnemy = new cEnemy(unicornMesh);
    //    unicornEnemy->tag = "unicorn";
    //    g_gameObjects.push_back(unicornEnemy);
    //}

    //// mamooth
    //std::vector<cMesh*> allMamooths = cGameManager::getGmInstance()->FindAllMeshesByObjectType("mamooth");
    //for (unsigned int i = 0; i < allMamooths.size(); i++)
    //{
    //    cMesh* mamoothMesh = allMamooths[i];
    //    cEnemy* mamoothEnemy = new cEnemy(mamoothMesh);
    //    mamoothEnemy->tag = "mamooth";
    //    g_gameObjects.push_back(mamoothEnemy);
    //}

    //// scorpion
    //std::vector<cMesh*> allScorpions = cGameManager::getGmInstance()->FindAllMeshesByObjectType("scorpion");
    //for (unsigned int i = 0; i < allScorpions.size(); i++)
    //{
    //    cMesh* scorpionMesh = allScorpions[i];
    //    cEnemy* scorpionEnemy = new cEnemy(scorpionMesh);
    //    scorpionEnemy->tag = "scorpion";
    //    g_gameObjects.push_back(scorpionEnemy);
    //}


    //cAnimation* animation = new cAnimation();
    //animation->m_time = 4.0;
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f,10.0f,0.0f), 0.0f));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(10.0f, 10.0f, 0.0f), 1.0f, cAnimation::EasingType::EaseIn));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(10.0f, 20.0f, 0.0f), 2.0f, cAnimation::EasingType::NoEasing));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 20.0f, 0.0f), 3.0f, cAnimation::EasingType::EaseInOut));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 10.0f, 0.0f), 4.0f, cAnimation::EasingType::EaseOut));

    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 0.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.0f, 1.0f, 0.0f, 0.0f), 1.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.0f, 0.0f, 1.0f, 0.0f), 2.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), 3.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 4.0));

    //cAnimation* animation = new cAnimation();
    //animation->m_time = 4.0;
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 10.0f, 0.0f), 0.0f));
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(10.0f, 10.0f, 0.0f), 1.0f, cAnimation::EasingType::EaseIn));
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(10.0f, 20.0f, 0.0f), 2.0f, cAnimation::EasingType::NoEasing));
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 20.0f, 0.0f), 3.0f, cAnimation::EasingType::EaseInOut));
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 10.0f, 0.0f), 4.0f, cAnimation::EasingType::EaseOut));
    // 
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 10.0f, 0.0f), 0.0f));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 0.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.0f, 1.0f, 0.0f, 0.0f), 1.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.0f, 0.0f, 1.0f, 0.0f), 2.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), 3.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 4.0));

    //Xwing animation
    //cAnimation* animation = new cAnimation();
    //animation->m_time = 5.0;
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 10.0f, 0.0f), 0.0f));
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(10.0f, 10.0f, 0.0f), 1.0f, cAnimation::EasingType::EaseIn));
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(10.0f, 20.0f, 0.0f), 2.0f, cAnimation::EasingType::NoEasing));
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 20.0f, 0.0f), 3.0f, cAnimation::EasingType::EaseInOut));
    ////animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(0.0f, 10.0f, 0.0f), 4.0f, cAnimation::EasingType::EaseOut));

    //cMesh* xWingMesh = cGameManager::getGmInstance()->FindMeshByFriendlyName("xwing1");
    //xWingMesh->bUseDebugColours = true;
    //xWingMesh->wholeObjectDebugColourRGBA = cColor::COLOR_WHITE;
    //sPhysicsProperties* xWingPhysics = new sPhysicsProperties(sPhysicsProperties::SPHERE);
    //xWingPhysics->pTheAssociatedMesh = xWingMesh;
    //xWingPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
    //xWingPhysics->setRotationFromEuler(xWingMesh->drawOrientation);
    //xWingPhysics->position = xWingMesh->drawPosition;
    //xWingPhysics->objectType = xWingMesh->objectType;
    //xWingPhysics->friendlyName = xWingMesh->friendlyName;
    //xWingPhysics->objectType = xWingMesh->objectType;

    //::g_pPhysics->AddShape(xWingPhysics);

    //cGameObject* xWingGameObject = new cGameObject(xWingMesh);
    //xWingGameObject->tag = "xwing";
    //xWingGameObject->LinkPhysics(xWingPhysics);
    //glm::vec3 xWingInitialPosition = xWingMesh->drawPosition;
    //glm::quat initialRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(xWingInitialPosition, 0.0f));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(xWingInitialPosition.x, xWingInitialPosition.y, xWingInitialPosition.z + 80.0f), 1.0f));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(xWingInitialPosition.x, xWingInitialPosition.y, xWingInitialPosition.z + 160.0f), 2.0f, cAnimation::EasingType::EaseIn));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(xWingInitialPosition.x, xWingInitialPosition.y, xWingInitialPosition.z + 240.0f), 3.0f, cAnimation::EasingType::NoEasing));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(xWingInitialPosition.x, xWingInitialPosition.y, xWingInitialPosition.z + 320.0f), 4.0f, cAnimation::EasingType::EaseInOut));
    //animation->m_positionKeyFrames.push_back(cAnimation::PositionKeyFrame(glm::vec3(xWingInitialPosition.x, xWingInitialPosition.y, xWingInitialPosition.z + 400.0f), 5.0f, cAnimation::EasingType::EaseOut));


    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 0.0));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.9f, 0.0f, 0.0f, 0.1f), 1.0, cAnimation::EasingType::EaseInOut));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.8f, 0.0f, 0.0f, 0.2f), 2.0, cAnimation::EasingType::EaseOut));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.7f, 0.0f, 0.0f, 1.3f), 3.0, cAnimation::EasingType::NoEasing));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.6f, 0.0f, 0.0f, 0.4f), 4.0, cAnimation::EasingType::EaseIn));
    //animation->m_rotationKeyFrames.push_back(cAnimation::RotationKeyFrame(glm::quat(0.5f, 0.0f, 0.0f, 0.5f), 4.0, cAnimation::EasingType::EaseOut));

    //cMesh* xWingCameraMesh = cGameManager::getGmInstance()->FindMeshByFriendlyName("xwingcamera");
    //cGameObject* wWingCameraObject = new cGameObject(xWingCameraMesh);
    //
    ////wWingCameraObject->SetParent(xWingGameObject);

    //xWingGameObject->animation = animation;
    //xWingGameObject->animation->m_isAnimRunning = true;
    //g_gameObjects.push_back(xWingGameObject);


    //cAnimation* explosionAnimation = new cAnimation();
    //explosionAnimation->m_time = 1.0;

    //explosionAnimation->m_scaleKeyFrames.push_back(cAnimation::ScaleKeyFrame(glm::vec3(1.0f,1.0f,1.0f), 0.25));
    //explosionAnimation->m_scaleKeyFrames.push_back(cAnimation::ScaleKeyFrame(glm::vec3(5.0f, 5.0f, 5.0f), 0.5, cAnimation::EasingType::EaseIn));
    //explosionAnimation->m_scaleKeyFrames.push_back(cAnimation::ScaleKeyFrame(glm::vec3(9.0f, 9.0f, 9.0f), 0.75, cAnimation::EasingType::NoEasing));
    //explosionAnimation->m_scaleKeyFrames.push_back(cAnimation::ScaleKeyFrame(glm::vec3(30.0f, 30.0f, 30.0f), 1.0, cAnimation::EasingType::EaseOut));

    //cSphere* explosionSphere = new cSphere();
    //explosionSphere->GetObjectMesh()->wholeObjectDebugColourRGBA = cColor::COLOR_BLUE;
    //explosionSphere->animation = explosionAnimation;
    //::g_vec_pMeshesToDraw.push_back(explosionSphere->GetObjectMesh());
    //g_gameObjects.push_back(explosionSphere);



    // Example 2

    //const float coneHeight = 2.0f;

    //cMesh* cone1 = new cMesh();
    //cone1->drawPosition = glm::vec3(0.0f, 10.0f, 0.0f);
    //cone1->bUseDebugColours = true;
    //cone1->wholeObjectDebugColourRGBA = cColor::COLOR_RED;
    //cone1->friendlyName = "cone";
    //cone1->meshName = "Cone_xyz_n_rgba_uv.ply";

    //sPhysicsProperties* conePhysics1 = new sPhysicsProperties(sPhysicsProperties::SPHERE);
    //conePhysics1->pTheAssociatedMesh = cone1;
    //conePhysics1->setShape(new sPhysicsProperties::sSphere(2.0f));
    //conePhysics1->setRotationFromEuler(cone1->drawOrientation);
    //conePhysics1->position = cone1->drawPosition;
    //conePhysics1->friendlyName = cone1->friendlyName;
    //conePhysics1->objectType = cone1->objectType;

    //cGameObject* coneObject1 = new cGameObject(cone1);
    //coneObject1->LinkPhysics(conePhysics1);
    //coneObject1->animation = animation;

    //::g_pPhysics->AddShape(conePhysics1);

    //cMesh* cone2 = new cMesh();
    //cone2->drawPosition = glm::vec3(0.0f, coneHeight, 0.0f);
    //cone2->bUseDebugColours = true;
    //cone2->wholeObjectDebugColourRGBA = cColor::COLOR_RED;
    //cone2->friendlyName = "cone";
    //cone2->meshName = "Cone_xyz_n_rgba_uv.ply";
    //
    //sPhysicsProperties* conePhysics2 = new sPhysicsProperties(sPhysicsProperties::SPHERE);
    //conePhysics2->pTheAssociatedMesh = cone2;
    //conePhysics2->setShape(new sPhysicsProperties::sSphere(2.0f));
    //conePhysics2->setRotationFromEuler(cone2->drawOrientation);
    //conePhysics2->position = cone2->drawPosition;
    //conePhysics2->friendlyName = cone2->friendlyName;
    //conePhysics2->objectType = cone2->objectType;

    //cGameObject* coneObject2 = new cGameObject(cone2);
    //coneObject2->LinkPhysics(conePhysics2);
    //coneObject2->animation = animation;

    //::g_pPhysics->AddShape(conePhysics2);
    ////g_gameObjects.push_back(coneObject2);

    //cMesh* cone3 = new cMesh();
    //cone3->drawPosition = glm::vec3(0.0f, coneHeight, 0.0f);
    //cone3->bUseDebugColours = true;
    //cone3->wholeObjectDebugColourRGBA = cColor::COLOR_RED;
    //cone3->friendlyName = "cone";
    //cone3->meshName = "Cone_xyz_n_rgba_uv.ply";
    //
    //sPhysicsProperties* conePhysics3 = new sPhysicsProperties(sPhysicsProperties::SPHERE);
    //conePhysics3->pTheAssociatedMesh = cone3;
    //conePhysics3->setShape(new sPhysicsProperties::sSphere(2.0f));
    //conePhysics3->setRotationFromEuler(cone3->drawOrientation);
    //conePhysics3->position = cone3->drawPosition;
    //conePhysics3->friendlyName = cone3->friendlyName;
    //conePhysics3->objectType = cone3->objectType;

    //cGameObject* coneObject3 = new cGameObject(cone3);
    //coneObject3->LinkPhysics(conePhysics3);
    //coneObject3->animation = animation;

    //::g_pPhysics->AddShape(conePhysics3);
    ////g_gameObjects.push_back(coneObject3);

    //cMesh* cone4 = new cMesh();
    //cone4->drawPosition = glm::vec3(0.0f, coneHeight, 0.0f);
    //cone4->bUseDebugColours = true;
    //cone4->wholeObjectDebugColourRGBA = cColor::COLOR_RED;
    //cone4->friendlyName = "cone";
    //cone4->meshName = "Cone_xyz_n_rgba_uv.ply";

    //sPhysicsProperties* conePhysics4 = new sPhysicsProperties(sPhysicsProperties::SPHERE);
    //conePhysics4->pTheAssociatedMesh = cone4;
    //conePhysics4->setShape(new sPhysicsProperties::sSphere(2.0f));
    //conePhysics4->setRotationFromEuler(cone4->drawOrientation);
    //conePhysics4->position = cone4->drawPosition;
    //conePhysics4->friendlyName = cone4->friendlyName;
    //conePhysics4->objectType = cone4->objectType;

    //cGameObject* coneObject4 = new cGameObject(cone4);
    //coneObject4->LinkPhysics(conePhysics4);
    //coneObject4->animation = animation;

    //::g_pPhysics->AddShape(conePhysics4);

    //coneObject4->SetParent(coneObject3);
    //coneObject3->SetParent(coneObject2);
    //coneObject2->SetParent(coneObject1);
    //g_gameObjects.push_back(coneObject1);
    //g_gameObjects.push_back(coneObject2);
    //g_gameObjects.push_back(coneObject3);
    //g_gameObjects.push_back(coneObject4);

    //g_vec_pMeshesToDraw.push_back(coneObject1->GetObjectMesh());
    // Example 1
    //cMesh* bathTub = new cMesh();
    //bathTub->drawPosition = glm::vec3(0.0f, 10.0f, 0.0f);
    //bathTub->drawScale = glm::vec3(0.4f);
    //bathTub->setRotationFromEuler(glm::vec3(0.0f));
    //bathTub->bUseDebugColours = true;
    //bathTub->friendlyName = "bathtub";
    //bathTub->meshName = "bathtub_xyz_n_rgba_uv.ply";
    //cGameObject* bathTubObject = new cGameObject(bathTub);
    //sPhysicsProperties* bathTubPhysics = new sPhysicsProperties();
//bathTubPhysics->shapeType = sPhysicsProperties::SPHERE;
//bathTubPhysics->pTheAssociatedMesh = bathTub;
//bathTubPhysics->shapeType = sPhysicsProperties::SPHERE;
//bathTubPhysics->setShape(new sPhysicsProperties::sSphere(2.0f));
//bathTubPhysics->setRotationFromEuler(bathTub->drawOrientation);
//bathTubPhysics->position = bathTub->drawPosition;
//bathTubPhysics->friendlyName = bathTub->friendlyName;
//bathTubPhysics->objectType = bathTub->objectType;
        //bathTubObject->LinkPhysics(bathTubPhysics);
    //::g_pPhysics->AddShape(bathTubPhysics);
        //bathTubObject->animation = animation;
    //g_vec_pMeshesToDraw.push_back(bathTubObject->GetObjectMesh());
    //g_gameObjects.push_back(bathTubObject);





    //cSphere* sphereDebug = new cSphere("spheretest1");
    //sphereDebug->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    //sphereDebug->SetScale(1.0f);
    //sphereDebug->tag = "spheretest";
    //sphereDebug->animation = animation;
    //g_vec_pMeshesToDraw.push_back(sphereDebug->GetObjectMesh());

    //cSphere* sphereDebug1 = new cSphere("spheretest2");
    //sphereDebug1->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
    //sphereDebug1->SetScale(1.0f);
    //sphereDebug1->tag = "spheretest";
    //sphereDebug1->animation = new cAnimation(
    //    sphereDebug1->GetPosition(),
    //    glm::vec3(10.0f, 15.0f, 0.0f),
    //    cAnimation::EasingType::EaseIn);
    //g_vec_pMeshesToDraw.push_back(sphereDebug1->GetObjectMesh());

    //cSphere* sphereDebug2 = new cSphere("spheretest3");
    //sphereDebug2->SetPosition(glm::vec3(0.0f, 20.0f, 0.0f));
    //sphereDebug2->SetScale(1.0f);
    //sphereDebug2->tag = "spheretest";
    //sphereDebug2->animation = new cAnimation(
    //    sphereDebug2->GetPosition(),
    //    glm::vec3(10.0f, 20.0f, 0.0f),
    //    cAnimation::EasingType::EaseOut);
    ////g_vec_pMeshesToDraw.push_back(sphereDebug2->GetObjectMesh());

    //cSphere* sphereDebug3 = new cSphere("spheretest4");
    //sphereDebug3->SetPosition(glm::vec3(0.0f, 25.0f, 0.0f));
    //sphereDebug3->SetScale(1.0f);
    //sphereDebug3->tag = "spheretest";
    //sphereDebug3->animation = new cAnimation(
    //    sphereDebug3->GetPosition(),
    //    glm::vec3(10.0f, 25.0f, 0.0f),
    //    cAnimation::EasingType::EaseInOut);
    ////g_vec_pMeshesToDraw.push_back(sphereDebug3->GetObjectMesh());

    //cSphere* sphereDebug4 = new cSphere("spheretest4");
    //sphereDebug4->SetPosition(glm::vec3(10.0f, 15.0f, 0.0f));
    //sphereDebug4->SetScale(glm::vec3(0.2f, 15.0f,1.0f));
    //sphereDebug4->tag = "spheretest4";
    //sphereDebug4->GetObjectMesh()->friendlyName = "spheretest4";
    //g_vec_pMeshesToDraw.push_back(sphereDebug4->GetObjectMesh());

    //cSphere* sphereDebug5 = new cSphere("spheretest5");
    //sphereDebug5->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
    //sphereDebug5->SetScale(glm::vec3(0.2f, 15.0f, 1.0f));
    //sphereDebug5->tag = "spheretest5";
    //sphereDebug5->GetObjectMesh()->friendlyName = "spheretest5";
    //g_vec_pMeshesToDraw.push_back(sphereDebug5->GetObjectMesh());

    //cSphere* sphereDebug2 = new cSphere();
    //sphereDebug2->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
    //sphereDebug2->SetScale(1.5f);
    //sphereDebug2->SetParent(sphereDebug);
    //g_vec_pMeshesToDraw.push_back(sphereDebug2->GetObjectMesh());
    //sphereDebug->GetObjectMesh()->vec_pChildMeshes.push_back(sphereDebug2->GetObjectMesh());

    //g_gameObjects.push_back(sphereDebug);
    //g_gameObjects.push_back(sphereDebug1);
    //g_gameObjects.push_back(sphereDebug2);
    //g_gameObjects.push_back(sphereDebug3);



    //sphereDebug->SetScale(3.0f);

    //cSphere* sphereDebug3 = new cSphere();
    //sphereDebug3->SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));
    //sphereDebug3->SetScale(10.0f);
    //sphereDebug2->GetObjectMesh()->vec_pChildMeshes.push_back(sphereDebug3->GetObjectMesh());

    //g_vec_pMeshesToDraw.push_back(sphereDebug2->GetMesh());

    //cMesh* snowMan3 = new cMesh();
    //snowMan3->friendlyName = "generictree";
    //snowMan3->objectType = "prop";
    //snowMan3->setUniformDrawScale(0.2f);
    //snowMan3->drawPosition = glm::vec3(10.0f, 0.0f, 0.0f);
    //snowMan3->textureName[0] = "SM_Prop_Snowman_xyz_n_rgba_uv.bmp";
    //snowMan3->textureRatios[0] = 1.0f;
    //snowMan3->meshName = "SM_Generic_Tree_01.ply";
    //cGameObject* snowMan = new cGameObject(snowMan3);

    //::g_vec_pMeshesToDraw.push_back(sphereDebug->GetMesh());

    //cMesh* camera = new cMesh();
    //camera->meshName = "Sphere_1_unit_Radius_xyz_n_rgba_uv.ply.ply";
    //camera->friendlyName = "camera";
    //camera->objectType = "camera";
    //camera->setUniformDrawScale(1.0f);
    //camera->drawPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    //camera->textureName[0] = "Water_Texture_01.bmp";
    //camera->setDrawOrientation(glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
    //camera->textureRatios[0] = 1.0f;
    //::g_vec_pMeshesToDraw.push_back(camera);

    ////SM_Prop_Snowman_xyz_n_rgba_uv
    //cMesh* snowMan = new cMesh();
    //snowMan->meshName = "SM_Prop_Snowman_xyz_n_rgba_uv.ply";
    //snowMan->friendlyName = "snowman2";
    //snowMan->objectType = "prop";
    //snowMan->setUniformDrawScale(0.1f);
    //snowMan->drawPosition = glm::vec3(15.0f, 0.0f, 0.0f);
    //snowMan->textureName[0] = "PolygonSnow_Texture_01_A.bmp";
    ////snowMan->setDrawOrientation(glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
    //snowMan->textureRatios[0] = 1.0f;
    //snowMan->setRotationFromEuler(glm::vec3(0.0f));

    //snowMan->setRotationFromEuler(glm::vec3(0.0f, -1.0f, 0.0f));
    //snowMan->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    //::g_vec_pMeshesToDraw.push_back(snowMan);

    ////SM_Prop_Snowman_xyz_n_rgba_uv
    //cMesh* genericTree = new cMesh();
    //genericTree->meshName = "SM_Generic_Tree_01.ply";
    //genericTree->friendlyName = "generictree";
    //genericTree->objectType = "prop";
    //genericTree->setUniformDrawScale(0.2f);
    //genericTree->drawPosition = glm::vec3(10.0f, 0.0f, 0.0f);
    //genericTree->textureName[0] = "PolygonSnow_Texture_01_A.bmp";
    //genericTree->textureRatios[0] = 1.0f;
    //::g_vec_pMeshesToDraw.push_back(genericTree);

    ////SM_Generic_Tree_02.ply
    //cMesh* genericTree = new cMesh();
    //genericTree->meshName = "SM_Generic_Tree_02.ply";
    //genericTree->friendlyName = "generictree2";
    //genericTree->objectType = "prop";
    //genericTree->setUniformDrawScale(0.2f);
    //genericTree->drawPosition = glm::vec3(10.0f, 0.0f, 0.0f);
    //genericTree->textureName[0] = "PolygonSnow_Texture_01_A.bmp";
    //genericTree->textureRatios[0] = 1.0f;
    //::g_vec_pMeshesToDraw.push_back(genericTree);

    ////Lantern.ply
    //cMesh* lantern = new cMesh();
    //lantern->meshName = "Lantern_xyz_n_rgba_uv.ply";
    //lantern->friendlyName = "lantern";
    //lantern->objectType = "prop";
    //lantern->setUniformDrawScale(1.6f);
    //lantern->setRotationFromEuler(glm::vec3(0.0f, 0.0f, 0.0f));
    //lantern->drawPosition = glm::vec3(10.0f, 30.0f, 0.0f);
    //lantern->textureName[0] = "T_LanternPole_mr.bmp";
    //lantern->textureRatios[0] = 0.2f;
    //lantern->textureName[1] = "T_LanternPole_a.bmp";
    //lantern->textureRatios[1] = 0.2f;
    //lantern->textureName[2] = "T_LanternPole_e.bmp";
    //lantern->textureRatios[2] = 0.2f;

    ////lantern->RotateAroundX(-90.0f);
    //::g_vec_pMeshesToDraw.push_back(lantern);

    //cMesh* snowGround = new cMesh();
    //snowGround->meshName = "plain_xyz_n_rgba_uv.ply";
    //snowGround->friendlyName = "snowground";
    //snowGround->objectType = "ground";
    //snowGround->setUniformDrawScale(5.0f);
    //snowGround->drawPosition = glm::vec3(0.0f, -25.0f, 0.0f);
    //snowGround->textureName[0] = "snow_texture_1.bmp";
    //snowGround->setDrawOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
    //snowGround->textureRatios[0] = 1.0f;
    //::g_vec_pMeshesToDraw.push_back(snowGround);

    //cMesh* pBathtub = new cMesh();
    //pBathtub->meshName = "bathtub_xyz_n_rgba_uv.ply";
    //pBathtub->friendlyName = "bathtub";
    //pBathtub->setUniformDrawScale(3.0f);
    //pBathtub->drawPosition = glm::vec3(0.0f, -30.0f, 0.0f);
    //pBathtub->textureName[0] = "Water_Texture_01.bmp";
    //pBathtub->setDrawOrientation(glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
    //pBathtub->textureRatios[0] = 1.0f;
    //::g_vec_pMeshesToDraw.push_back(pBathtub);

//    cMesh* pGroundMesh = new cMesh();
//    //    pGroundMesh->meshName = "Terrain_xyz_n_rgba_uv.ply";
//    //    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_00_132K_xyz_n_rgba_uv.ply";    
//    //    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_07_1K_xyz_n_rgba_uv.ply";    
//    pGroundMesh->meshName = "Big_Flat_Mesh_256x256_12_5_xyz_n_rgba_uv.ply";
//    pGroundMesh->drawPosition.y = -50.0f;
//    pGroundMesh->drawPosition.z = -50.0f;
//    pGroundMesh->friendlyName = "Ground";
//
//    //pGroundMesh->bIsWireframe = true;
//    //pGroundMesh->bDoNotLight = true;
//    //pGroundMesh->wholeObjectDebugColourRGBA = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
//    //pGroundMesh->bUseDebugColours = true;
//    //
////    pGroundMesh->textureName[0] = "NvF5e_height_map.bmp";
////    pGroundMesh->textureName[0] = "Blank_UV_Text_Texture.bmp";
//    pGroundMesh->textureName[0] = "TaylorSwift_Eras_Poster.bmp";
//    pGroundMesh->textureRatios[0] = 1.0f;

    //    pGroundMesh->textureName[1] = "taylor-swift-jimmy-fallon.bmp";
    //    pGroundMesh->textureRatios[1] = 0.0f;

    //::g_vec_pMeshesToDraw.push_back(pGroundMesh);


    //::g_pDebugSphereMesh = new cMesh();
    //::g_pDebugSphereMesh->meshName = "Sphere_1_unit_Radius.ply";
    //::g_pDebugSphereMesh->bIsWireframe = true;
    //::g_pDebugSphereMesh->bDoNotLight = true;
    //::g_pDebugSphereMesh->setUniformDrawScale(1.0f);
    //::g_pDebugSphereMesh->bIsVisible = false;
    //::g_pDebugSphereMesh->friendlyName = "DEBUG_SPHERE";
    // Note: we are NOT adding this to the vector of meshes


        // bun_zipper_res2_xyz_n_rgba.ply
    // bathtub_xyz_n_rgba.ply

//    // Add some models to the "scene"
    //cMesh* bunny1 = new cMesh();   
    //bunny1->meshName = "bathtub_xyz_n_rgba.ply";
    //bunny1->friendlyName = "bunny";
    //bunny1->drawOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
    //bunny1->drawPosition = glm::vec3(0.0f, 10.0f, 100.f);
    //bunny1->drawScale = glm::vec3(1.0f, 1.0f, 1.f);
    //bunny1->drawOrientation.y = glm::radians(0.0f);

    //bunny1->setRotationFromEuler(bunny1->drawOrientation);

    //cMesh* bunny2 = new cMesh();
    //bunny2->meshName = "bathtub_xyz_n_rgba.ply";
    //bunny2->friendlyName = "bunny";
    //bunny2->drawOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
    //bunny2->drawPosition = glm::vec3(0.0f, 10.0f, 150.f);
    //bunny2->drawScale = glm::vec3(1.0f, 1.0f, 1.f);
    //bunny2->drawOrientation.y = glm::radians(90.0f);

    //bunny1->setRotationFromEuler(bunny2->drawOrientation);

    //g_vec_pMeshesToDraw.push_back(bunny1);
    //g_vec_pMeshesToDraw.push_back(bunny2);

//
//    cMesh bunny2;
//    bunny2.meshName = "bun_zipper_res2_xyz_n_rgba.ply";
//    bunny2.position = glm::vec3(1.0f, 0.0f, 0.0f);
//    bunny2.scale = 7.5f;
//    bunny2.orientation.y = glm::radians(135.0f);
//
 //   cMesh* pBathtub = new cMesh();
 //   pBathtub->meshName = "bathtub_xyz_n_rgba.ply";
 //   pBathtub->friendlyName = "bathtub";
 ////   pBathtub->bDoNotLight = true;
 //   pBathtub->drawPosition = glm::vec3(0.0f, -30.0f, 0.0f);
 //   //bunny2.position = glm::vec3(1.0f, 0.0f, 0.0f);
 //   ::g_vec_pMeshesToDraw.push_back(pBathtub);

//
////    // Smart array of cMesh object
////    std::vector<cMesh> vecMeshesToDraw;
//    g_vecMeshesToDraw.push_back(bunny2);
//    g_vecMeshesToDraw.push_back(bathtub);
//    g_vecMeshesToDraw.push_back(terrain);

//    cMesh* pTerrain = new cMesh();
//    pTerrain->meshName = "Terrain_xyz_n_rgba.ply";
//    //bunny2.position = glm::vec3(1.0f, 0.0f, 0.0f);
//    pTerrain->scale = 1.0f;
//    pTerrain->drawPosition.y = -25.0f;
//    ::g_vec_pMeshesToDraw.push_back(pTerrain);

    //cMesh* pBathtub = new cMesh();
    //pBathtub->meshName = "bathtub_xyz_n_rgba_uv.ply";
    //pBathtub->friendlyName = "bathtub";
    //pBathtub->setUniformDrawScale(3.0f);
    //pBathtub->drawPosition = glm::vec3(0.0f, -30.0f, 0.0f);
    //pBathtub->textureName[0] = "TaylorSwift_Eras_Poster.bmp";
    //pBathtub->textureRatios[0] = 1.0f;
    //::g_vec_pMeshesToDraw.push_back(pBathtub);

    //cMesh* pBathtub = new cMesh();
    //pBathtub->meshName = "bathtub_xyz_n_rgba_uv.ply";
    //pBathtub->friendlyName = "bathtub";
    //pBathtub->setUniformDrawScale(3.0f);
    //pBathtub->drawPosition = glm::vec3(0.0f, -30.0f, 0.0f);
    //pBathtub->objectType = "bathtub";
    //pBathtub->textureName[0] = "Water_Texture_01.bmp";
    //pBathtub->textureRatios[0] = 1.0f;
    //::g_vec_pMeshesToDraw.push_back(pBathtub);


    //cMesh* pGridGroundMesh = new cMesh();
    //pGridGroundMesh->meshName = "Flat_Grid_100x100.ply";
    //pGridGroundMesh->bIsWireframe = true;
    //pGridGroundMesh->bDoNotLight = true;
    ////note this does NOT have a physProps, so is ignored by the physics update loop
    //pGridGroundMesh->drawPosition.y = 0.0f;   //  0,-10,0
    //pGridGroundMesh->friendlyName = "Ground";
    //::g_vec_pMeshesToDraw.push_back(pGridGroundMesh);

//    cMesh* pGroundMesh = new cMesh();
////    pFlat_1x1_planeMesh->meshName = "Flat_1x1_plane.ply";
//    pGroundMesh->meshName = "Terrain_xyz_n_rgba_uv.ply";
////    pGroundMesh->meshName = "HilbertRamp_stl (rotated).ply";
//    pGroundMesh->friendlyName = "Ground";
//    pGroundMesh->textureName[0] = "TaylorSwift_Eras_Poster.bmp";
//    pGroundMesh->textureRatios[0] = 1.0f;
//    ::g_vec_pMeshesToDraw.push_back(pGroundMesh);

    // Add matching physics object
//    sPhysicsProperties* pGroundMeshShape = new sPhysicsProperties();
//    pGroundMeshShape->shapeType = sPhysicsProperties::MESH_OF_TRIANGLES_INDIRECT;
//
////    pGroundMeshShape->setShape( new sPhysicsProperties::sMeshOfTriangles_Indirect("HilbertRamp_stl (rotated).ply") );
//    pGroundMeshShape->setShape( new sPhysicsProperties::sMeshOfTriangles_Indirect(pGroundMesh->meshName) );
//
//
//    // Tie this phsyics object to the associated mesh
//    pGroundMeshShape->pTheAssociatedMesh = pGroundMesh;
//    // If it's infinite, the physics intrator ignores it
//    pGroundMeshShape->inverse_mass = 0.0f;  // Infinite, so won't move
//
////    pGroundMeshShape->acceleration.y = (-9.81f / 5.0f);
//
////    pGroundMeshShape->position.x = -10.0f;
//    pGroundMeshShape->position.y = -50.0f;
////    pGroundMeshShape->orientation.z = glm::radians(-45.0f);
//    pGroundMeshShape->friendlyName = "Ground";
//    ::g_pPhysics->AddShape(pGroundMeshShape);
   
    //pFlat_1x1_planeMesh->bIsWireframe = true;
    //pFlat_1x1_planeMesh->bDoNotLight = true;
    // note this does NOT have a physProps, so is ignored by the physics update loop
//    sTransformInfo transformPlane = pFlat_1x1_planeMesh->getTransformInfo();
//    transformPlane.position.y = -40.0f;   //  0,-10,0
//    transformPlane.position.x = -10.0f;   //  0,-10,0
//    pFlat_1x1_planeMesh->setTransformInfo(transformPlane);
//    pFlat_1x1_planeMesh->drawPosition.x = 10.0f;
//    pFlat_1x1_planeMesh->orientation.z = glm::radians(+12.0f);
    // Also add the physics items
//   sPhysicsProperties* pFlat_1x1_plane = new sPhysicsProperties();
//   pFlat_1x1_plane->pTheAssociatedMesh = pFlat_1x1_planeMesh;
//   ::g_vec_pPhysicalProps.push_back(pFlat_1x1_plane);

//    cMesh* pFlat_1x1_planeMesh_DEBUG = new cMesh();
//    pFlat_1x1_planeMesh_DEBUG->meshName = pGroundMesh->meshName;
//    //pFlat_1x1_planeMesh_DEBUG->bIsWireframe = true;
//    //pFlat_1x1_planeMesh_DEBUG->bDoNotLight = true;
//    //pFlat_1x1_planeMesh_DEBUG->bUseDebugColours = true;
//    //pFlat_1x1_planeMesh_DEBUG->wholeObjectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//
//    pFlat_1x1_planeMesh_DEBUG->drawPosition = pGroundMeshShape->position;
////    pFlat_1x1_planeMesh_DEBUG->drawOrientation = pGroundMeshShape->orientation;
//    pFlat_1x1_planeMesh_DEBUG->setDrawOrientation(pGroundMeshShape->get_qOrientation());
//    pFlat_1x1_planeMesh_DEBUG->setUniformDrawScale(1.001f);
//    pFlat_1x1_planeMesh_DEBUG->bIsVisible = false;
//    ::g_vec_pMeshesToDraw.push_back(pFlat_1x1_planeMesh_DEBUG);
//
//    const float MAX_SPHERE_LOCATION = 100.0f;
//    const float MAX_VELOCITY = 1.0f;

    // Make a bunch of spheres...
//    const unsigned int NUMBER_OF_SPHERES = 3;
//    for (unsigned int count = 0; count != NUMBER_OF_SPHERES; count++)
//    {
//        cMesh* pSphereMesh = new cMesh();
//        pSphereMesh->meshName = "Sphere_1_unit_Radius.ply";
//        pSphereMesh->friendlyName = "sphere" + std::to_string(count);
//        pSphereMesh->setUniformDrawScale(3.0f);
//        pSphereMesh->drawPosition = glm::vec3(0.0f, 100.0f, 100.f);
//
//        // Add drawing mesh to the things to draw
//        ::g_vec_pMeshesToDraw.push_back(pSphereMesh);
//
//        // Now the physical object to match this
//        sPhysicsProperties* pSpherePhysProps = new sPhysicsProperties();        // HEAP
//
//        pSpherePhysProps->velocity.y = 0.0f; // getRandomFloat(0.0f, MAX_VELOCITY);
//        pSpherePhysProps->velocity.x = 0.0f; // getRandomFloat(-MAX_VELOCITY, MAX_VELOCITY);
//        pSpherePhysProps->velocity.z = 0.0f; // getRandomFloat(-MAX_VELOCITY, MAX_VELOCITY);
//
//        pSpherePhysProps->acceleration.y = (-9.81f / 5.0f);
//
//        pSpherePhysProps->position.x = MathUtils::GetRandomFloat(-MAX_SPHERE_LOCATION, MAX_SPHERE_LOCATION);
//            // getRandomFloat(-MAX_SPHERE_LOCATION, MAX_SPHERE_LOCATION);
//        pSpherePhysProps->position.z = MathUtils::GetRandomFloat(-MAX_SPHERE_LOCATION, MAX_SPHERE_LOCATION);
//            // getRandomFloat(-MAX_SPHERE_LOCATION, MAX_SPHERE_LOCATION);
//        pSpherePhysProps->position.y = MAX_SPHERE_LOCATION; //  getRandomFloat(10.0f, MAX_SPHERE_LOCATION + 20.0f);
//
//        // Mass of 10 somethings? kg?
//        pSpherePhysProps->inverse_mass = 1.0f / 10.0f;
//
//        pSpherePhysProps->shapeType = sPhysicsProperties::SPHERE;
//
//        // The rendered graphical object that moves with this physics object
//        pSpherePhysProps->pTheAssociatedMesh = pSphereMesh;
//
////        sPhysicsProperties::sSphere* pTemp = new sPhysicsProperties::sSphere(1.0f);
////        pSpherePhysProps->setShape(pTemp);
//
//        pSpherePhysProps->setShape( new sPhysicsProperties::sSphere(5.0f) );
//        ::g_pPhysics->AddShape(pSpherePhysProps);
//
//    }//for ( unsigned int count...


//    float spiderScale = 10.0f;
//
//    cMesh* pSpiderMan_body = new cMesh();
//    pSpiderMan_body->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->friendlyName = "SpiderManBody";
//    pSpiderMan_body->meshName = "legospiderman_body_xyz_n_rgba.ply";
////    pSpiderMan_body->drawOrientation.x = glm::radians(-90.0f);
////    pSpiderMan_body->drawOrientation.z = glm::radians(-90.0f);
//
//    pSpiderMan_body->adjustRoationAngleFromEuler( glm::vec3(-90.0f, 0.0f, 0.0f) );
////    pSpiderMan_body->adjustRoationAngleFromEuler( glm::vec3(0.0f, 0.0f, -90.0f) );
//
//
//
//    ::g_vec_pMeshesToDraw.push_back(pSpiderMan_body);
//
//    // Now add these as child meshes
//    cMesh* pSpiderMan_head = new cMesh();
//    pSpiderMan_head->setUniformDrawScale(spiderScale);
//    pSpiderMan_head->meshName = "legospiderman_head_xyz_n_rgba.ply";
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_head);
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManHead);
//
//    cMesh* pSpiderMan_Hips = new cMesh();
//    pSpiderMan_Hips->setUniformDrawScale(spiderScale);
//    pSpiderMan_Hips->meshName = "legospiderman_Hips_xyz_n_rgba.ply";
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Hips);
//
//    cMesh* pSpiderMan_Left_arm = new cMesh();
//    pSpiderMan_Left_arm->meshName = "legospiderman_Left_arm_xyz_n_rgba.ply";
//    pSpiderMan_Left_arm->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Left_arm);
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManLeft_arm);
//
//    cMesh* pSpiderMan_Left_hand = new cMesh();
//    pSpiderMan_Left_hand->meshName = "legospiderman_Left_hand_xyz_n_rgba.ply";
//    pSpiderMan_Left_hand->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Left_hand);
//
//    cMesh* pSpiderMan_Right_arm = new cMesh();
//    pSpiderMan_Right_arm->meshName = "legospiderman_Right_arm_xyz_n_rgba.ply";
//    pSpiderMan_Right_arm->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Right_arm);
//
//    cMesh* pSpiderMan_Right_hand = new cMesh();
//    pSpiderMan_Right_hand->meshName = "legospiderman_Right_hand_xyz_n_rgba.ply";
//    pSpiderMan_Right_hand->setUniformDrawScale(spiderScale);
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Right_hand);
//
//    cMesh* pSpiderMan_Left_leg = new cMesh();
//    pSpiderMan_Left_leg->setUniformDrawScale(spiderScale);
//    pSpiderMan_Left_leg->meshName = "legospiderman_Left_leg_xyz_n_rgba.ply";
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Left_leg);
////    ::g_vec_pMeshesToDraw.push_back(pSpiderManLeft_leg);
//
//    cMesh* pSpiderMan_Right_leg = new cMesh();
//    pSpiderMan_Right_leg->setUniformDrawScale(spiderScale);
//    pSpiderMan_Right_leg->meshName = "legospiderman_Right_leg_xyz_n_rgba.ply";
//    pSpiderMan_body->vec_pChildMeshes.push_back(pSpiderMan_Right_leg);




//    cMesh* pSpiderManHead = new cMesh();
//    pSpiderManHead->setUniformDrawScale(spiderScale);
//    pSpiderManHead->meshName = "legospiderman_head_xyz_n_rgba.ply";
//    ::g_vec_pMeshesToDraw.push_back(pSpiderManHead);
//
//    cMesh* pSpiderManLeft_arm = new cMesh();
//    pSpiderManLeft_arm->meshName = "legospiderman_Left_arm_xyz_n_rgba.ply";
//    ::g_vec_pMeshesToDraw.push_back(pSpiderManLeft_arm);
//
//    cMesh* pSpiderManLeft_leg = new cMesh();
//    pSpiderManLeft_leg->setUniformDrawScale(spiderScale);
//    pSpiderManLeft_leg->meshName = "legospiderman_Left_leg_xyz_n_rgba.ply";
//    ::g_vec_pMeshesToDraw.push_back(pSpiderManLeft_leg);

        
    return true;
}

bool LoadPhysics(void)
{
    //if (!g_currentScene->m_scenePhysics)
    //{
    //    g_currentScene->m_scenePhysics = new cPhysics();
    //    g_currentScene->m_scenePhysics->setVAOManager(::g_pMeshManager);
    //}

    //cMesh* pGroundMesh = cGameManager::getGmInstance()->FindMeshByFriendlyName("snowground");
    //if (pGroundMesh)
    //{
    //    sPhysicsProperties* pGroundMeshShape = new sPhysicsProperties();
    //    pGroundMeshShape->shapeType = sPhysicsProperties::MESH_OF_TRIANGLES_INDIRECT;
    //    pGroundMeshShape->setShape(new sPhysicsProperties::sMeshOfTriangles_Indirect(pGroundMesh->meshName));
    //    pGroundMeshShape->pTheAssociatedMesh = pGroundMesh;
    //    pGroundMeshShape->inverse_mass = 0.0f;  // Infinite, so won't move
    //    pGroundMeshShape->position.y = pGroundMesh->drawPosition.y;
    //    pGroundMeshShape->friendlyName = "snowground";
    //    pGroundMeshShape->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    //    pGroundMeshShape->setRotationFromEuler(glm::vec3(0.0f, 0.0f, 0.0f));
    //    ::g_pPhysics->AddShape(pGroundMeshShape);

    //    return true;
    //}

    return false;
}