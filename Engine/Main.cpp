//https://www.glfw.org/docs/latest/quick.html
//http://graphics.stanford.edu/data/3Dscanrep/

#include "OpenGLCommon.h"
//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "cGlobal.h"

#include <fstream>      // C++ file IO (secret: it's a wraper for the c IO)
#include <sstream>      // like a string builder
#include <vector>       // Aka a "smart array"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <gl/GL.h>

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/easing.hpp>

#include "Basic Shader Manager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"

#include "GLWF_CallBacks.h" // keyboard and mouse input

#include "cCamera.h"

#include "cLightManager.h"
#include "cLightHelper.h"
#include "cGameObject.h"

#include <algorithm>

#include "TextureManager/cBasicTextureManager.h"
#include "cGameManager.h"

// Utils
#include "cColor.h"
#include "MathUtils.h"
#include "WindowsUtils.h"

#include "cFlyCamera.h"
#include "cInputHandler.h"
#include "cAnimationSystem.h"
#include "cUiManager.h"
#include "cCameraManager.h"
#include "cSphere.h"
#include "cLionPlayer.h"
#include "cEnemyManager.h"
#include "cScene.h"
#include "cParticleSystem.h"
#include "cSoftBodyVerlet.h"

#include "fbo/cFBO.h"

// maze maker
#include "cMazeMaker.h"

#include "Engine.h"
#include "Game.h"

#include "cMeshNav.h"
#include "cHiResTimer.h"

#include "cCube.h"
#include "cTreasureHunter.h"

cInputHandler* g_inputHandler = NULL;

cGameManager* cGameManager::gmInstance = NULL;
cUiManager* g_uiManager = NULL;

cVAOManager* g_pMeshManager = NULL;

cBasicTextureManager* g_pTextureManager = NULL;

cMesh* g_pDebugSphereMesh = NULL;
GLuint g_DebugSphereMesh_shaderProgramID = 0;

std::vector<double> g_vecLastFrameTimes;

std::vector<cScene*> g_scenes;
cScene* g_currentScene = NULL;

cFlyCamera* g_pFlyCamera = NULL;

cAnimationSystem* g_animationSystem = NULL;

cLionPlayer* lionPlayer = NULL;

cParticleSystem g_anEmitter;

cParticleSystem* rainParticleSystem = NULL;
cMesh* g_particleMeshModel = NULL;
cMesh* g_rainMeshModel = NULL;

cMesh* tractorMesh = nullptr;
cMesh* chain1Mesh = nullptr;
cMesh* chain2Mesh = nullptr;
cMesh* chain3Mesh = nullptr;
cMesh* chain4Mesh = nullptr;

cMesh* bridgeMesh = nullptr;

cMesh* oceanMesh = nullptr;

cMesh* islandTerrain = nullptr;

cMesh* g_rainSplashMesh = nullptr;

cSoftBodyVerlet* g_softBody;
std::vector< cSoftBodyVerlet* > chainSoftBodies;

cShaderManager* shaderManager = nullptr;

float g_HeightAdjust = 10.0f;
glm::vec2 g_UVOffset = glm::vec2(0.0f, 0.0f);

Engine engine;
GLuint shaderProgramID = 0;

Game game;

// TODO: replace this for data members in cScene
int g_selectedMesh = 0;
int g_selectedLight = 0;

bool inGame = false;
cShaderManager::cShader computeShader;

unsigned int computeShaderID = 0;

cMediaPlayer* mediaPlayer = nullptr;



int main(void)
{
    srand((unsigned int)time(0));

    mediaPlayer = new cMediaPlayer();
    mediaPlayer->Initialize();
    //mediaPlayer->PlayAudio("christmas.wav");

    GLFWwindow* window;
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    std::string windowTitle = "Project 2 - Game Engine Frameworks & Patterns";

    window = glfwCreateWindow(640, 480, windowTitle.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    Engine::Init(window);
    engine.SetWindowName(windowTitle);

    g_currentScene->Init();

    cHiResTimer* p_HRTimer = new cHiResTimer(60);

    float yaxisRotation = 0.0f;
    double lastTime = glfwGetTime();
    float frameRate = 0.0f;
    double deltaTime = 0.0;

    g_currentScene->Awake(window);
    game.Awake(*window);

    g_currentScene->Start(window);
    game.Start(*window);

    while (!glfwWindowShouldClose(window))
    {
        deltaTime = p_HRTimer->getFrameTime();

        game.EarlyUpdate(*window, deltaTime);

        //{
        //    glBindFramebuffer(GL_FRAMEBUFFER, engine.g_FBO_1->ID);
        //    float ratio;
        //    //int width, height;
        //    //glfwGetFramebufferSize(window, &width, &height);
        //    //ratio = width / (float)height;

        //    ratio = engine.g_FBO_1->width / (float)engine.g_FBO_1->height;

        //    //engine.g_FBO_1->clearBuffers(true, true);
        //    glViewport(0, 0, engine.g_FBO_1->width, engine.g_FBO_1->height);
        //    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //    engine.g_FBO_1->clearBuffers(true, true);

        //    glm::vec3 sceneCameraEye = cameras[1]->GetPosition();
        //    glm::vec3 sceneCameraTarget = cameras[1]->GetTarget();

        //    engine.DrawPass1(window, deltaTime, engine.g_FBO_1->width,
        //        engine.g_FBO_1->height, sceneCameraEye, sceneCameraTarget);

        //    // Same scene, but from a different camera location
        //    glBindFramebuffer(GL_FRAMEBUFFER, engine.g_FBO_2->ID);

        //    ratio = engine.g_FBO_2->width / (float)engine.g_FBO_2->height;

        //    //engine.g_FBO_1->clearBuffers(true, true);
        //    glViewport(0, 0, engine.g_FBO_2->width, engine.g_FBO_2->height);
        //    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //    engine.g_FBO_2->clearBuffers(true, true);

        //    sceneCameraEye = cameras[2]->GetPosition();
        //    sceneCameraTarget = cameras[2]->GetTarget();

        //    engine.DrawPass1(window, deltaTime, engine.g_FBO_2->width,
        //        engine.g_FBO_2->height, sceneCameraEye, sceneCameraTarget);
        //}

        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            float ratio;
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            ratio = width / (float)height;

            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            engine.DrawPass2(window, deltaTime, width, height);
        }

        engine.Update(deltaTime, *window, shaderProgramID);
        g_currentScene->Update(window, deltaTime);
        game.Update(*window, deltaTime);
        //GameUpdate(deltaTime);


        //g_currentScene->DisplaySceneSkyBox(shaderProgramID, deltaTime);

        // Update game objects
        //for (unsigned int index = 0; index != g_gameObjects.size(); index++)
        //{
        //    g_gameObjects[index]->Update();
        //}

        ////glBindFramebuffer(GL_FRAMEBUFFER, engine.g_FBO_1->ID);
        ////engine.g_FBO_1->clearBuffers(true, true);
        ////engine.g_FBO_1->clearColourBuffer(0);
        ////engine.g_FBO_1->clearDepthBuffer();

        ////glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ////float ratio;
        ////int width, height;

        ////glfwGetFramebufferSize(window, &width, &height);
        ////ratio = width / (float)height;

        ////glViewport(0, 0, width, height);
        ////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ////// While drawing a pixel, see if the pixel that's already there is closer or not?
        ////glEnable(GL_DEPTH_TEST);
        ////// (Usually) the default - does NOT draw "back facing" triangles
        ////glEnable(GL_CULL_FACE);
        ////glCullFace(GL_BACK);

        //g_currentScene->DisplaySceneSkyBox(shaderProgramID);

        //// ***********************************************************************
        ////if (!::g_vecAnimationCommands.empty())
        ////{
        ////    ::g_vecAnimationCommands[0].Update(deltaTime);
        ////    // Done? 
        ////    if (::g_vecAnimationCommands[0].isDone())
        ////    {
        ////        // Erase the item from vector
        ////        // Is "slow" in that it has to copy the vector again
        ////        ::g_vecAnimationCommands.erase(::g_vecAnimationCommands.begin());
        ////    }
        ////}//if (!vecAnimationCommands.empty())
        //// ***********************************************************************

        g_anEmitter.Update(deltaTime);
        //rainParticleSystem->Update(deltaTime);

        //std::vector< cParticleSystem::sParticleRender> particleList;
        //engine.g_DrawDebugSphere(g_anEmitter.GetPosition(), 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), deltaTime);
        //g_anEmitter.GetParticleList(particleList);
        //for (cParticleSystem::sParticleRender& particle : particleList)
        //{
        //    glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

        //    //::g_DrawDebugSphere(particle.position, 0.1f, glm::vec4(0.0f,0.0f,0.0f, 1.0f));
        //    g_particleMeshModel->drawPosition = particle.position;
        //    g_particleMeshModel->setUniformDrawScale(1.0f);
        //    g_particleMeshModel->setDrawOrientation(particle.orientation);
        //    g_particleMeshModel->SetScale(particle.scaleXYZ);
        //    g_particleMeshModel->bDoNotLight = true;
        //    
        //    glDisable(GL_CULL_FACE);
        //    engine.DrawObject(g_particleMeshModel, matModel, shaderProgramID, deltaTime);
        //    glEnable(GL_CULL_FACE);
        //}

        //g_softBody->Update(deltaTime, shaderProgramID);


        //GLuint bIsOffScreenTextureQuad_ID = glGetUniformLocation(shaderProgramID, "bIsOffScreenTextureQuad");
        //glUniform1f(bIsOffScreenTextureQuad_ID, (GLfloat)GL_TRUE);

        {
            GLint textureUnitNumber = 50;
            //GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
            glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
            glBindTexture(GL_TEXTURE_2D, engine.g_FBO_2->colourTexture_0_ID);
            GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "textureOffScreen");
            glUniform1i(texture_01_UL, textureUnitNumber);
        }

        //cMesh* fboMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName("quad2");
        //engine.DrawObject(fboMesh, glm::mat4(1.0f), shaderProgramID);
        //glUniform1f(bIsOffScreenTextureQuad_ID, (GLfloat)GL_FALSE);

        //g_uiManager->Update(window);


        engine.EndOfFrame(deltaTime, *window);
    }

    engine.End(*window);
    game.End(*window);
}