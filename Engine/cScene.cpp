#include <algorithm>

#include "cScene.h"

#include "CharacterController.h"

//extern int g_selectedMesh;
extern cMesh* g_objectSelected;
//extern std::vector<iCamera*> cameras;

cScene::cScene()
{
    m_enemyManager = new cEnemyManager();
    m_enemyManager->Init();
}

cScene::~cScene()
{
    delete m_uiManager;
    delete m_pFlyCamera;
    delete m_scenePlayer;
    delete m_skyBox;
    delete m_enemyManager;
}

cScene::cScene(std::string sceneName)
{
    m_sceneName = sceneName;
}

void cScene::Init()
{
    //m_systems.push_back(new CharacterController());
}

void cScene::Awake(GLFWwindow* window)
{

}

void cScene::Start(GLFWwindow* window)
{
    for (iSystem* system : m_systems)
    {
        system->Start(*window);
    }

    //m_commandGroup->Start();
}

void cScene::Update(GLFWwindow* window, double deltaTime)
{
    for (iSystem* system : m_systems)
    {
        system->Update(*window, deltaTime);
    }

    m_scenePhysics->Update(deltaTime);
    m_sceneLights->Update(deltaTime);
    m_enemyManager->Update(deltaTime);

    UpdateGameObjects(deltaTime);
}

void cScene::DestroyObjectById(unsigned int id)
{
}

void cScene::UpdateGameObjects(double deltaTime)
{
    for (unsigned int i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i]->Update(deltaTime);
    }

    //m_commandGroup->Update(deltaTime);
    m_commandManager->Update(deltaTime);
}

std::vector<cMesh*>& cScene::GetAllMeshes()
{
	return m_meshes;
}

void cScene::AddMesh(cMesh*& meshToAdd)
{
	m_meshes.push_back(meshToAdd);
}

bool cScene::DeleteMeshById(unsigned int id)
{
    unsigned int idToDelete = 0;
    bool foundMesh = false;

    for (cMesh* mesh : m_meshes)
    {
        if (mesh->GetUniqueID() == id)
        {
            foundMesh = true;
            break;
        }

        idToDelete++;
    }

    delete m_meshes[idToDelete];
    m_meshes.erase(m_meshes.begin() + idToDelete);

    SelectNextMesh();

    return foundMesh;
}

cMesh* cScene::GetMeshByFriendlyName(std::string friendlyName)
{
    for (cMesh* mesh : m_meshes)
    {
        if (mesh->friendlyName == friendlyName)
        {
            return mesh;
        }
    }

    return new cMesh();
}

cGameObject* cScene::GetPlayer()
{
    for (int i = 0; i < m_gameObjects.size(); i++)
    {
        if (m_gameObjects[i]->tag == "captain1")
        {
            return m_gameObjects[i];
        }
    }

    return nullptr;
}

void cScene::AddGameObject(cGameObject* gameObjectToAdd)
{
    m_gameObjects.push_back(gameObjectToAdd);
}

bool cScene::RemoveMeshByFriendlyName(std::string friendlyName)
{
    for (int i = 0; i < m_meshes.size(); i++)
    {
    	if (m_meshes[i]->friendlyName == friendlyName)
    	{
    
            m_meshes.erase(m_meshes.begin() + i);
    		return true;
    	}
    }

    return false;

}

bool CompareMeshesByFriendlyName(const cMesh* mesh1, const cMesh* mesh2)
{
	return mesh1->friendlyName < mesh2->friendlyName;
}

void cScene::SortMeshesByName()
{
	std::sort(m_meshes.begin(), m_meshes.end(), CompareMeshesByFriendlyName);
}

void cScene::SelectNextMesh()
{
    bool isValidMesh = false;

    while (!isValidMesh)
    {
		selectedMeshID++;
        //g_selectedMesh++;

        if (selectedMeshID >= m_meshes.size())
        {
            selectedMeshID = 0;
        }

        if (m_meshes[selectedMeshID]->objectType != "tree")
        {
            std::cout << "Selected model: " << selectedMeshID << std::endl;
            ::g_objectSelected = m_meshes[selectedMeshID];
            isValidMesh = true;
        }
    }
}

void cScene::SelectPreviousMesh()
{
	bool isValidMesh = false;

	while (!isValidMesh)
	{
		selectedMeshID--;
	
		if (selectedMeshID < 0)//>= ::g_vec_pMeshesToDraw.size())
		{
			selectedMeshID = (int)m_meshes.size() - 1;
		}
	
		if (m_meshes[selectedMeshID]->objectType != "tree")
		{
			::g_objectSelected = m_meshes[selectedMeshID];
			std::cout << "Selected model: " << selectedMeshID << " Friendly name: " << ::g_objectSelected->friendlyName << std::endl;
			::g_objectSelected = m_meshes[selectedMeshID];
			isValidMesh = true;
		}
	}
}

void cScene::SelectMeshByIndex(unsigned int index)
{
	g_objectSelected = m_meshes[index];
}

void cScene::DisplaySceneSkyBox(GLuint shaderProgramID, double deltaTime)
{
    m_skyBox->DrawSkyBox(shaderProgramID, deltaTime);
}

void cScene::RestartTransforms()
{
    for (cGameObject* gameObject : m_gameObjects)
    {
        gameObject->RestartTransform();
    }

    for (iCamera* camera : m_sceneCameras)
    {
        camera->ResetCameraInitialConfig();
        //camera->setAt(glm::vec3(0.0f));
        //camera->SetOrientation(glm::vec3(0.0f));
    }
}

const char** cScene::GetNameOfAllLights()
{
	unsigned int size = 0;

	for (unsigned int i = 0; i < m_sceneLights->NUMBER_OF_LIGHTS_IM_USING; i++)
	{
		size += (unsigned int)m_sceneLights->theLights[i].lightFriendlyName.size();
	}

	const char** items = new const char* [size];

	int index = 0;
	// Copy each string from the vector to the array
	for (unsigned int i = 0; i < m_sceneLights->NUMBER_OF_LIGHTS_IM_USING; ++i)
	{
		std::string cameraFriendlyName = m_sceneLights->theLights[i].lightFriendlyName;
		items[index] = _strdup(cameraFriendlyName.c_str());
		++index;
	}

	return items;
}

unsigned int cScene::GetNumberOfLights()
{
	return (unsigned int)m_sceneLights->NUMBER_OF_LIGHTS_IM_USING;
}

std::vector<iCamera*>& cScene::GetCameras()
{
	return m_sceneCameras;
}

std::vector<cGameObject*>& cScene::GetGameObjects()
{
    return m_gameObjects;
}

bool cScene::SaveSceneToJson()
{
    std::cout << "saving info" << std::endl;
    cJsonHandler configReaderWriter;

    sSceneInfo sceneInfo = SerializeScene();

    configReaderWriter.SaveScene(sceneInfo);

    //std::string SceneFileName = configReaderWriter.SceneFileName;
    //std::vector<sModel> allModels;

    //for (unsigned int i = 0; i < m_meshes.size(); i++)
    //{
    //    if (m_meshes[i]->isToSave == false)
    //    {
    //        continue;
    //    }

    //    sModel model;

    //    model.MeshName = m_meshes[i]->meshName;
    //    model.Position = m_meshes[i]->getDrawPosition();
    //    model.Alpha = m_meshes[i]->transparencyAlpha;
    //    model.DebugColourRGBA = m_meshes[i]->wholeObjectDebugColourRGBA;
    //    model.DrawScale = m_meshes[i]->drawScale;
    //    model.FriendlyName = m_meshes[i]->friendlyName;
    //    model.ObjectType = m_meshes[i]->objectType;
    //    model.IsUsingDebugColors = m_meshes[i]->bUseDebugColours;
    //    model.IsUsingVertexColors = m_meshes[i]->isUsingVertexColors;
    //    model.IsUsingBones = m_meshes[i]->UseBonesUL;
    //    model.Orientation = m_meshes[i]->GetOrientation();

    //    std::vector<cMesh::Texture> texturesInMesh = m_meshes[i]->GetAllTextures();
    //    for (unsigned int i = 0; i < texturesInMesh.size(); i++)
    //    {
    //        sTextureInfo textureInfo;
    //        textureInfo.Name = texturesInMesh[i].Name;
    //        textureInfo.Ratio = texturesInMesh[i].Ratio;

    //        model.ObjectTextures.push_back(textureInfo);
    //    }

    //    allModels.push_back(model);
    //}

    //configReaderWriter.WriteSceneConfig(SceneFileName, allModels);

    //std::string cameraInfoFileName = "docs/CameraInfo.json";
    //std::vector<sCameraInfo> allCameras;

    //for (unsigned int i = 0; i < m_sceneCameras.size(); i++)
    //{
    //    sCameraInfo camera;

    //    camera.CameraType = m_sceneCameras[i]->GetCameraType();
    //    camera.Position = m_sceneCameras[i]->GetPosition();
    //    camera.Orientation = m_sceneCameras[i]->GetOrientation();
    //    camera.Target = m_sceneCameras[i]->GetTarget();
    //    camera.Forward = m_sceneCameras[i]->GetForward();
    //    camera.Up = m_sceneCameras[i]->GetUp();
    //    camera.NearClip = m_sceneCameras[i]->GetNearClip();
    //    camera.FarClip = m_sceneCameras[i]->GetFarClip();

    //    allCameras.push_back(camera);
    //}

    //if (!configReaderWriter.WriteCameraConfig(cameraInfoFileName, allCameras))
    //{
    //    return false;
    //}

    //std::string SceneLightsName = "docs/SceneLights.json";

    //std::vector< sLightsInfo > allLights;
    //for (unsigned int i = 0; i < m_sceneLights->NUMBER_OF_LIGHTS_IM_USING; i++)
    //{
    //    sLightsInfo light;

    //    light.FriendlyName = m_sceneLights->theLights[i].lightFriendlyName;
    //    light.Position = m_sceneLights->theLights[i].position;
    //    light.Diffuse = m_sceneLights->theLights[i].diffuse;
    //    light.Specular = m_sceneLights->theLights[i].specular;
    //    light.Atten = m_sceneLights->theLights[i].atten;
    //    light.Direction = m_sceneLights->theLights[i].direction;
    //    light.Param1 = m_sceneLights->theLights[i].param1;
    //    light.Param2 = m_sceneLights->theLights[i].param2;

    //    allLights.push_back(light);
    //}

    //if (!configReaderWriter.WriteLightsConfig(SceneLightsName, allLights))
    //{
    //    return false;
    //}

    return true;
}

sSceneInfo cScene::SerializeScene()
{
    sSceneInfo serializedScene;

    serializedScene.SceneName = m_sceneName;

    // Serialize meshes
    std::cout << "saving info" << std::endl;
    cJsonHandler configReaderWriter;

    std::string SceneFileName = configReaderWriter.SceneFileName;

    for (cMesh* mesh : m_meshes)
    {
        if (mesh->isToSave == false)
        {
            continue;
        }

        sModel model;

        model.MeshName = mesh->meshName;
        model.Position = mesh->getDrawPosition();
        model.Alpha = mesh->transparencyAlpha;
        model.DebugColourRGBA = mesh->wholeObjectDebugColourRGBA;
        model.DrawScale = mesh->drawScale;
        model.FriendlyName = mesh->friendlyName;
        model.ObjectType = mesh->objectType;
        model.IsUsingDebugColors = mesh->bUseDebugColours;
        model.IsUsingVertexColors = mesh->isUsingVertexColors;
        model.IsUsingBones = mesh->UseBonesUL;
        model.Orientation = mesh->GetOrientation();

        std::vector<cMesh::Texture> texturesInMesh = mesh->GetAllTextures();

        for (unsigned int i = 0; i < texturesInMesh.size(); i++)
        {
            sTextureInfo textureInfo;
            textureInfo.Name = texturesInMesh[i].Name;
            textureInfo.Ratio = texturesInMesh[i].Ratio;

            model.ObjectTextures.push_back(textureInfo);
        }

        serializedScene.ModelInfo.push_back(model);
    }

    // Serialize cameras
    for (iCamera* camera : m_sceneCameras)
    {
        sCameraInfo cameraInfo;

        cameraInfo.CameraType = camera->GetCameraType();
        cameraInfo.Position = camera->GetPosition();
        cameraInfo.Orientation = camera->GetOrientation();
        cameraInfo.Target = camera->GetTarget();
        cameraInfo.Forward = camera->GetForward();
        cameraInfo.Up = camera->GetUp();
        cameraInfo.NearClip = camera->GetNearClip();
        cameraInfo.FarClip = camera->GetFarClip();

        serializedScene.CameraInfo.push_back(cameraInfo);
    }

    // Serialize lights
    for (unsigned int i = 0; i < m_sceneLights->NUMBER_OF_LIGHTS_IM_USING; i++)
    {
        sLightsInfo light;

        light.FriendlyName = m_sceneLights->theLights[i].lightFriendlyName;
        light.Position = m_sceneLights->theLights[i].position;
        light.Diffuse = m_sceneLights->theLights[i].diffuse;
        light.Specular = m_sceneLights->theLights[i].specular;
        light.Atten = m_sceneLights->theLights[i].atten;
        light.Direction = m_sceneLights->theLights[i].direction;
        light.Param1 = m_sceneLights->theLights[i].param1;
        light.Param2 = m_sceneLights->theLights[i].param2;

        serializedScene.LightsInfo.push_back(light);
    }

    return serializedScene;
}

cLight cScene::GetLightByFriendlyName(std::string friendlyName)
{
    cLight light;
    this->m_sceneLights->GetLightByFriendlyName(friendlyName, light);

    return light;
}

void cScene::AddCamera(iCamera* camera)
{
    this->m_sceneCameras.push_back(camera);
}

iSystem* cScene::GetSystem(std::string systemName)
{
    for (iSystem* system : this->m_systems)
    {
        if (system->GetSystemType() == systemName)
        {
            return system;
        }
    }

    return nullptr;
}
