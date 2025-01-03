#include "Engine.h"

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>


#include "cGlobal.h"
#include "MathUtils.h"

#include "cInputHandler.h"
#include "cUiManager.h"
#include "cGameObject.h"
#include "cFlyCamera.h"
#include "cLightManager.h"

#include "Basic Shader Manager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"

#include "TextureManager/cBasicTextureManager.h"

#include "cCameraManager.h"
#include "cScene.h"
#include "cSoftBodyVerlet.h"
#include "cParticleSystem.h"
#include "cAnimationSystem.h"
#include "cLightHelper.h"

#include "WindowsUtils.h"
#include "cColor.h"
#include "LogUtils.h"

// For the compute shader example
#include "cGLBuffer_1D_Image.h"
#include "cGLBuffer_1D_SSBO.h"
#include "cGLComputeEuler.h"

extern cUiManager* g_uiManager;
extern cUiManager* g_uiManager;
extern cInputHandler* g_inputHandler;
extern cFlyCamera* g_pFlyCamera;
extern iCamera* g_camera;
extern bool bUseFlyCamera;
extern cMesh* g_objectSelected;
extern GLuint g_DebugSphereMesh_shaderProgramID;
extern GLuint shaderProgramID;
extern cBasicTextureManager* g_pTextureManager;
extern cMesh* g_pDebugSphereMesh;
extern cMesh* g_particleMeshModel;
extern cMesh* g_rainMeshModel;
extern cVAOManager* g_pMeshManager;
extern cMesh* islandTerrain;

extern cScene* g_currentScene;
extern cParticleSystem g_anEmitter;
extern cShaderManager::cShader computeShader;
extern cParticleSystem* rainParticleSystem;

std::vector<cFBO*> Engine::g_FBOs;

cFBO* Engine::g_FBO_1 = NULL;
cFBO* Engine::g_FBO_2 = NULL;

extern cSoftBodyVerlet* g_softBody;
extern std::vector< cSoftBodyVerlet* > chainSoftBodies;
extern cAnimationSystem* g_animationSystem;

extern cMesh* bridgeMesh;

extern cVAOManager* g_pMeshManager;
extern cMesh* g_rainSplashMesh;

extern cShaderManager* shaderManager;


extern unsigned int computeShaderID;

extern bool inGame;

//extern void DrawLightDebugSpheres(glm::mat4 matProjection, glm::mat4 matView,
//    GLuint shaderProgramID);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    g_inputHandler->KeyCallback(window, key, scancode, action, mods);
}

void windowSizeChangedCallback(GLFWwindow* window, int width, int height)
{
    //std::cout << "Screen is now: " 
    //   << width << " x " << height << std::endl;
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    g_inputHandler->MouseButtonCallback(window, button, action, mods);
}

static void MouseScrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_inputHandler->MouseScrollWheelCallback(window, xoffset, yoffset);
}

static void MouseEnteredWindowCallback(GLFWwindow* window, int entered)
{
    g_inputHandler->MouseEnteredWindowCallback(window, entered);
}

void Engine::OpenGlInit(GLFWwindow* window)
{
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //    GLuint vertex_buffer; //, vertex_shader, fragment_shader;//v , program;
    //    GLint mvp_location;// , vpos_location, vcol_location;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetWindowSizeCallback(window, windowSizeChangedCallback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // Create FBO
    g_FBO_1 = new cFBO();
    std::string FBOError;
    if (!g_FBO_1->init(1920, 1080, FBOError))
    {
        std::cout << "Error creating FBO: " << FBOError << std::endl;
    }

    g_FBO_2 = new cFBO();
    if (!g_FBO_2->init(1920, 1080, FBOError))
    {
        std::cout << "Error creating FBO: " << FBOError << std::endl;
    }

    g_FBOs.push_back(g_FBO_1);
    g_FBOs.push_back(g_FBO_2);

    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, MouseScrollWheelCallback);
    glfwSetCursorEnterCallback(window, MouseEnteredWindowCallback);





    shaderManager = new cShaderManager();
    shaderManager->setBasePath("assets/shaders");


#pragma region compute_01_image1D 

    //{//compute_01_image1D
    //    cShaderManager::cShader computeShader;
    //    computeShader.shaderType = cShaderManager::cShader::COMPUTE_SHADER;
    //    computeShader.fileName = "compute_01_image1D.glsl";

    //    //    if ( ! pShaderThing->createProgramFromFile("shader01", vertexShader, fragmentShader ) )
    //    if (!pShaderThing->createProgramFromFile("compute1DImage", computeShader))
    //    {
    //        std::cout << "Error: Couldn't compile or link:" << std::endl;
    //        std::cout << pShaderThing->getLastError();
    //        return -1;
    //    }
    //    else
    //    {
    //        std::cout << "Compute shader compiled OK" << std::endl;
    //    }

    //    unsigned int compute1DImageID = pShaderThing->getIDFromFriendlyName("compute1DImage");

    //    cGLBuffer_1D_Image compute1DImageBuffer;

    //    std::vector< glm::vec4 > vecMyDataIntoGPU;

    //    for ( unsigned int count = 0; count != 2048; count++ )
    //    {
    //        glm::vec4 curData = glm::vec4(0.0f);
    //        curData.x = float(count);
    //        curData.y = 100.0f;
    //        curData.z = 200.0f;
    //        curData.w = 300.0f;
    //        vecMyDataIntoGPU.push_back(curData);
    //    }

    //    compute1DImageBuffer.init(compute1DImageID, vecMyDataIntoGPU);

    //    glDispatchCompute(1024, 1, 1);
    //    
    //    // If you need this right away, you'll have to wait...
    //    // You don't have to do this if you are setting something up 
    //    //  for the next frame.
    //    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    //    std::vector< glm::vec4 > vecMyDataOutOfGPU;
    //    compute1DImageBuffer.getData(vecMyDataOutOfGPU);

    //    for ( unsigned int index = 0; index != 5; index++ )
    //    {
    //        std::cout
    //            << "OG: " << Print_glm_vec(vecMyDataIntoGPU[index])
    //            << "\t"
    //            << "new: " << Print_glm_vec(vecMyDataOutOfGPU[index])
    //            << std::endl;
    //    }

    //}//compute_01_image1D

#pragma endregion

#pragma region cGLBuffer_1D_SSBO

    //{//cGLBuffer_1D_SSBO
    //    cShaderManager::cShader computeShader;
    //    computeShader.shaderType = cShaderManager::cShader::COMPUTE_SHADER;
    //    computeShader.fileName = "compute_01_SSBO_1D.glsl";

    //    //    if ( ! pShaderThing->createProgramFromFile("shader01", vertexShader, fragmentShader ) )
    //    if (!shaderManager->createProgramFromFile("compute01_SSBO_1D", computeShader))
    //    {
    //        std::cout << "Error: Couldn't compile or link: " << computeShader.fileName << std::endl;
    //        std::cout << shaderManager->getLastError();
    //    }
    //    else
    //    {
    //        std::cout << "Compute shader compiled OK" << std::endl;
    //    }

    //    cGLBuffer_1D_SSBO compute_1D_SSBO_Buffer;

    //    std::vector<glm::vec4> vecObjects;
    //    const unsigned int NUMBER_OF_OBJECTS = 1024;

    //    for (unsigned int index = 0; index != NUMBER_OF_OBJECTS; index++)
    //    {
    //        glm::vec4 newPoint;
    //        //        newPoint.x = getRandomFloat(0.0f, 100.0f);
    //        newPoint.x = (float)index + 1.0f;
    //        newPoint.y = newPoint.x;
    //        newPoint.z = newPoint.x;
    //        newPoint.w = newPoint.x;

    //        vecObjects.push_back(newPoint);
    //    }

    //    if (!compute_1D_SSBO_Buffer.init(computeShader.ID, vecObjects))
    //    {
    //        std::cout << "Error: buffer init didn't work" << std::endl;
    //    }

    //    unsigned int coputeProgramID = shaderManager->getIDFromFriendlyName("compute01_SSBO_1D");
    //    glUseProgram(coputeProgramID);

    //    gDEBUG_PROCESS_GL_ERROR


    //        //    GLint numberOfActiveUniforms = 0;
    //        //    glGetProgramiv(computeShader.ID,	// Shader program ID ('name')
    //        //                   GL_ACTIVE_UNIFORMS,		// Give me how many active uniforms there are
    //        //                   &numberOfActiveUniforms);
    //        //
    //        //
    //        //    GLint deltaTime_ID = glGetUniformLocation(computeShader.ID, "deltaTime");
    //        //    glUniform1f(deltaTime_ID, 0.1f);

    //        compute_1D_SSBO_Buffer.run();
    //    gDEBUG_PROCESS_GL_ERROR

    //        //        glDispatchCompute(128, 1, 1);
    //        //        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    //        std::vector<glm::vec4> vecObjectsUpdated;

    //    compute_1D_SSBO_Buffer.getData(vecObjectsUpdated);
    //    gDEBUG_PROCESS_GL_ERROR


    //        for (unsigned int index = 0; index != 10; index++)
    //        {
    //            std::cout << index << ": ";
    //            LogUtils::Vector3Log(vecObjects[index]);
    //            LogUtils::Vector3Log(vecObjectsUpdated[index]);
    //        }
    //}//cGLBuffer_1D_SSBO

#pragma endregion

#pragma region Computer_Shader_SSBO_Euler_update

//    {//Computer_Shader_SSBO_Euler_update
    
    computeShader.shaderType = cShaderManager::cShader::COMPUTE_SHADER;
    computeShader.fileName = "compute_02_SSBO_Euler.glsl";

    //    if ( ! pShaderThing->createProgramFromFile("shader01", vertexShader, fragmentShader ) )
    if (!shaderManager->createProgramFromFile("computeEuler", computeShader))
    {
        std::cout << "Error: Couldn't compile or link:" << computeShader.fileName << std::endl;
        std::cout << shaderManager->getLastError();
    }
    else
    {
        //std::cout << "Compute shader compiled OK" << std::endl;
    }

    computeShaderID = shaderManager->getIDFromFriendlyName("computeEuler");

    cGLComputeEuler physicsObjectsBuffer;

    std::vector<cGLComputeEuler::sPysicsState> vecObjects;
    const unsigned int NUMBER_OF_OBJECTS = 2000;
    glm::vec3 startPositionLimitsMin = glm::vec3(-50.0f, 0.0f, -50.0f);
    glm::vec3 startPositionLimitsMax = glm::vec3(50.0f, 50.0f, 50.0f);
    for (unsigned int index = 0; index != NUMBER_OF_OBJECTS; index++)
    {
        cGLComputeEuler::sPysicsState newObject;
        newObject.position.x = MathUtils::GetRandomFloat(startPositionLimitsMin.x, startPositionLimitsMax.x);
        newObject.position.y = MathUtils::GetRandomFloat(startPositionLimitsMin.y, startPositionLimitsMax.y);
        newObject.position.z = MathUtils::GetRandomFloat(startPositionLimitsMin.z, startPositionLimitsMax.z);

        newObject.velocity.x = 0.0f;
        // Start with +1 to 5 upward velocity
        newObject.velocity.y = MathUtils::GetRandomFloat(1.0f, 5.0f);
        newObject.velocity.z = 0.0f;

        newObject.acceleration = glm::vec4(0.0f, -9.81f, 0.0f, 1.0f);

        vecObjects.push_back(newObject);
    }

    if (!physicsObjectsBuffer.init(computeShaderID, vecObjects))
    {
        std::cout << "Error: buffer init didn't work" << std::endl;
    }
    ::g_ProcessOpenGLError(__FILE__, __LINE__);


    glUseProgram(computeShaderID);

    GLint numberOfActiveUniforms = 0;
    glGetProgramiv(computeShaderID,	// Shader program ID ('name')
        GL_ACTIVE_UNIFORMS,		// Give me how many active uniforms there are
        &numberOfActiveUniforms);


    GLint computeEuler_deltaTime_ID = glGetUniformLocation(computeShaderID, "deltaTime");
    glUniform1f(computeEuler_deltaTime_ID, 0.1f);


    glDispatchCompute(NUMBER_OF_OBJECTS, 1, 1);
    ::g_ProcessOpenGLError(__FILE__, __LINE__);

    // If you want to wait until the compute is done.
    // You don't have to do this, like if you have something else to do, like render, etc.
    // You could also sleep() for a bit?
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glMemoryBarrier.xhtml
    // https://learnopengl.com/Guest-Articles/2022/Compute-Shaders/Introduction
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    ::g_ProcessOpenGLError(__FILE__, __LINE__);


    std::vector<cGLComputeEuler::sPysicsState> vecObjectsUpdated;
    if (!physicsObjectsBuffer.getData(computeShaderID, vecObjectsUpdated))
    {
        std::cout << "Error buffer getData() didn't work" << std::endl;
    }
    ::g_ProcessOpenGLError(__FILE__, __LINE__);

    //LogUtils::Vector3Log("Original pos: ", vecObjects[0].position);
    //LogUtils::Vector3Log("Original vel: ", vecObjects[0].velocity);
    //LogUtils::Vector3Log("New pos: ", vecObjectsUpdated[0].position);
    //LogUtils::Vector3Log("New vel: ", vecObjectsUpdated[0].velocity);

    //std::cout << "Original pos: " << Print_glm_vec(vecObjects[0].position) << std::endl;
    //std::cout << "Original vel: " << Print_glm_vec(vecObjects[0].velocity) << std::endl;
    //std::cout << "New pos: " << Print_glm_vec(vecObjectsUpdated[0].position) << std::endl;
    //std::cout << "New vel: " << Print_glm_vec(vecObjectsUpdated[0].velocity) << std::endl;
    //    }//Computer_Shader_SSBO_Euler_update

#pragma endregion


    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "vertexShader01.glsl";

    cShaderManager::cShader geometryShader;
    geometryShader.fileName = "geometryPassThrough.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "fragmentShader01.glsl";

    if (!shaderManager->createProgramFromFile("shader01", vertexShader, geometryShader, fragmentShader))
    {
        std::cout << "Error: Couldn't compile or link:" << std::endl;
        std::cout << shaderManager->getLastError();
        //return -1;
    }

    shaderProgramID = shaderManager->getIDFromFriendlyName("shader01");

    // Set the debug shader ID we're going to use
    ::g_DebugSphereMesh_shaderProgramID = shaderProgramID;
    ::g_pMeshManager = new cVAOManager();
    ::g_pMeshManager->setBasePath("assets/models");

    // loading json info
    sSceneInfo sceneInfo;
    std::vector<std::string> modelInfoWithTexture;
    std::vector<std::string> fbxModelsInfo;
    std::vector<std::string> modelInfo;
    std::vector<std::string> texturesInfo;

    cJsonHandler configReaderWriter;
    if (!configReaderWriter.LoadJsonInfo(sceneInfo, modelInfoWithTexture, modelInfo, fbxModelsInfo, texturesInfo))
    {
        std::cout << "json files not loaded" << std::endl;
    }

    for (unsigned int i = 0; i < modelInfoWithTexture.size(); i++)
    {
        sModelDrawInfo model;
        if (!::g_pMeshManager->LoadModelIntoVAO(modelInfoWithTexture[i], model, shaderProgramID))
        {
            std::cout << "Model: " << modelInfoWithTexture[i] << " failed to load" << std::endl;
        }
    }

    for (unsigned int i = 0; i < modelInfo.size(); i++)
    {
        sModelDrawInfo model;
        if (!::g_pMeshManager->LoadModelIntoVAOWithoutTexture(modelInfo[i], model, shaderProgramID))
        {
            std::cout << "Model: " << modelInfo[i] << " failed to load" << std::endl;
        }
    }

    for (unsigned int i = 0; i < fbxModelsInfo.size(); i++)
    {
        sModelDrawInfo model;
        if (!::g_pMeshManager->LoadModelWithAssimp(fbxModelsInfo[i], model, shaderProgramID))
        {
            std::cout << "Model: " << fbxModelsInfo[i] << " failed to load" << std::endl;
        }
    }

    const char* folderRelativePath = "assets/models/game";

    std::vector < std::string> modelTypes;
    WindowsUtils::GetAllFoldersFromFolder(folderRelativePath, modelTypes);

    for (unsigned int i = 0; i < modelTypes.size(); i++)
    {
        std::string fileInFolderPath = std::string(folderRelativePath) + "/" + modelTypes[i];

        std::vector < std::string> filesInFolder;
        WindowsUtils::GetAllFilesFromFolder(fileInFolderPath, filesInFolder);

        cVAOManager::ModelInfo modelInfo;

        modelInfo.modelFolderName = modelTypes[i];

        for (unsigned int i = 0; i < filesInFolder.size(); i++)
        {
            ::g_pMeshManager->setBasePath(fileInFolderPath);

            modelInfo.modelFiles.push_back(filesInFolder[i]);

            sModelDrawInfo model;
            if (!::g_pMeshManager->LoadModelIntoVAO(filesInFolder[i], model, shaderProgramID))
            {
                std::cout << "Model: " << filesInFolder[i] << " failed to load" << std::endl;
            }
            //if (!::g_pMeshManager->LoadModelWithAssimp(filesInFolder[i], model, shaderProgramID))
            //{
            //    std::cout << "Model: " << filesInFolder[i] << " failed to load" << std::endl;
            //}
        }

        ::g_pMeshManager->modelInfo.push_back(modelInfo);
    }

    //std::vector< std::string> gameAssets;
    //WindowsUtils::GetAllFilesFromFolder(folderRelativePath, gameAssets);


    ::g_pMeshManager->setBasePath("assets/models");

    // lights
    g_currentScene->m_sceneLights = new cLightManager();
    g_currentScene->m_sceneLights->SetUniformLocations(shaderProgramID);

    std::vector<Bones::CharacterAnimation*> animations;

    LoadAnimations(animations);
    LoadTexturesFromFile(texturesInfo);

    const char* texturesRelativePath = "assets/textures/game";

    std::vector < std::string> texturesTypes;
    WindowsUtils::GetAllFoldersFromFolder(texturesRelativePath, texturesTypes);

    for (unsigned int i = 0; i < texturesTypes.size(); i++)
    {
        std::string fileInFolderPath = std::string(texturesRelativePath) + "/" + texturesTypes[i];

        std::vector < std::string> filesInFolder;
        WindowsUtils::GetAllFilesFromFolder(fileInFolderPath, filesInFolder);

        cVAOManager::TextureInfo textureInfo;

        textureInfo.textureFolderName = texturesTypes[i];

        for (unsigned int j = 0; j < filesInFolder.size(); j++)
        {
            ::g_pMeshManager->setBasePath(fileInFolderPath);

            textureInfo.textureFiles.push_back(filesInFolder[j]);

            std::string textureRelativePath = std::string("assets/textures/game") + "/" + texturesTypes[i];

            ::g_pTextureManager->SetBasePath(textureRelativePath);

            if (!::g_pTextureManager->Create2DTextureFromBMPFile(filesInFolder[j], true))
            {
                std::cout << filesInFolder[j] << " failed to load in " << texturesTypes[i] << std::endl;
            }
        }

        ::g_pMeshManager->textureInfo.push_back(textureInfo);
    }


    LoadLights(sceneInfo.LightsInfo, shaderProgramID);
    LoadPhysics();
    LoadModels(sceneInfo.ModelInfo);

    cCameraManager::LoadCameras(sceneInfo.CameraInfo, g_currentScene->m_gameObjects);
}

void Engine::Init(GLFWwindow* window)
{
    ::g_animationSystem = new cAnimationSystem();
    ::g_inputHandler = new cInputHandler();
    ::g_mediaPlayer = new cMediaPlayer();
    ::g_currentScene = new cScene();

    OpenGlInit(window);

    g_uiManager = new cUiManager();
    g_uiManager->Init(window);
}

void Engine::DrawPass1(GLFWwindow* window, double deltaTime, int width, int height, glm::vec3 scene_CameraEye, glm::vec3 scene_CameraTarget)
{
    float ratio;

    glUseProgram(shaderProgramID);

    //glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float)height;

    //glViewport(0, 0, width, height);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // While drawing a pixel, see if the pixel that's already there is closer or not?
    glEnable(GL_DEPTH_TEST);
    // (Usually) the default - does NOT draw "back facing" triangles
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // *****************************************************************
    g_currentScene->m_sceneLights->UpdateUniformValues(shaderProgramID);
    // *****************************************************************

    //uniform vec4 eyeLocation;
    GLint eyeLocation_UL = glGetUniformLocation(shaderProgramID, "eyeLocation");

    //glm::vec3 cameraEye = g_camera->GetPosition();

    glUniform4f(eyeLocation_UL,
        scene_CameraEye.x, scene_CameraEye.y, scene_CameraEye.z, 1.0f);

    //glUniform4f(eyeLocation_UL,
    //    cameraEye.x, cameraEye.y, cameraEye.z, 1.0f);


    //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    //glm::mat4 matProjection = glm::perspective(0.6f,
    //                                           ratio,
    //                                           0.1f, // near clip
    //                                           10'000.0f); // far clip

    glm::mat4 matProjection;
    glm::mat4 matView;

    matProjection = glm::perspective(0.6f,
        ratio,
        g_pFlyCamera->GetNearClip(), // near clip
        g_pFlyCamera->GetFarClip()); // far clip

    //glm::mat4 matView = glm::lookAt(::g_cameraEye,
    //                                ::g_cameraTarget,
    //                                ::g_upVector);

    ////        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
    //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    //        mat4x4_mul(mvp, p, m);

    matView = glm::lookAt(scene_CameraEye,
        scene_CameraTarget,
        glm::vec3(0.0f,1.0f,0.0f));

    //if (::bUseFlyCamera)
    //{
    //    matProjection = glm::perspective(0.6f,
    //        ratio,
    //        g_pFlyCamera->GetNearClip(), // near clip
    //        g_pFlyCamera->GetFarClip()); // far clip

    //    //glm::mat4 matView = glm::lookAt(::g_cameraEye,
    //    //                                ::g_cameraTarget,
    //    //                                ::g_upVector);
    //    ////        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
    //    //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    //    //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    //    //        mat4x4_mul(mvp, p, m);

    //    matView = glm::lookAt(::g_pFlyCamera->getEye(),
    //        ::g_pFlyCamera->getAt(),
    //        ::g_pFlyCamera->getUp());
    //}
    //else
    //{
    //    matProjection = glm::perspective(0.6f,
    //        ratio,
    //        g_camera->GetNearClip(), // near clip
    //        g_camera->GetFarClip()); // far clip

    //    //glm::mat4 matView = glm::lookAt(::g_cameraEye,
    //    //                                ::g_cameraTarget,
    //    //                                ::g_upVector);


    //    switch (::g_inputHandler->currentMode)
    //    {
    //    case cInputHandler::EditorMode:

    //        matView = glm::lookAt(g_camera->GetPosition(),
    //            g_camera->GetTarget(),
    //            g_camera->GetUp());
    //        break;

    //    case cInputHandler::GameMode:
    //        glm::vec3 lookDir = g_camera->GetOrientation() * g_camera->GetForward();
    //        matView = glm::lookAt(
    //            g_camera->GetPosition(),
    //            g_camera->GetPosition() + lookDir,
    //            g_camera->GetUp()
    //        );
    //        break;
    //    }
    //}

    //glm::vec3 lookDir = g_camera->GetCameraOrientation() * g_camera->FORWARD_VECTOR;
    //glm::mat4 matView = glm::lookAt(
    //    g_camera->GetCameraPosition(),
    //    g_camera->GetCameraPosition() + lookDir,
    //    g_camera->UP_VECTOR
    //);

    GLint matProjection_UL = glGetUniformLocation(shaderProgramID, "matProjection");
    glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, glm::value_ptr(matProjection));

    GLint matView_UL = glGetUniformLocation(shaderProgramID, "matView");
    glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(matView));

    DrawLightDebugSpheres(matProjection, matView, shaderProgramID, deltaTime);

    //// Update game objects
    //for (unsigned int index = 0; index != g_gameObjects.size(); index++)
    //{
    //    g_gameObjects[index]->Update();
    //}

    //glBindFramebuffer(GL_FRAMEBUFFER, engine.g_FBO_1->ID);
    //engine.g_FBO_1->clearBuffers(true, true);
    //engine.g_FBO_1->clearColourBuffer(0);
    //engine.g_FBO_1->clearDepthBuffer();

    //// *********************************************************************
    // Draw all the objects
    //DrawObjects(shaderProgramID, deltaTime);

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //float ratio;
    //int width, height;

    //glfwGetFramebufferSize(window, &width, &height);
    //ratio = width / (float)height;

    //glViewport(0, 0, width, height);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //// While drawing a pixel, see if the pixel that's already there is closer or not?
    //glEnable(GL_DEPTH_TEST);
    //// (Usually) the default - does NOT draw "back facing" triangles
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);


    // Point the spotlight at the bathtub
    //cMesh* pBathTub = g_pFindMeshByFriendlyName("bathtub");
    //if ( pBathTub )
    //{
    //    glm::vec3 bathTubToLightRay = pBathTub->drawPosition - glm::vec3(::g_pTheLights->theLights[0].position);
    //    bathTubToLightRay = glm::normalize(bathTubToLightRay);
    //    ::g_pTheLights->theLights[0].direction = glm::vec4(bathTubToLightRay, 1.0f);
    //}

    //cMesh* objectToAdjust = g_pFindMeshByFriendlyName("bathtub");
    //if (objectToAdjust)
    //{
    //    //glm::vec3 bathTubToLightRay = objectToAdjust->drawPosition - glm::vec3(::g_pTheLights->theLights[1].position);
    //    //bathTubToLightRay = glm::normalize(bathTubToLightRay);
    //    //::g_pTheLights->theLights[1].direction = glm::vec4(bathTubToLightRay, 1.0f);
    //    ::g_cameraTarget = objectToAdjust->drawPosition;
    //}

    g_currentScene->DisplaySceneSkyBox(shaderProgramID, deltaTime);

    // ***********************************************************************
    //if (!::g_vecAnimationCommands.empty())
    //{
    //    ::g_vecAnimationCommands[0].Update(deltaTime);
    //    // Done? 
    //    if (::g_vecAnimationCommands[0].isDone())
    //    {
    //        // Erase the item from vector
    //        // Is "slow" in that it has to copy the vector again
    //        ::g_vecAnimationCommands.erase(::g_vecAnimationCommands.begin());
    //    }
    //}//if (!vecAnimationCommands.empty())
    // ***********************************************************************

    //g_uiManager->UpdateUi(window);

    g_camera->Update();

    g_currentScene->m_scenePhysics->DoPhysicUpdate(deltaTime);
    g_currentScene->m_scenePhysics->Update(deltaTime);


    //std::vector< cParticleSystem::sParticleRender> particleList;
    ////::g_DrawDebugSphere(g_anEmitter.GetPosition(), 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    //g_anEmitter.GetParticleList(particleList);
    ////for (cParticleSystem::sParticleRender& particle : particleList)
    ////{
    ////    glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

    ////    //::g_DrawDebugSphere(particle.position, 0.1f, glm::vec4(0.0f,0.0f,0.0f, 1.0f));
    ////    g_particleMeshModel->drawPosition = particle.position;
    ////    g_particleMeshModel->setUniformDrawScale(1.0f);
    ////    g_particleMeshModel->setDrawOrientation(particle.orientation);
    ////    g_particleMeshModel->SetScale(particle.scaleXYZ);
    ////    DrawObject(g_particleMeshModel, matModel, shaderProgramID);
    ////}


    //g_softBody->VerletUpdate(deltaTime);
    //g_softBody->ApplyCollision(deltaTime);
    //g_softBody->SatisfyConstraints();


    //for (cSoftBodyVerlet::sParticle* particle : g_softBody->vec_pParticles)
    //{
    //    glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

    //    //g_DrawDebugSphere(particle->position, 0.01f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), deltaTime);
    //    g_particleMeshModel->drawPosition = particle->position;
    //    g_particleMeshModel->setUniformDrawScale(1.0f);
    //    //g_particleMeshModel->setDrawOrientation(particle->orientation);
    //    DrawObject(g_particleMeshModel, matModel, shaderProgramID, deltaTime);
    //    //g_softBody.CreateSoftBody(particle);

    //}

    GLuint bIsOffScreenTextureQuad_ID = glGetUniformLocation(shaderProgramID, "bIsOffScreenTextureQuad");
    glUniform1f(bIsOffScreenTextureQuad_ID, (GLfloat)GL_TRUE);

    ////float ratio;
    //int screenWidth, screenHeight;
    //glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    //ratio = screenWidth / (float)screenHeight;
    ////            ratio = ::g_pFBO_1->width / (float)::g_pFBO_1->height;
    ////            glViewport(0, 0, ::g_pFBO_1->width, (float)::g_pFBO_1->height);

    //glViewport(0, 0, screenWidth, screenHeight);

    //GLint screenWidthAndHeight_UL = glGetUniformLocation(shaderProgramID, "screenWidthAndHeight");
    //glUniform2f(screenWidthAndHeight_UL,
    //    (GLfloat)screenWidth,
    //    (GLfloat)screenHeight);

    //{
    //    GLint textureUnitNumber = 50;
    //    //GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
    //    glActiveTexture(GL_TEXTURE0 + textureUnitNumber);

    //    glBindTexture(GL_TEXTURE_2D, g_FBO_2->colourTexture_0_ID);
    //    GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "textureOffScreen_0");
    //    glUniform1i(texture_01_UL, textureUnitNumber);
    //}

    //{
    //    GLint textureUnitNumber = 60;
    //    //GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
    //    glActiveTexture(GL_TEXTURE0 + textureUnitNumber);

    //    glBindTexture(GL_TEXTURE_2D, g_FBO_1->colourTexture_0_ID);
    //    GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "textureOffScreen_1");
    //    glUniform1i(texture_01_UL, textureUnitNumber);

    //    //return;

    //}




    DrawObjects(shaderProgramID, deltaTime);

    //cMesh* fboMesh = cGameManager::GetGMInstance()->FindMeshByFriendlyName("rightmapscreen");
    //DrawObject(fboMesh, glm::mat4(1.0f), shaderProgramID, deltaTime);

    glUniform1f(bIsOffScreenTextureQuad_ID, (GLfloat)GL_FALSE);
}

void Engine::DrawPass2(GLFWwindow* window, double deltaTime, int width, int height)
{
    float ratio;

    glUseProgram(shaderProgramID);

    //glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float)height;

    if (height == 0)
    {
        ratio = 1.0f;
    }

    //glViewport(0, 0, width, height);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // While drawing a pixel, see if the pixel that's already there is closer or not?
    glEnable(GL_DEPTH_TEST);
    // (Usually) the default - does NOT draw "back facing" triangles
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // *****************************************************************
    g_currentScene->m_sceneLights->UpdateUniformValues(shaderProgramID);
    // *****************************************************************

    //uniform vec4 eyeLocation;
    GLint eyeLocation_UL = glGetUniformLocation(shaderProgramID, "eyeLocation");

    glm::vec3 cameraEye = g_camera->GetPosition();

    glUniform4f(eyeLocation_UL,
        cameraEye.x, cameraEye.y, cameraEye.z, 1.0f);


    //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    //glm::mat4 matProjection = glm::perspective(0.6f,
    //                                           ratio,
    //                                           0.1f, // near clip
    //                                           10'000.0f); // far clip

    glm::mat4 matProjection;
    glm::mat4 matView;

    if (::bUseFlyCamera)
    {
        matProjection = glm::perspective(0.6f,
            ratio,
            g_pFlyCamera->GetNearClip(), // near clip
            g_pFlyCamera->GetFarClip()); // far clip

        //glm::mat4 matView = glm::lookAt(::g_cameraEye,
        //                                ::g_cameraTarget,
        //                                ::g_upVector);



        ////        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        //        mat4x4_mul(mvp, p, m);

        matView = glm::lookAt(::g_pFlyCamera->getEye(),
            ::g_pFlyCamera->getAt(),
            ::g_pFlyCamera->getUp());
    }
    else
    {
        matProjection = glm::perspective(0.6f,
            ratio,
            g_camera->GetNearClip(), // near clip
            g_camera->GetFarClip()); // far clip

        //glm::mat4 matView = glm::lookAt(::g_cameraEye,
        //                                ::g_cameraTarget,
        //                                ::g_upVector);

        //cMesh* captainMesh = g_currentScene->GetMeshByFriendlyName("captain1");

        //glm::vec3 cameraPosition = glm::vec3(
        //    captainMesh->drawPosition.x,
        //    captainMesh->drawPosition.y + 1200.0f,
        //    captainMesh->drawPosition.z + 2000.0f);

        glm::vec3 cameraPosition = g_camera->GetPosition();
        glm::vec3 cameraTarget = g_camera->GetTarget();

        glm::vec3 lookDir = g_camera->GetOrientation() * g_camera->GetForward();
        matView = glm::lookAt(cameraPosition,
            cameraPosition + lookDir ,
            g_camera->GetUp()
        );

        //break;


        //switch (::g_inputHandler->currentMode)
        //{
        //case cInputHandler::EditorMode:

        //    matView = glm::lookAt(g_camera->GetPosition(),
        //        g_camera->GetTarget(),
        //        g_camera->GetUp());
        //    break;

        //case cInputHandler::GameMode:
        //    glm::vec3 lookDir = g_camera->GetOrientation() * g_camera->GetForward();
        //    matView = glm::lookAt(
        //        g_camera->GetPosition(),
        //        g_camera->GetPosition() + lookDir,
        //        g_camera->GetUp()
        //    );
        //    break;
        //}
    }

    //glm::vec3 lookDir = g_camera->GetCameraOrientation() * g_camera->FORWARD_VECTOR;
    //glm::mat4 matView = glm::lookAt(
    //    g_camera->GetCameraPosition(),
    //    g_camera->GetCameraPosition() + lookDir,
    //    g_camera->UP_VECTOR
    //);

    GLint matProjection_UL = glGetUniformLocation(shaderProgramID, "matProjection");
    glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, glm::value_ptr(matProjection));

    GLint matView_UL = glGetUniformLocation(shaderProgramID, "matView");
    glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(matView));



    GLint screenWidthAndHeight_UL = glGetUniformLocation(shaderProgramID, "screenWidthAndHeight");
    glUniform2f(screenWidthAndHeight_UL,
        (GLfloat)width,
        (GLfloat)height);

    {
        GLint textureUnitNumber = 50;
        //GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);

        glBindTexture(GL_TEXTURE_2D, g_FBO_2->colourTexture_0_ID);
        GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "textureOffScreen_0");
        glUniform1i(texture_01_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 60;
        //GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);

        glBindTexture(GL_TEXTURE_2D, g_FBO_1->colourTexture_0_ID);
        GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "textureOffScreen_1");
        glUniform1i(texture_01_UL, textureUnitNumber);

    //return;

    }


    //g_currentScene->DisplaySceneSkyBox(shaderProgramID, deltaTime);
    DrawObjects(shaderProgramID, deltaTime);
    DrawLightDebugSpheres(matProjection, matView, shaderProgramID, deltaTime);

    g_currentScene->DisplaySceneSkyBox(shaderProgramID, deltaTime);
    //cMesh* tvBody = cGameManager::GetGMInstance()->FindMeshByFriendlyName("retrotv1");

    //DrawObject(tvBody, glm::mat4(1.0f), shaderProgramID);

    //cMesh* tvScreen = cGameManager::GetGMInstance()->FindMeshByFriendlyName("retrotv2");
    //tvScreen->isUsingVertexColors = false;
    //// Set the framebuffer object ( from  the 1st pass)
    //// to the texture on the TV screen
    //tvScreen->textureIsFromFBO = true;
    //tvScreen->FBOTextureNumber = g_FBO_1->colourTexture_0_ID;
    //tvScreen->textureRatios[0] = 1.0f;
    //tvScreen->bDoNotLight = true;
    //DrawObject(tvScreen, glm::mat4(1.0f), shaderProgramID);

    ////tvBody->drawPosition.x += 10.0f;
    ////DrawObject(tvBody, glm::mat4(1.0f), shaderProgramID);
    ////tvBody->drawPosition.x -= 10.0f;


    //tvScreen->textureIsFromFBO = true;
    //tvScreen->FBOTextureNumber = g_FBO_2->colourTexture_0_ID;
    //tvScreen->textureRatios[0] = 1.0f;
    //tvScreen->bDoNotLight = true;
    //tvScreen->drawPosition.x += 10.0f;
    //DrawObject(tvScreen, glm::mat4(1.0f), shaderProgramID);
    //tvScreen->drawPosition.x -= 10.0f;


    //cMesh* leftCornerScreen = cGameManager::GetGMInstance()->FindMeshByFriendlyName("leftconsolecornerscreen1");
    //if (leftCornerScreen)
    //{
    //    //leftCornerScreen->FBOTextureNumber = g_FBO_2->colourTexture_0_ID;
    //    leftCornerScreen->textureRatios[0] = 1.0f;
    //    leftCornerScreen->bDoNotLight = true;

    //    DrawObject(leftCornerScreen, glm::mat4(1.0f), shaderProgramID);
    //}

    //cMesh* rightCornerScreen = cGameManager::GetGMInstance()->FindMeshByFriendlyName("rightconsolecornerscreen1");
    //if (rightCornerScreen)
    //{
    //    //rightCornerScreen->FBOTextureNumber = g_FBO_2->colourTexture_0_ID;
    //    rightCornerScreen->textureRatios[0] = 1.0f;
    //    rightCornerScreen->bDoNotLight = true;

    //    DrawObject(rightCornerScreen, glm::mat4(1.0f), shaderProgramID);
    //}

    std::vector< cParticleSystem::sParticleRender> particleList;
    //g_DrawDebugSphere(g_anEmitter.GetPosition(), 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), deltaTime);

    g_anEmitter.GetParticleList(particleList);

    glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix
    //cMesh* fireMesh = g_anEmitter.CreateModelFromParticles();

    //if (fireMesh != nullptr)
    //{
    //    //fireMesh->drawPosition = g_anEmitter.GetPosition();
    //    fireMesh->textureName[0] = "Explosion_texture.bmp";
    //    fireMesh->textureRatios[0] = 1.0f;
    //    DrawObject(fireMesh, matModel, shaderProgramID, deltaTime);
    //}

    for (cParticleSystem::sParticleRender& particle : particleList)
    {
        //g_DrawDebugSphere(particle.position, 1.0f, glm::vec4(1.0f,0.0f,0.0f, 1.0f), deltaTime);
        g_particleMeshModel->drawPosition = particle.position;
        //g_particleMeshModel->setUniformDrawScale(1.0f);
        g_particleMeshModel->setDrawOrientation(particle.orientation);
        g_particleMeshModel->SetScale(particle.scaleXYZ);
        g_particleMeshModel->isFire = true;
        g_particleMeshModel->bDoNotLight = true;
        DrawObject(g_particleMeshModel, matModel, shaderProgramID, deltaTime);
    }

    // rain
    //cParticalSystem::Rain();

    std::vector< cParticleSystem::sParticleRender> rainParticleList;
    //g_DrawDebugSphere(g_anEmitter.GetPosition(), 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), deltaTime);

    //rainParticleSystem->GetParticleList(rainParticleList);

    //glm::mat4 rainMatModel = glm::mat4(1.0f);   // Identity matrix
    //cMesh* fireMesh = g_anEmitter.CreateModelFromParticles();

    //if (fireMesh != nullptr)
    //{
    //    //fireMesh->drawPosition = g_anEmitter.GetPosition();
    //    fireMesh->textureName[0] = "Explosion_texture.bmp";
    //    fireMesh->textureRatios[0] = 1.0f;
    //    DrawObject(fireMesh, matModel, shaderProgramID, deltaTime);
    //}

    //for (cParticleSystem::sParticleRender& rainParticle : rainParticleList)
    //{
    //    bool isAboveMesh = true;

    //    islandTerrain->IsAboveMesh(rainParticle.position, isAboveMesh);

    //    if (!isAboveMesh)
    //    {
    //        //g_DrawDebugSphere(rainParticle.position, 1.0f, glm::vec4(1.0f,0.0f,0.0f, 1.0f), deltaTime);
    //        g_rainSplashMesh->drawPosition = rainParticle.position;
    //        //g_particleMeshModel->setUniformDrawScale(1.0f);
    //        g_rainSplashMesh->setDrawOrientation(rainParticle.orientation);
    //        g_rainSplashMesh->SetScale(rainParticle.scaleXYZ);

    //        //g_rainMeshModel->drawScale.x /= 10.0f;
    //        //g_rainMeshModel->drawScale.y *= 2.0f;

    //        g_rainSplashMesh->isRain = true;
    //        DrawObject(g_rainSplashMesh, rainMatModel, shaderProgramID, deltaTime);

    //        //g_DrawDebugSphere(rainParticle.position, 10.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), deltaTime);
    //    }
    //    else
    //    {
    //        //g_DrawDebugSphere(rainParticle.position, 1.0f, glm::vec4(1.0f,0.0f,0.0f, 1.0f), deltaTime);
    //        g_rainMeshModel->drawPosition = rainParticle.position;
    //        //g_particleMeshModel->setUniformDrawScale(1.0f);
    //        g_rainMeshModel->setDrawOrientation(rainParticle.orientation);
    //        g_rainMeshModel->SetScale(rainParticle.scaleXYZ);

    //        g_rainMeshModel->drawScale.x /= 10.0f;
    //        g_rainMeshModel->drawScale.y *= 2.0f;

    //        g_rainMeshModel->isRain = true;
    //        DrawObject(g_rainMeshModel, rainMatModel, shaderProgramID, deltaTime);
    //    }


    //}

    //g_softBody->Update(deltaTime, shaderProgramID);

    //for (unsigned int i = 0; i < chainSoftBodies.size(); i++)
    //{
    //    chainSoftBodies[i]->Update(deltaTime, shaderProgramID);
    //}

    //float highestChain1 = chainSoftBodies[0]->GetHighestPointWithBoundsZ(6.0f, 10.0f);
    //float highestChain2 = chainSoftBodies[1]->GetHighestPointWithBoundsZ(6.0f, 10.0f);

    ////std::cout << "highestChain1" << highestChain1 << std::endl;
    ////std::cout << "highestChain2" << highestChain2 << std::endl;

    //bridgeMesh->drawPosition.y = (highestChain1 + highestChain2) / 2.0f;


    //g_softBody->VerletUpdate(deltaTime);
    //g_softBody->ApplyCollision(deltaTime);
    //g_softBody->SatisfyConstraints();

    //for (cSoftBodyVerlet::sParticle* particle : g_softBody->vec_pParticles)
    //{
    //    //glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

    //    g_DrawDebugSphere(particle->position, 0.10f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), deltaTime);
    //    //g_particleMeshModel->drawPosition = particle.position;
    //    //g_particleMeshModel->setUniformDrawScale(1.0f);
    //    //g_particleMeshModel->setDrawOrientation(particle.orientation);
    //    //DrawObject(g_particleMeshModel, matModel, shaderProgramID);
    //    //g_softBody.CreateSoftBody(particle);

    //}
}

void Engine::DrawPass3(GLFWwindow* window, double deltaTime)
{
  
}

void Engine::SetBoolInFragmentShader(std::string boolString, bool trueOrFalse)
{
    if (trueOrFalse)
    {
        GLint boolStringID = glGetUniformLocation(shaderProgramID, boolString.c_str());
        glUniform1f(boolStringID, (GLfloat)GL_TRUE);
    }
    else
    {
        GLint boolStringID = glGetUniformLocation(shaderProgramID, boolString.c_str());
        glUniform1f(boolStringID, (GLfloat)GL_FALSE);
    }
}

void Engine::SetUniformFloatInShader(std::string floatName, float value)
{
    GLint floatStringID = glGetUniformLocation(shaderProgramID, floatName.c_str());
    glUniform1f(floatStringID, (GLfloat)value);
}

void Engine::Update(double deltaTime, GLFWwindow& window, GLuint shaderProgramID)
{
    // Switch the "main" shader
    //        shaderProgramID = pShaderThing->getIDFromFriendlyName("shader01");
    //        glUseProgram(shaderProgramID);

    ::g_inputHandler->HandleInput();
    ::g_inputHandler->AsyncMouseHandlingUpdate(&window);
    ::g_pFlyCamera->Update(deltaTime);

    g_anEmitter.Update(deltaTime);
    //rainParticleSystem->Update(deltaTime);

    g_currentScene->Update(&window, deltaTime);

    g_uiManager->Update(window);

    //float ratio;
    //int width, height;

    //glUseProgram(shaderProgramID);

    //glfwGetFramebufferSize(window, &width, &height);
    //ratio = width / (float)height;

    //glViewport(0, 0, width, height);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //// While drawing a pixel, see if the pixel that's already there is closer or not?
    //glEnable(GL_DEPTH_TEST);
    //// (Usually) the default - does NOT draw "back facing" triangles
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    ////DrawPass1(window, deltaTime);
    ////DrawPass2(window, deltaTime);

    //// *****************************************************************
    //::g_pTheLights->UpdateUniformValues(shaderProgramID);
    //// *****************************************************************

    ////uniform vec4 eyeLocation;
    //GLint eyeLocation_UL = glGetUniformLocation(shaderProgramID, "eyeLocation");

    //glm::vec3 cameraEye = g_camera->GetPosition();

    //glUniform4f(eyeLocation_UL,
    //    cameraEye.x, cameraEye.y, cameraEye.z, 1.0f);


    ////mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    ////glm::mat4 matProjection = glm::perspective(0.6f,
    ////                                           ratio,
    ////                                           0.1f, // near clip
    ////                                           10'000.0f); // far clip

    //glm::mat4 matProjection;
    //glm::mat4 matView;

    //if (::bUseFlyCamera)
    //{
    //    matProjection = glm::perspective(0.6f,
    //        ratio,
    //        g_pFlyCamera->GetNearClip(), // near clip
    //        g_pFlyCamera->GetFarClip()); // far clip

    //    //glm::mat4 matView = glm::lookAt(::g_cameraEye,
    //    //                                ::g_cameraTarget,
    //    //                                ::g_upVector);



    //    ////        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
    //    //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    //    //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    //    //        mat4x4_mul(mvp, p, m);

    //    matView = glm::lookAt(::g_pFlyCamera->getEye(),
    //        ::g_pFlyCamera->getAt(),
    //        ::g_pFlyCamera->getUp());
    //}
    //else
    //{
    //    matProjection = glm::perspective(0.6f,
    //        ratio,
    //        g_camera->GetNearClip(), // near clip
    //        g_camera->GetFarClip()); // far clip

    //    //glm::mat4 matView = glm::lookAt(::g_cameraEye,
    //    //                                ::g_cameraTarget,
    //    //                                ::g_upVector);


    //    switch (::g_inputHandler->currentMode)
    //    {
    //    case cInputHandler::EditorMode:

    //        matView = glm::lookAt(g_camera->GetPosition(),
    //            g_camera->GetTarget(),
    //            g_camera->GetUp());
    //        break;

    //    case cInputHandler::GameMode:
    //        glm::vec3 lookDir = g_camera->GetOrientation() * g_camera->GetForward();
    //        matView = glm::lookAt(
    //            g_camera->GetPosition(),
    //            g_camera->GetPosition() + lookDir,
    //            g_camera->GetUp()
    //        );
    //        break;
    //    }
    //}

    ////glm::vec3 lookDir = g_camera->GetCameraOrientation() * g_camera->FORWARD_VECTOR;
    ////glm::mat4 matView = glm::lookAt(
    ////    g_camera->GetCameraPosition(),
    ////    g_camera->GetCameraPosition() + lookDir,
    ////    g_camera->UP_VECTOR
    ////);

    //GLint matProjection_UL = glGetUniformLocation(shaderProgramID, "matProjection");
    //glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, glm::value_ptr(matProjection));

    //GLint matView_UL = glGetUniformLocation(shaderProgramID, "matView");
    //glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(matView));

    //DrawLightDebugSpheres(matProjection, matView, shaderProgramID);

    

}

void Engine::End(GLFWwindow& window)
{
    g_uiManager->End(&window);

    // Delete everything
    delete g_FBO_1;
    delete g_FBO_2;

    glfwDestroyWindow(&window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Engine::EndOfFrame(double deltaTime, GLFWwindow& window)
{

    std::string windowTitle = windowText;
    glfwPollEvents();
    glfwSwapBuffers(&window);
    glfwSetWindowTitle(&window, windowTitle.c_str());

    bool hasDeletedAll = false;

    while (!hasDeletedAll)
    {
        int indexToDelete = 0;
        hasDeletedAll = true;
        for (unsigned int i = 0; i < g_currentScene->m_gameObjects.size(); i++)
        {
            if (g_currentScene->m_gameObjects[i]->IsToDelete())
            {
                hasDeletedAll = false;
                indexToDelete = i;
                g_currentScene->m_gameObjects.erase(g_currentScene->m_gameObjects.begin() + indexToDelete);
                break;
            }
        }
    }
}

void Engine::LoadAnimations(std::vector<Bones::CharacterAnimation*>& animations)
{
    const char* folderRelativePath = "assets/animations";
    ::g_pMeshManager->setBasePath(folderRelativePath);

    std::vector< std::string> animationFiles;
    WindowsUtils::GetAllFilesFromFolder(folderRelativePath, animationFiles);

    if (animationFiles.size() == 0)
    {
        return;
    }

    for (unsigned int i = 0; i < animationFiles.size(); i++)
    {
        sModelDrawInfo model;
        if (!::g_pMeshManager->LoadAnimationWithAssimp(animationFiles[i], model))
        {
            std::cout << "Model: " << animationFiles[i] << " failed to load" << std::endl;
        }
    }

    for (unsigned int u = 0; u < g_animationSystem->characterAnimations.size(); u++)
    {
        Bones::CharacterAnimation* boneAnimation = g_animationSystem->characterAnimations[u];
        AnimationGroup* animationGroup = new AnimationGroup();

        animationGroup->name = boneAnimation->Name;

        for (unsigned int i = 0; i < boneAnimation->Channels.size(); i++)
        {
            AnimationData* animation = new AnimationData();
            animation->name = boneAnimation->Channels[i]->Name;
            animation->duration = boneAnimation->Duration;
            animation->timer = 0.0;

            // Positions
            for (unsigned int p = 0; p < boneAnimation->Channels[i]->m_PositionKeyFrames.size(); p++)
            {
                animation->m_positionKeyFrames.push_back(boneAnimation->Channels[i]->m_PositionKeyFrames[p]);
            }

            // Scales
            for (unsigned int s = 0; s < boneAnimation->Channels[i]->m_ScaleKeyFrames.size(); s++)
            {
                animation->m_scaleKeyFrames.push_back(boneAnimation->Channels[i]->m_ScaleKeyFrames[s]);
            }

            // Rotation
            for (unsigned int r = 0; r < boneAnimation->Channels[i]->m_RotationKeyFrames.size(); r++)
            {
                animation->m_rotationKeyFrames.push_back(boneAnimation->Channels[i]->m_RotationKeyFrames[r]);
            }

            animationGroup->animations.push_back(animation);
        }

        g_animationSystem->animationGroups.push_back(animationGroup);
    }

    g_animationSystem->currentAnimationGroup = g_animationSystem->animationGroups[0];
    //g_animationSystem->currentAnimation = g_animationSystem->characterAnimations[0];

    g_animationSystem->SetAnimationByName("Captain_Running.dae");
}

void Engine::LoadTexturesFromFile(std::vector<std::string>& textures)
{
    ::g_pTextureManager = new cBasicTextureManager();
    ::g_pTextureManager->SetBasePath("assets/textures");

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        if (!::g_pTextureManager->Create2DTextureFromBMPFile(textures[i], true))
        {
            std::cout << textures[i] << " failed to load!" << std::endl;
        }
    }

    // Load a cube map
    ::g_pTextureManager->SetBasePath("assets/textures/CubeMaps");
    std::string errors;

    ::g_pTextureManager->CreateCubeTextureFromBMPFiles("SnowyDay",
        "negx.bmp",
        "posx.bmp",
        "posy.bmp",
        "negy.bmp",
        "posz.bmp",
        "negz.bmp",
        true,
        errors);

    ::g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
        "SpaceBox_right1_posX.bmp",
        "SpaceBox_left2_negX.bmp",
        "SpaceBox_top3_posY.bmp",
        "SpaceBox_bottom4_negY.bmp",
        "SpaceBox_front5_posZ.bmp",
        "SpaceBox_back6_negZ.bmp",
        true,
        errors);

    ::g_pTextureManager->CreateCubeTextureFromBMPFiles("sunnyday",
        "TropicalSunnyDayLeft2048.bmp",
        "TropicalSunnyDayRight2048.bmp",
        "TropicalSunnyDayUp2048.bmp",
        "TropicalSunnyDayDown2048.bmp",
        "TropicalSunnyDayFront2048.bmp",
        "TropicalSunnyDayBack2048.bmp",
        true,
        errors);

    ::g_pTextureManager->SetBasePath("assets/textures/CubeMaps/night_sky");

    ::g_pTextureManager->CreateCubeTextureFromBMPFiles("nightsky",
        "posx.bmp",
        "negx.bmp",
        "negy.bmp",
        "posy.bmp",
        "negz.bmp",
        "posz.bmp",
        true,
        errors);

    ::g_pTextureManager->SetBasePath("assets/textures/CubeMaps/day_sky");

    ::g_pTextureManager->CreateCubeTextureFromBMPFiles("skyday",
        "skydayposx.bmp",
        "skydaynegx.bmp",
        "skydaynegy.bmp",
        "skydayposy.bmp",
        "skydaynegz.bmp",
        "skydayposz.bmp",
        true,
        errors);


    ::g_pTextureManager->SetBasePath("assets/skyboxes/MegaSun");

    ::g_pTextureManager->CreateCubeTextureFromBMPFiles("megasun",
        "MegaSunRight.bmp",
        "MegaSunLeft.bmp",
        "MegaSunTop.bmp",
        "MegaSunBottom.bmp",
        "MegaSunFront.bmp",
        "MegaSunBack.bmp",
        true,
        errors);

    //std::cout << errors << std::endl;
}

void Engine::LoadLights(std::vector<sLightsInfo>& sceneLights, GLuint shaderProgramID)
{
    for (unsigned int i = 0; i < sceneLights.size(); i++)
    {
        g_currentScene->m_sceneLights->theLights[i].atten = sceneLights[i].Atten;
        g_currentScene->m_sceneLights->theLights[i].diffuse = sceneLights[i].Diffuse;
        g_currentScene->m_sceneLights->theLights[i].direction = sceneLights[i].Direction;
        g_currentScene->m_sceneLights->theLights[i].lightFriendlyName = sceneLights[i].FriendlyName;
        g_currentScene->m_sceneLights->theLights[i].param1 = sceneLights[i].Param1;
        g_currentScene->m_sceneLights->theLights[i].param2 = sceneLights[i].Param2;
        g_currentScene->m_sceneLights->theLights[i].position = sceneLights[i].Position;
        g_currentScene->m_sceneLights->theLights[i].specular = sceneLights[i].Specular;
    }

    //::g_pTheLights = new cLightManager();
    //::g_pTheLights->SetUniformLocations(shaderProgramID);

    //// Light #0 - directional light 
    //::g_pTheLights->theLights[0].TurnOn();//.TurnOff();
    //::g_pTheLights->theLights[0].param1.x = 2.0f;   // Directional
    //::g_pTheLights->theLights[0].direction = glm::normalize(glm::vec4(-0.5f, -1.0f, 0.0f, 1.0f));
    //::g_pTheLights->theLights[0].diffuse = cGameManager::NormalizeRGBA(glm::vec4(101.0f, 152.0f, 205.0f, 255.0f));
    //::g_pTheLights->theLights[0].specular = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    //// post lights - rgba(251.0f,251.0f,253.0f,255.0f)
    //::g_pTheLights->theLights[1].TurnOn();   // Turn on
    //::g_pTheLights->theLights[1].param1.x = 0.0f;   // 1 = spot light
    //::g_pTheLights->theLights[1].lightFriendlyName = "postlight";
    //::g_pTheLights->theLights[1].direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    //::g_pTheLights->theLights[1].diffuse = cGameManager::NormalizeRGBA(glm::vec4(251.0f, 251.0f, 253.0f, 255.0f)); // glm::vec4(0.9f, 0.5f, 0.45f, 0.01f);
    //::g_pTheLights->theLights[1].position = glm::vec4(0.0f, 53.0f, 0.0f, 0.0f);// -46.0f;
    //::g_pTheLights->theLights[1].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);// -46.0f;
    //::g_pTheLights->theLights[1].atten.x = 0.0f;        // Constant attenuation
    //::g_pTheLights->theLights[1].atten.y = 0.02f;        // Linear attenuation
    //::g_pTheLights->theLights[1].atten.z = 0.01f;        // Quadratic attenuation
    //::g_pTheLights->theLights[1].param1.y = 30.0f;
    //::g_pTheLights->theLights[1].param1.z = 30.0f;

    ////rgba(247.0f, 245.0f, 163.0f, 255.0f)
    //::g_pTheLights->theLights[1].diffuse = cGameManager::NormalizeRGBA(glm::vec4(247.0f, 245.0f, 163.0f, 255.0f)); // glm::vec4(0.9f, 0.5f, 0.45f, 0.01f);
    //::g_pTheLights->theLights[1].specular = cGameManager::NormalizeRGBA(glm::vec4(247.0f, 245.0f, 163.0f, 255.0f));

    // spot lights
    //::g_pTheLights->theLights[0].param1.x = 2.0f;   // Directional
    //::g_pTheLights->theLights[0].lightFriendlyName = "moonlight";
    //::g_pTheLights->theLights[0].direction = glm::normalize(glm::vec4(1.0f, -0.5f, 0.0f, 1.0f));
    //::g_pTheLights->theLights[0].specular = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);// -46.0f;
    //::g_pTheLights->theLights[0].
    //// rgba(101, 152, 205, 255) - moonlight in pureref file (solution folder)
    //::g_pTheLights->theLights[0].diffuse = cGameManager::NormalizeRGBA(glm::vec4(101.0f, 152.0f, 205.0f, 255.0f));
    ////::g_pTheLights->theLights[0].specular = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f);
    //::g_pTheLights->theLights[0].atten.x = 0.1f;        // Constant attenuation
    //::g_pTheLights->theLights[0].atten.y = 0.02f;        // Linear attenuation
    //::g_pTheLights->theLights[0].atten.z = 0.001f;        // Quadratic attenuation
    ////::g_pTheLights->theLights[0].param1.y = 180.0f;

    //::g_pTheLights->theLights[0].param1.z = 180.0f;

    // Light #1 - spot light
    //::g_pTheLights->theLights[1].param2.x = 1.0f;   // Turn on
    //::g_pTheLights->theLights[1].param1.x = 1.0f;   // 0 = point  / 1 = spot  / 2 = direction 
    //::g_pTheLights->theLights[1].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f); //With spots, set direction(relative)
    //::g_pTheLights->theLights[1].diffuse = cGameManager::NormalizeRGBA(glm::vec4(101.0f, 152.0f, 205.0f, 255.0f));
    ////	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
    //::g_pTheLights->theLights[1].param1.y = 55.0f; // x = lightType, y = inner angle, z = outer angle, w = TBD
    //::g_pTheLights->theLights[1].param1.z = 55.0f;

    //::g_pTheLights->theLights[1].position = glm::vec4(0.0f, 53.0f, 0.0f, 0.0f);// -46.0f;

    //// How "bright" the lights is, really the opposite of brightness.
    //// how dim do you want this
    ////::g_pTheLights->theLights[0].atten.x = 0.0f;         // Constant attenuation
    ////::g_pTheLights->theLights[0].atten.y = 0.01f;        // Linear attenuation
    ////::g_pTheLights->theLights[0].atten.z = 0.000000001f; // Quadratic attenuation

    //::g_pTheLights->theLights[0].param2.x = 1.0f;   // Turn on
    ////    ::g_pTheLights->theLights[0].param1.x = 0.0f;   // 0 = point light
    //::g_pTheLights->theLights[0].param1.x = 1.0f;   // 1 = spot light
    //// With spots, set direction (relative)
    //::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ////	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
    ////::g_pTheLights->theLights[0].param1.y = 15.0f;
    ////::g_pTheLights->theLights[0].param1.z = 25.0f;

    ////    ::g_pTheLights->theLights[0].param1.y = 30.0f;
    ////::g_pTheLights->theLights[0].param1.z = 25.0f;


    //::g_pTheLights->theLights[0].position.x = -46.0f;
    //::g_pTheLights->theLights[0].position.y = 23.0f;
    //::g_pTheLights->theLights[0].position.z = -26.0f;

    //// How "bright" the lights is
    //// Really the opposite of brightness.
    ////  how dim do you want this
    //::g_pTheLights->theLights[0].atten.x = 0.0f;        // Constant attenuation
    //::g_pTheLights->theLights[0].atten.y = 0.01f;        // Linear attenuation
    //::g_pTheLights->theLights[0].atten.z = 0.000000001f;        // Quadratic attenuation

    //// Light #1 is a directional light 
    //::g_pTheLights->theLights[1].param2.x = 1.0f;   // Turn on
    //::g_pTheLights->theLights[1].param1.x = 2.0f;   // Directional

    //// Direction with respect of the light.
    //::g_pTheLights->theLights[1].direction = glm::normalize(glm::vec4(-0.5f, -1.0f, 0.0f, 1.0f));
    ////    float lightBrightness = 0.7f;
    //::g_pTheLights->theLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //::g_pTheLights->theLights[1].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

bool Engine::LoadModels(std::vector<sModel> sceneModelsInfo)
{
    for (unsigned int i = 0; i < sceneModelsInfo.size(); i++)
    {
        cMesh* mesh = new cMesh();
        mesh->drawPosition = sceneModelsInfo[i].Position;
        mesh->drawScale = sceneModelsInfo[i].DrawScale;
        mesh->setDrawOrientation(sceneModelsInfo[i].Orientation);
        mesh->bUseDebugColours = sceneModelsInfo[i].IsUsingDebugColors;
        mesh->isUsingVertexColors = sceneModelsInfo[i].IsUsingVertexColors;
        mesh->UseBonesUL = sceneModelsInfo[i].IsUsingBones;
        mesh->friendlyName = sceneModelsInfo[i].FriendlyName;
        mesh->objectType = sceneModelsInfo[i].ObjectType;
        mesh->transparencyAlpha = sceneModelsInfo[i].Alpha;
        mesh->wholeObjectDebugColourRGBA = sceneModelsInfo[i].DebugColourRGBA;

        mesh->meshName = sceneModelsInfo[i].MeshName;

        for (unsigned int j = 0; j < sceneModelsInfo[i].ObjectTextures.size(); j++)
        {
            mesh->textureName[j] = sceneModelsInfo[i].ObjectTextures[j].Name;
            mesh->textureRatios[j] = sceneModelsInfo[i].ObjectTextures[j].Ratio;
        }

        g_currentScene->AddMesh(mesh);
    }

    ::g_pDebugSphereMesh = new cMesh();
    ::g_pDebugSphereMesh->meshName = "shapes/Sphere_1_unit_Radius_xyz_n_rgba_uv.ply";
    ::g_pDebugSphereMesh->bIsWireframe = true;
    ::g_pDebugSphereMesh->bDoNotLight = true;
    ::g_pDebugSphereMesh->setUniformDrawScale(1.0f);
    ::g_pDebugSphereMesh->bIsVisible = false;
    ::g_pDebugSphereMesh->friendlyName = "DEBUG_SPHERE";

    return true;
}

bool Engine::LoadPhysics(void)
{
    if (!g_currentScene->m_scenePhysics)
    {
        g_currentScene->m_scenePhysics = new cPhysics();
        g_currentScene->m_scenePhysics->setVAOManager(::g_pMeshManager);
    }

    return true;
}

double leafWave = 0.0;

void Engine::DrawObject(cMesh* pCurrentMesh, glm::mat4 matModelParent, GLuint shaderProgramID, double deltaTime)
{
    pCurrentMesh->drawTime -= deltaTime;

    if (inGame)
    {
        glm::vec3 cameraPosition = g_camera->getAt();
        SetUniformVec4InShader("cameraPosition", glm::vec4(cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.0f));
    }

    if (pCurrentMesh->isDrawTimed && (pCurrentMesh->drawTime <= 0.0f))
    {
        return;
    }

    // mat4x4_identity(m);
    glm::mat4 matModel = matModelParent;

    // Translation
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        glm::vec3(pCurrentMesh->drawPosition.x,
            pCurrentMesh->drawPosition.y,
            pCurrentMesh->drawPosition.z));

    // Rotation matrix generation
    //    glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
    //                                       pCurrentMesh->drawOrientation.x, // (float)glfwGetTime(),
    //                                       glm::vec3(1.0f, 0.0, 0.0f));
    //
    //
    //    glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
    //                                       pCurrentMesh->drawOrientation.y, // (float)glfwGetTime(),
    //                                       glm::vec3(0.0f, 1.0, 0.0f));
    //
    //    glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
    //                                       pCurrentMesh->drawOrientation.z, // (float)glfwGetTime(),
    //                                       glm::vec3(0.0f, 0.0, 1.0f));

    // Now we are all bougie, using quaternions
    glm::mat4 matRotation = glm::mat4(pCurrentMesh->GetOrientation());

    //glm::mat4 matRotation = pCurrentMesh->GetRotation();



    // Scaling matrix
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        glm::vec3(pCurrentMesh->drawScale.x,
            pCurrentMesh->drawScale.y,
            pCurrentMesh->drawScale.z));
    //--------------------------------------------------------------

    // Combine all these transformation
    matModel = matModel * matTranslate;         // Done last

    //    matModel = matModel * matRotateX;
    //    matModel = matModel * matRotateY;
    //    matModel = matModel * matRotateZ;
        //
    matModel = matModel * matRotation;


    matModel = matModel * matScale;             // Mathematically done 1st

    //        m = m * rotateZ;
    //        m = m * rotateY;
    //        m = m * rotateZ;



       //mat4x4_mul(mvp, p, m);
    //    glm::mat4 mvp = matProjection * matView * matModel;

    //    GLint mvp_location = glGetUniformLocation(shaderProgramID, "MVP");
    //    //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    //    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

    GLint matModel_UL = glGetUniformLocation(shaderProgramID, "matModel");
    glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(matModel));


    // Also calculate and pass the "inverse transpose" for the model matrix
    glm::mat4 matModel_InverseTranspose = glm::inverse(glm::transpose(matModel));

    // uniform mat4 matModel_IT;
    GLint matModel_IT_UL = glGetUniformLocation(shaderProgramID, "matModel_IT");
    glUniformMatrix4fv(matModel_IT_UL, 1, GL_FALSE, glm::value_ptr(matModel_InverseTranspose));


    if (pCurrentMesh->bIsWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //        glPointSize(10.0f);


    // uniform bool bDoNotLight;
    GLint bDoNotLight_UL = glGetUniformLocation(shaderProgramID, "bDoNotLight");

    if (pCurrentMesh->bDoNotLight)
    {
        // Set uniform to true
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_TRUE);
    }
    else
    {
        // Set uniform to false;
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_FALSE);
    }

    //uniform bool bUseDebugColour;	
    GLint bUseDebugColour_UL = glGetUniformLocation(shaderProgramID, "bUseDebugColour");
    if (pCurrentMesh->bUseDebugColours)
    {
        glUniform1f(bUseDebugColour_UL, (GLfloat)GL_TRUE);
        //uniform vec4 debugColourRGBA;
        GLint debugColourRGBA_UL = glGetUniformLocation(shaderProgramID, "debugColourRGBA");
        glUniform4f(debugColourRGBA_UL,
            pCurrentMesh->wholeObjectDebugColourRGBA.r,
            pCurrentMesh->wholeObjectDebugColourRGBA.g,
            pCurrentMesh->wholeObjectDebugColourRGBA.b,
            pCurrentMesh->wholeObjectDebugColourRGBA.a);
    }
    else
    {
        glUniform1f(bUseDebugColour_UL, (GLfloat)GL_FALSE);
    }



    // FOR NOW, hardcode the texture settings

    // uniform bool bUseVertexColours;
    GLint bUseVertexColours_UL = glGetUniformLocation(shaderProgramID, "bUseVertexColours");

    bool isUsingVertexColors = pCurrentMesh->isUsingVertexColors;

    glUniform1f(bUseVertexColours_UL, (GLfloat)isUsingVertexColors);


    SetUpTextures(pCurrentMesh, shaderProgramID);

    // *********************************************************************
        // Is this using the heigth map?
        // HACK:
    GLint bUseHeightMap_UL = glGetUniformLocation(shaderProgramID, "bUseHeightMap");
    // uniform bool bUseHeightMap;
    if (pCurrentMesh->friendlyName == "Ground")
    {
        glUniform1f(bUseHeightMap_UL, (GLfloat)GL_TRUE);

        //uniform float heightScale;
        GLint heightScale_UL = glGetUniformLocation(shaderProgramID, "heightScale");
        glUniform1f(heightScale_UL, g_HeightAdjust);

        //uniform vec2 UVOffset;
        GLint UVOffset_UL = glGetUniformLocation(shaderProgramID, "UVOffset");
        glUniform2f(UVOffset_UL, g_UVOffset.x, g_UVOffset.y);


    }
    else
    {
        glUniform1f(bUseHeightMap_UL, (GLfloat)GL_FALSE);
    }
    // *********************************************************************


    // *********************************************************************
        //  Discard texture example
        //    uniform bool bUseDiscardMaskTexture;
        //    uniform sampler2D maskSamplerTexture01;
    {
        GLint bUseDiscardMaskTexture_UL = glGetUniformLocation(shaderProgramID, "bUseDiscardMaskTexture");

        // uniform bool bUseHeightMap;
        if (pCurrentMesh->friendlyName == "Ground")
        {
            glUniform1f(bUseDiscardMaskTexture_UL, (GLfloat)GL_TRUE);

            //uniform sampler2D maskSamplerTexture01; 	// Texture unit 25
            GLint textureUnitNumber = 25;
            GLuint stencilMaskID = ::g_pTextureManager->getTextureIDFromName("FAKE_Stencil_Texture_612x612.bmp");
            glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
            glBindTexture(GL_TEXTURE_2D, stencilMaskID);

            GLint bUseDiscardMaskTexture_UL = glGetUniformLocation(shaderProgramID, "maskSamplerTexture01");
            glUniform1i(bUseDiscardMaskTexture_UL, textureUnitNumber);

        }
        else
        {
            glUniform1f(bUseDiscardMaskTexture_UL, (GLfloat)GL_FALSE);
        }
    }

    if (pCurrentMesh->transparencyAlpha < 1.0f)
    {
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    //if (pCurrentMesh->isDrawBehind)
    //{
    //    glDepthRange(1, 1);
    //}
    //else
    //{
    //    glDepthRange(0, 1);
    //}

    if (pCurrentMesh->isFoliage)
    {
        SetBoolInFragmentShader("isFoliage", true);
        leafWave += deltaTime;

        SetUniformFloatInShader("foliageWave", (float)sin(leafWave) * 50.0f);

        SetUniformFloatInShader("foliageHeight",300.0f);
    }
    else
    {
        SetBoolInFragmentShader("isFoliage", false);
    }

    if (pCurrentMesh->isFire)
    {
        SetBoolInFragmentShader("isFire", true);
    }
    else
    {
        SetBoolInFragmentShader("isFire", false);
    }

    if (pCurrentMesh->isRain)
    {
        SetBoolInFragmentShader("isRain", true);
    }
    else
    {
        SetBoolInFragmentShader("isRain", false);
    }


    GLint transparencyAlpha = glGetUniformLocation(shaderProgramID, "transparencyAlpha");
    glUniform1f(transparencyAlpha, pCurrentMesh->transparencyAlpha);
    // *********************************************************************

    //GLint isFire = glGetUniformLocation(shaderProgramID, "isFire");
    //glUniform1f(isFire, (GLfloat)pCurrentMesh->isFire);

    //if (pCurrentMesh->isFire)
    //{
    //    glm::vec3 lowerVector = glm::vec3(-0.5f, 0.0f, -0.5f);
    //    glm::vec3 upperVector = glm::vec3(0.5f, 1.0f, 0.5f);

    //    if (pCurrentMesh->friendlyName == "fireimpostor1")
    //    {
    //        fireImpostor1Timer += (deltaTime / 3.0);

    //        std::cout << "fireImpostor1Timer: " << fireImpostor1Timer << std::endl;

    //        if (fireImpostor1Timer > fireImpostor1TimerMax)
    //        {
    //            GLint randomPos = glGetUniformLocation(shaderProgramID, "randomPos");

    //            glm::vec3 randomPosVec = MathUtils::GetRandomVec3(lowerVector, upperVector);
    //            glUniform4f(randomPos, randomPosVec.x, 0.0f, randomPosVec.z, 1.0f);

    //            pCurrentMesh->RotateAroundY(glm::degrees(randomPosVec.y));

    //            fireImpostor1Timer = 0.0f;
    //        }
    //    }

    //    if (pCurrentMesh->friendlyName == "fireimpostor2")
    //    {
    //        fireImpostor2Timer += (deltaTime / 3.0f);

    //        std::cout << "fireImpostor2Timer: " << fireImpostor2Timer << std::endl;

    //        if (fireImpostor2Timer > fireImpostor2TimerMax)
    //        {
    //            GLint randomPos = glGetUniformLocation(shaderProgramID, "randomPos");

    //            glm::vec3 randomPosVec = MathUtils::GetRandomVec3(lowerVector, upperVector);
    //            glUniform4f(randomPos, randomPosVec.x, 0.0f, randomPosVec.z, 1.0f);

    //            pCurrentMesh->RotateAroundY(glm::degrees(randomPosVec.y));

    //            fireImpostor2Timer = 0.0f;
    //        }
    //    }
    //}


    GLint isBeholder = glGetUniformLocation(shaderProgramID, "isBeholder");
    glUniform1f(isBeholder, (GLfloat)(pCurrentMesh->friendlyName == "beholder"));

    if (pCurrentMesh->UseBonesUL)
    {
        SetBoolInFragmentShader("UseBones", true);
        sModelDrawInfo modelWithBoneInfo;
        ::g_pMeshManager->FindDrawInfoByModelName(pCurrentMesh->meshName, modelWithBoneInfo);

        for (int i = 0; i < pCurrentMesh->BoneInfoVec.size(); ++i)
        {
            glUniformMatrix4fv(::g_pMeshManager->BoneMatricesUL[i], 1, GL_FALSE, glm::value_ptr(pCurrentMesh->BoneInfoVec[i].FinalTransformation));
        }

        glBindVertexArray(modelWithBoneInfo.VAO_ID);
        glDrawElements(GL_TRIANGLES, modelWithBoneInfo.numberOfIndices, GL_UNSIGNED_INT, (GLvoid*)0);

        SetBoolInFragmentShader("UseBones", false);
    }
    else
    {
        if (pCurrentMesh->hasBackupModel)
        {
            float distanceToCamera = glm::distance(::g_pFlyCamera->GetPosition(), pCurrentMesh->drawPosition);
            //std::cout << "distanceToCamera: " << distanceToCamera << std::endl;

            if (distanceToCamera > 10000.0f)
            {
                sModelDrawInfo modelInfo;
                if (::g_pMeshManager->FindDrawInfoByModelName(pCurrentMesh->backupModelName, modelInfo))
                {
                    // Found it!!!

                    glBindVertexArray(modelInfo.VAO_ID); 		//  enable VAO (and everything else)
                    glDrawElements(GL_TRIANGLES,
                        modelInfo.numberOfIndices,
                        GL_UNSIGNED_INT,
                        0);
                    glBindVertexArray(0); 			            // disable VAO (and everything else)

                }
            }
            else
            {
                sModelDrawInfo modelInfo;
                if (::g_pMeshManager->FindDrawInfoByModelName(pCurrentMesh->meshName, modelInfo))
                {
                    // Found it!!!

                    glBindVertexArray(modelInfo.VAO_ID); 		//  enable VAO (and everything else)
                    glDrawElements(GL_TRIANGLES,
                        modelInfo.numberOfIndices,
                        GL_UNSIGNED_INT,
                        0);
                    glBindVertexArray(0); 			            // disable VAO (and everything else)

                }
            }
        }
        else
        {

            sModelDrawInfo modelInfo;
            if (::g_pMeshManager->FindDrawInfoByModelName(pCurrentMesh->meshName, modelInfo))
            {
                // Found it!!!

                glBindVertexArray(modelInfo.VAO_ID); 		//  enable VAO (and everything else)
                glDrawElements(GL_TRIANGLES,
                    modelInfo.numberOfIndices,
                    GL_UNSIGNED_INT,
                    0);
                glBindVertexArray(0); 			            // disable VAO (and everything else)

            }
        }


    }

    glm::mat4 matRemoveScaling = glm::scale(glm::mat4(1.0f),
        glm::vec3(
            1.0f / pCurrentMesh->drawScale.x,
            1.0f / pCurrentMesh->drawScale.y,
            1.0f / pCurrentMesh->drawScale.z));

    matModel = matModel * matRemoveScaling;

    for (cMesh* pChild : pCurrentMesh->vec_pChildMeshes)
    {

        // Notice we are passing the "parent" (already transformed) matrix
        // NOT an identiy matrix

        // if you are using scaling, you can "undo" the scaling
        // i.e. add the opposite of the scale the parent had


        DrawObject(pChild, matModel, shaderProgramID, deltaTime);

    }//for ( cMesh* pChild 

    GLint isRightMap = glGetUniformLocation(shaderProgramID, "isRightMap");
    glUniform1f(isRightMap, (GLfloat)false);

    return;
}

void Engine::DrawObjects(GLuint shaderProgramID, double deltaTime)
{
    std::vector<cMesh*> meshesToDraw = g_currentScene->GetAllMeshes();
    for (unsigned int index = 0; index != meshesToDraw.size(); index++)
    {
        cMesh* pCurrentMesh = meshesToDraw[index];

        if (pCurrentMesh->bIsVisible)
        {
            glm::mat4 matModel = glm::mat4(1.0f);   // Identity matrix

            DrawObject(pCurrentMesh, matModel, shaderProgramID, deltaTime);
        }
    }
}

void Engine::SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgramID)
{
    //    GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[0]);
    //    if (Texture00 == 0)
    //    {
    //        Texture00 = ::g_pTextureManager->getTextureIDFromName("Blank_UV_Text_Texture.bmp");
    //    }
    //
    //    // We are just going to pick texture unit 5 (for no reason, just as an example)
    //    //    glActiveTexture(GL_TEXTURE5);       // #5 TEXTURE UNIT
    //    glActiveTexture(GL_TEXTURE0 + 5);       // #5 TEXTURE UNIT
    //    glBindTexture(GL_TEXTURE_2D, Texture00);
    //
    //    //uniform sampler2D texture_00;
    //    GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
    //    glUniform1i(texture_00_UL, 5);     // <- 5, an integer, because it's "Texture Unit #5"
    //    // ***************************************************************

    //    uniform sampler2D texture_00;			// 2D meaning x,y or s,t or u,v
    //    uniform sampler2D texture_01;
    //    uniform sampler2D texture_02;
    //    uniform sampler2D texture_03;
    //    uniform sampler2D texture_04;			// 2D meaning x,y or s,t or u,v
    //    uniform sampler2D texture_05;
    //    uniform sampler2D texture_06;
    //    uniform sampler2D texture_07;
    //    //... and so on
    //    //uniform float textureMixRatio[8];
    //    uniform vec4 textureMixRatio_0_3;
    //    uniform vec4 textureMixRatio_4_7;

    GLint textureMixRatio_0_3_UL = glGetUniformLocation(shaderProgramID, "textureMixRatio_0_3");
    //    GLint textureMixRatio_4_7_UL = glGetUniformLocation(shaderProgramID, "textureMixRatio_4_7");

    glUniform4f(textureMixRatio_0_3_UL,
        pCurrentMesh->textureRatios[0],
        pCurrentMesh->textureRatios[1],
        pCurrentMesh->textureRatios[2],
        pCurrentMesh->textureRatios[3]);
    //    glUniform4f(textureMixRatio_4_7_UL,
    //                pCurrentMesh->textureRatios[4],
    //                pCurrentMesh->textureRatios[5],
    //                pCurrentMesh->textureRatios[6],
    //                pCurrentMesh->textureRatios[7]);
    // Also set up the height map and discard texture

    {
        // uniform sampler2D heightMapSampler;		// Texture unit 20
        GLint textureUnitNumber = 20;
        GLuint Texture20 = ::g_pTextureManager->getTextureIDFromName("NvF5e_height_map.bmp");
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture20);
        GLint texture_20_UL = glGetUniformLocation(shaderProgramID, "heightMapSampler");
        glUniform1i(texture_20_UL, textureUnitNumber);
    }

    if (pCurrentMesh->textureIsFromFBO)
    {
        //if (pCurrentMesh->isLeftMap)
        //{
        //    GLint textureUnitNumber = 50;
        //    //GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        //    glActiveTexture(GL_TEXTURE0 + textureUnitNumber);

        //    glBindTexture(GL_TEXTURE_2D, g_FBO_2->colourTexture_0_ID);
        //    GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "textureOffScreen_0");
        //    glUniform1i(texture_01_UL, textureUnitNumber);


        //}
        //else
        //{
        //    GLint isRightMap = glGetUniformLocation(shaderProgramID, "isRightMap");
        //    glUniform1f(isRightMap, (GLfloat)true);

        //    GLint textureUnitNumber = 60;
        //    //GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        //    glActiveTexture(GL_TEXTURE0 + textureUnitNumber);

        //    glBindTexture(GL_TEXTURE_2D, g_FBO_1->colourTexture_0_ID);
        //    GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "textureOffScreen_0");
        //    glUniform1i(texture_01_UL, textureUnitNumber);
        //}


        /*if (pCurrentH)*/

        //GLint textureMixRatio_0_1 = glGetUniformLocation(shaderProgramID, "textureFBOMixRatio_0_1");

        //glUniform1i(textureMixRatio_0_1, pCurrentMesh->textureRatios[0],pCurrentMesh->textureRatios[1]);

        //GLint textureUnitNumber = 0;
        ////GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        //glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        //glBindTexture(GL_TEXTURE_2D, pCurrentMesh->FBOTextureNumber);
        //GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
        //glUniform1i(texture_00_UL, textureUnitNumber);


        //GLint textureUnitNumber0 = 0;
        //glActiveTexture(GL_TEXTURE0 + textureUnitNumber0);
        ////        glBindTexture(GL_TEXTURE_2D, Texture00);
        //glBindTexture(GL_TEXTURE_2D, pCurrentMesh->FBOTextureNumber0);
        //GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
        //glUniform1i(texture_00_UL, textureUnitNumber0);

        //GLint textureUnitNumber1 = 1;
        //glActiveTexture(GL_TEXTURE0 + textureUnitNumber1);
        ////        glBindTexture(GL_TEXTURE_2D, Texture00);
        //glBindTexture(GL_TEXTURE_2D, pCurrentMesh->FBOTextureNumber1);
        //GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "texture_01");
        //glUniform1i(texture_01_UL, textureUnitNumber1);

        //return;
    }

    if (pCurrentMesh->isFlickering)
    {
        GLint isFlickering = glGetUniformLocation(shaderProgramID, "isFlickering");
        glUniform1f(isFlickering, (GLfloat)GL_TRUE);

        GLint flicketingAmount = glGetUniformLocation(shaderProgramID, "flickeringAmount");
        glUniform1f(flicketingAmount, pCurrentMesh->flickeringAmount);
    }
    else
    {
        GLint isFlickering = glGetUniformLocation(shaderProgramID, "isFlickering");
        glUniform1f(isFlickering, (GLfloat)GL_FALSE);

        GLint flicketingAmount = glGetUniformLocation(shaderProgramID, "flickeringAmount");
        glUniform1f(flicketingAmount, 0.0f);
    }

    if (pCurrentMesh->isHavingStatic)
    {
        GLint isHavingStatic = glGetUniformLocation(shaderProgramID, "isHavingStatic");
        glUniform1f(isHavingStatic, (GLfloat)GL_TRUE);

        GLint randomUOffset = glGetUniformLocation(shaderProgramID, "randomUOffSet");
        glUniform1f(randomUOffset, MathUtils::GetRandomFloat(-0.2f,0.2f));

        GLint randomVOffset = glGetUniformLocation(shaderProgramID, "randomVOffSet");
        glUniform1f(randomVOffset, MathUtils::GetRandomFloat(-0.2f, 0.2f));
    }
    else
    {
        GLint isHavingStatic = glGetUniformLocation(shaderProgramID, "isHavingStatic");
        glUniform1f(isHavingStatic, (GLfloat)GL_FALSE);
    }

    {
        GLint textureUnitNumber = 0;
        GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture00);
        GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
        glUniform1i(texture_00_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 1;
        GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture01);
        GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "texture_01");
        glUniform1i(texture_01_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 2;
        GLuint Texture02 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture02);
        GLint texture_02_UL = glGetUniformLocation(shaderProgramID, "texture_02");
        glUniform1i(texture_02_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 3;
        GLuint Texture03 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture03);
        GLint texture_03_UL = glGetUniformLocation(shaderProgramID, "texture_03");
        glUniform1i(texture_03_UL, textureUnitNumber);
    }
    // and so on to however many texture you are using

//    uniform vec4 textureMixRatio_0_3;
//    uniform vec4 textureMixRatio_4_7;


    // Set up a skybox
    {
        // uniform samplerCube skyBoxTexture;		// Texture unit 30
        GLint textureUnit30 = 30;
        GLuint skyBoxID = ::g_pTextureManager->getTextureIDFromName("nightsky");
        glActiveTexture(GL_TEXTURE0 + textureUnit30);
        // NOTE: Binding is NOT to GL_TEXTURE_2D
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxID);
        GLint skyBoxSampler_UL = glGetUniformLocation(shaderProgramID, "skyBoxTexture");
        glUniform1i(skyBoxSampler_UL, textureUnit30);
    }

    return;
}

cFBO* Engine::GetRandomFBO()
{
    unsigned int randomIndex = MathUtils::GetRandomInt(0, (unsigned int)g_FBOs.size() - 1);
    return g_FBOs[randomIndex];
}

void Engine::RenderBoneDebug() const
{
    sModelDrawInfo boneDebugModel;
    ::g_pMeshManager->FindDrawInfoByModelName("shapes/cone.obj", boneDebugModel);

    glBindVertexArray(boneDebugModel.VAO_ID);
    glDrawElements(GL_TRIANGLES, boneDebugModel.numberOfTriangles * 3, GL_UNSIGNED_INT, (GLvoid*)0);
}

void Engine::g_DrawDebugSphere(glm::vec3 position, float scale, glm::vec4 colourRGBA, double deltaTime)
{
    // Save the debug sphere state
    bool OLD_isVisible = ::g_pDebugSphereMesh->bIsVisible;
    glm::vec3 OLD_position = ::g_pDebugSphereMesh->drawPosition;
    glm::vec3 OLD_scale = ::g_pDebugSphereMesh->drawScale;
    glm::vec4 OLD_colours = ::g_pDebugSphereMesh->wholeObjectDebugColourRGBA;

    ::g_pDebugSphereMesh->bIsVisible = true;
    ::g_pDebugSphereMesh->drawPosition = position;
    ::g_pDebugSphereMesh->setUniformDrawScale(scale);
    ::g_pDebugSphereMesh->bDoNotLight = true;
    ::g_pDebugSphereMesh->bUseDebugColours = true;
    ::g_pDebugSphereMesh->wholeObjectDebugColourRGBA = colourRGBA;

    DrawObject(::g_pDebugSphereMesh, glm::mat4(1.0f), ::g_DebugSphereMesh_shaderProgramID, deltaTime);

    ::g_pDebugSphereMesh->bIsVisible = OLD_isVisible;
    ::g_pDebugSphereMesh->drawPosition = OLD_position;
    ::g_pDebugSphereMesh->drawScale = OLD_scale;
    ::g_pDebugSphereMesh->wholeObjectDebugColourRGBA = OLD_colours;

    return;
}

void Engine::SetWindowName(std::string windowName)
{
    windowText = windowName;
}

void Engine::DrawLightDebugSpheres(glm::mat4 matProjection, glm::mat4 matView, GLuint shaderProgramID, double deltaTime)
{
    if (!::g_drawDebugLightSpheres)
    {
        return;
    }

    // Draw concentric spheres to indicate light position and attenuation
    cLight lightSelected = g_currentScene->m_sceneLights->GetLightSelected();

    // Small white sphere where the light is
    g_DrawDebugSphere(lightSelected.position,
        0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), deltaTime);

    cLightHelper lightHelper;

    // vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
    float constantAtten = lightSelected.atten.x;
    float linearAtten = lightSelected.atten.y;
    float quadAtten = lightSelected.atten.z;

    // Draw a red sphere at 75% brightness
    {
        float distAt75Percent = lightHelper.calcApproxDistFromAtten(0.75f, 0.01f, 100000.0f,
            constantAtten, linearAtten, quadAtten, 50);

        g_DrawDebugSphere(lightSelected.position,
            distAt75Percent,
            glm::vec4(0.5f, 0.0f, 0.0f, 1.0f), deltaTime);
    }

    // Draw a green sphere at 50% brightness
    {
        float distAt50Percent = lightHelper.calcApproxDistFromAtten(0.50f, 0.01f, 100000.0f,
            constantAtten, linearAtten, quadAtten, 50);

        g_DrawDebugSphere(lightSelected.position,
            distAt50Percent,
            glm::vec4(0.0f, 0.5f, 0.0f, 1.0f), deltaTime);
    }

    // Draw a yellow? sphere at 25% brightness
    {
        float distAt25Percent = lightHelper.calcApproxDistFromAtten(0.25f, 0.01f, 100000.0f,
            constantAtten, linearAtten, quadAtten, 50);

        g_DrawDebugSphere(lightSelected.position,
            distAt25Percent,
            glm::vec4(0.50f, 0.5f, 0.0f, 1.0f), deltaTime);
    }

    // Draw a blue sphere at 1% brightness
    {
        float distAt_5Percent = lightHelper.calcApproxDistFromAtten(0.01f, 0.01f, 100000.0f,
            constantAtten, linearAtten, quadAtten, 50);

        g_DrawDebugSphere(lightSelected.position,
            distAt_5Percent,
            glm::vec4(0.0f, 0.0f, 0.5f, 1.0f), deltaTime);
    }

    return;
}

void Engine::SetUniformVec4InShader(std::string vec4Name, glm::vec4 vector)
{
    GLint uniformLocation = glGetUniformLocation(shaderProgramID, "vec4Name");

    glUseProgram(shaderProgramID);
    glUniform4f(uniformLocation, vector.x, vector.y, vector.z, vector.w);

}