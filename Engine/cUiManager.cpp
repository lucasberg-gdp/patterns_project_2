
#include "OpenGLCommon.h"

#include "cGlobal.h"
#include "cUiManager.h"

#include "cAnimationSystem.h"
#include "cScene.h"
#include "Engine.h"


//extern std::vector< cGameObject* > g_gameObjects;

extern cVAOManager* g_pMeshManager;

extern cAnimationSystem* g_animationSystem;
extern cScene* g_currentScene;
extern Engine engine;
extern bool bUseFlyCamera;
extern bool inGame;

void cUiManager::Init(GLFWwindow* window)
{
    const char* glsl_version = "#version 130";

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    io = ImGui::GetIO(); (void)io;
    io.WantCaptureMouse = true;

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init(glsl_version);

    cGameManager::GetGMInstance()->showUi = true;
}

void cUiManager::End(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void cUiManager::Update(GLFWwindow& window)
{
    if (cGameManager::GetGMInstance()->showUi == false)
    {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //if (show_demo_window)
    //    ImGui::ShowDemoWindow(&show_demo_window);

    if (cGameManager::GetGMInstance()->showUi)
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Editor");

        isHoveringUi = ImGui::IsWindowHovered();

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) 
        {
            isHoveringUi = true;
        }

        if (ImGui::Checkbox("Use Fly camera", &bUseFlyCamera))
        {
            show_another_window = true;
        }

        if (ImGui::Button("Project Info"))
        {
            showProjectInfo = true;
        }

        if (ImGui::Button("Play")) // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            inGame = true;

            engine.SetBoolInFragmentShader("inGame", true);
            bUseFlyCamera = false;

            //g_camera->SetPosition(g_currentScene->GetCameras()[1]->GetPosition());

            g_camera = g_currentScene->GetCameras()[1];

            g_animationSystem->StartAllAnimations(g_currentScene->GetGameObjects());
            //g_camera->MoveToDirection(glm::vec3(0.0f, 0.0f, 1.0f));
            if (cGameManager::GetGMInstance()->showUi)
            {
                cGameManager::GetGMInstance()->showUi = false;

                g_animationSystem->m_isAnimating = true;

                g_animationSystem->StartAllAnimations(g_currentScene->GetGameObjects());
                g_currentScene->isScenePlaying = true;
            }
            else
            {
                g_camera->StopMovingInADirection(glm::vec3(0.0f, 0.0f, 1.0f));
                cGameManager::GetGMInstance()->showUi = true;
                //g_animationSystem->StopAllAnimations(g_gameObjects);
                g_currentScene->isScenePlaying = false;
                g_animationSystem->m_isAnimating = true;

            }

        }
        ImGui::SameLine();
        if (ImGui::Button("Pause"))
        {
            inGame = false;

            engine.SetBoolInFragmentShader("inGame", false);

            g_animationSystem->StopAllAnimations(g_currentScene->m_gameObjects);
            g_currentScene->isScenePlaying = false;
        }

        if (ImGui::Button("Edit lights"))
        {
            showLightsEditor = true;
        }

        if (ImGui::Button("Back to start"))
        {
            g_camera->SetPosition(g_camera->GetInitialPosition());
            g_pFlyCamera->setEye(glm::vec3(7.0f, 5.0f, 11.0f));
        }

        if (ImGui::Button("New Scene"))
        {
            showNewScene = true;
        }

        if (ImGui::Button("New Model"))
        {
            this->showAddModel = true;
        }

        const char** items = cGameManager::GetGMInstance()->GetNameOfAllObjects();
        static int selectedItem = 0;

        ImGui::Text("App average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::Text("Select object");
        if (ImGui::Combo("##",
            &selectedItem,
            cGameManager::GetGMInstance()->GetNameOfAllObjects(),
            cGameManager::GetGMInstance()->GetNumberOfObjects()))
        {
            g_currentScene->SelectMeshByIndex(selectedItem);
            isHoveringUi = true;
        }

        // Display the selected item
        ImGui::Text("Selected Item: %s", ::g_objectSelected->friendlyName.c_str());
        ImGui::Text("Position");
        DisplayVec3InLine("objpos", ::g_objectSelected->drawPosition);
        ImGui::Text("Rotation");
        DisplayRotationInLine("objrot", ::g_objectSelected->drawOrientation);
        ImGui::Text("Scale");
        DisplayObjectScaleInLine("objscl", ::g_objectSelected->drawScale);

        ImGui::Checkbox("Use vertex colours", &(::g_objectSelected->isUsingVertexColors));      // Edit bools storing our window open/close state

        ImGui::Checkbox("Use Bones", &(::g_objectSelected->UseBonesUL));      // Edit bools storing our window open/close state

        static char friendlyNameBuffer[256] = "";
        ImGui::InputText("##NewModelName", friendlyNameBuffer, IM_ARRAYSIZE(friendlyNameBuffer));
        ImGui::SameLine();
        if (ImGui::Button("Update Name"))
        {
            std::string friendlyName(friendlyNameBuffer);
            ::g_objectSelected->friendlyName = friendlyName;
        }

        //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        //ImGui::Checkbox("Another Window", &show_another_window);

        //if (ImGui::SliderFloat("float", &f, 0.0f, 1.0f))
        //{
        //    isHoveringUi = true;
        //}

        //if (ImGui::ColorEdit3("clear color", (float*)&clear_color))
        //{
        //    isHoveringUi = true;
        //}

        if (ImGui::Button("Make a copy"))
        {
            cMesh* newMesh = new cMesh(::g_objectSelected->objectType);
            newMesh->bDoNotLight = ::g_objectSelected->bDoNotLight ;
            newMesh->bIsScaleUniform = ::g_objectSelected->bIsScaleUniform;
            newMesh->drawPosition = ::g_objectSelected->drawPosition;
            newMesh->bIsVisible = ::g_objectSelected->bIsVisible;
            newMesh->bIsWireframe = ::g_objectSelected->bIsWireframe;
            newMesh->bUseDebugColours = ::g_objectSelected->bUseDebugColours;
            newMesh->drawOrientation = ::g_objectSelected->drawOrientation;
            newMesh->setRotationFromEuler(newMesh->drawOrientation);
            newMesh->drawScale = ::g_objectSelected->drawScale;
            newMesh->initialScale = ::g_objectSelected->initialScale;
            newMesh->meshName = ::g_objectSelected->meshName;
            newMesh->transparencyAlpha  = ::g_objectSelected->transparencyAlpha;
            newMesh->wholeObjectDebugColourRGBA = ::g_objectSelected->wholeObjectDebugColourRGBA;

            std::vector<cMesh::Texture> allTextures = ::g_objectSelected->GetAllTextures();

            for (unsigned int i = 0; i < allTextures.size(); i++)
            {
                newMesh->textureName[i] = allTextures[i].Name;
                newMesh->textureRatios[i] = allTextures[i].Ratio;
            }

            for (unsigned int i = 0; i < ::g_objectSelected->vec_pChildMeshes.size(); i++)
            {
                newMesh->vec_pChildMeshes.push_back(::g_objectSelected->vec_pChildMeshes[i]);
            }

            g_currentScene->AddMesh(newMesh);
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Object")) 
        {
            //cGameManager::GetGMInstance()->DeleteGameObjectById(g_objectSelected->GetUniqueID());
            g_currentScene->DeleteMeshById(g_objectSelected->GetUniqueID());
        }

        if (ImGui::Button("Save scene")) // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            g_currentScene->GetCameras()[0] = ::g_pFlyCamera;

            if (g_currentScene->SaveSceneToJson())
            {
                std::cout << "Scene saved!" << std::endl;
            }
            else
            {
                std::cout << "Scene failed to save!" << std::endl;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Load scene")) // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            //LoadJson();
        }
        ImGui::SameLine();


        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::End();

        // 3. Show another simple window.
        if (show_another_window)
        {
            DisplayCameraEditor();
        }

        if (showProjectInfo)
        {
            DisplayProjectInfo();
        }

        if (showNewScene)
        {
            DisplayNewScene();
        }

        if (showLightsEditor)
        {
            DisplayLightEditor(*(g_currentScene->m_sceneLights));
        }

        if (showAddModel)
        {
            DisplayAddModelEditor();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(&window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void cUiManager::DisplayVec3InLine(std::string label, glm::vec3& vec3)
{
    ImGui::Text("x");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "X").c_str(), &vec3.x))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);

        if (physicsObject)
        {
            physicsObject->position.x = vec3.x;
        }
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Y").c_str(), &::g_objectSelected->drawPosition.y))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);
        if (physicsObject)
        {
            physicsObject->position.y = vec3.y;
        }

        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Z").c_str(), &::g_objectSelected->drawPosition.z))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);

        if (physicsObject)
        {
            physicsObject->position.z = vec3.z;
        }
        isHoveringUi = true;
    }
}

void cUiManager::DisplayRotationInLine(std::string label, glm::vec3& eulerXYZ)
{
    glm::vec3 eulerAngles = glm::degrees(eulerXYZ);

    //std::cout << "Euler angles: (" << eulerAngles.x << "," << eulerAngles.y << "," << eulerAngles.z << std::endl;

    ImGui::Text("x");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "X").c_str(), &eulerAngles.x))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);

        if (physicsObject)
        {
            physicsObject->eulerOrientation.x = glm::radians(eulerAngles.x);
            physicsObject->setRotationFromEuler(physicsObject->eulerOrientation);
        }

        g_objectSelected->drawOrientation.y = eulerXYZ.x;
        g_objectSelected->setRotationFromEuler(glm::radians(eulerAngles));
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Y").c_str(), &eulerAngles.y))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);

        if (physicsObject)
        {
            physicsObject->eulerOrientation.y = glm::radians(eulerAngles.y);
            physicsObject->setRotationFromEuler(physicsObject->eulerOrientation);
        }

        g_objectSelected->drawOrientation.y = eulerXYZ.y;
        g_objectSelected->setRotationFromEuler(glm::radians(eulerAngles));
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Z").c_str(), &eulerAngles.z))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);

        if (physicsObject)
        {
            physicsObject->eulerOrientation.z = glm::radians(eulerAngles.z);
            physicsObject->setRotationFromEuler(physicsObject->eulerOrientation);
        }

        g_objectSelected->drawOrientation.z = eulerXYZ.z;
        g_objectSelected->setRotationFromEuler(glm::radians(eulerAngles));

        isHoveringUi = true;
    }
}

void cUiManager::DisplayProjectInfo()
{
    ImGui::Begin("Project Info", &showProjectInfo);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    
    ImGui::Text("1 - Starts cutscene");
    ImGui::Text("2 - Stops cutscene");
    ImGui::NewLine();
    ImGui::Text("Most cutscene logic in Game.cpp, including all Lua functions used in the project.");

    //ImGui::Text("Movement");
    //ImGui::NewLine();
    //ImGui::BulletText("Move around with WSAD");

    //ImGui::NewLine();
    //ImGui::Text("Technologies");
    //ImGui::NewLine();
    //ImGui::BulletText("Grass: Generated in the geometry shader");
    //ImGui::BulletText("Level of detail: The models in the far island are replaced by better resolution models up close");
    //ImGui::BulletText("Fire: Particles spamming from a circle and with cleaned texture");
    //ImGui::BulletText("Character Movement: Character movement made using the vertices in the model below to move around");
    //ImGui::BulletText("Rain: Particles position, velocity and acceleration calculated in a compute shader");
    //ImGui::BulletText("Blur: When the game is running a blur effect is added based on the distance of the vertex to the camera");

    if (ImGui::Button("Close Me"))
    {
        showProjectInfo = false;
    }

    ImGui::End();
}

void cUiManager::DisplayNewScene()
{
    ImGui::Begin("New Scene", &showNewScene);

    static char newSceneBuffer[256] = "";
    ImGui::InputText("Scene name", newSceneBuffer, IM_ARRAYSIZE(newSceneBuffer));

    if (ImGui::Button("Close Me"))
    {
        showNewScene = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Create"))
    {
        std::string sceneName(newSceneBuffer);

        cJsonHandler jsonReaderWriter;
        jsonReaderWriter.CreateScene(sceneName);
        showNewScene = false;
    }

    ImGui::End();
}

void cUiManager::DisplayCameraEditor()
{
    static int selectedCamera = 0;
    const char** camerasCharArray = cGameManager::GetGMInstance()->GetNameOfAllCameras();

    ImGui::Begin("Game cameras", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Select camera");
    if (ImGui::Combo("##cameras",
        &selectedCamera,
        cGameManager::GetGMInstance()->GetNameOfAllCameras(),
        cGameManager::GetGMInstance()->GetNumberOfCameras()))
    {
        g_camera = (iCamera*)g_currentScene->GetCameras()[(size_t)selectedCamera + 1]; // skipping fly camera
        isHoveringUi = true;
    }

    glm::vec3 vec3 = g_camera->GetPosition();

    std::string label = "CameraPosition";
    ImGui::Text("x");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "X").c_str(), &vec3.x))
    {
        g_camera->SetPosition(vec3);
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Y").c_str(), &vec3.y))
    {
        g_camera->SetPosition(vec3);
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Z").c_str(), &vec3.z))
    {
        g_camera->SetPosition(vec3);
        isHoveringUi = true;
    }

    glm::vec3 targetVec3 = g_camera->GetTarget();

    label = "CameraTarget";
    ImGui::Text("x");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "X").c_str(), &targetVec3.x))
    {
        g_camera->setAt(targetVec3);
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Y").c_str(), &targetVec3.y))
    {
        g_camera->setAt(targetVec3);
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Z").c_str(), &targetVec3.z))
    {
        g_camera->setAt(targetVec3);
        isHoveringUi = true;
    }


    if (ImGui::Button("Close Me"))
    {
        show_another_window = false;
    }

    ImGui::End();
}

void cUiManager::DisplayLightEditor(cLightManager& lights)
{
    static int selectedLight = 0;

    ImGui::Begin("Game lights", &showLightsEditor); 
    ImGui::Text("Select light");
    if (ImGui::Combo("##lights",
        &selectedLight,
        g_currentScene->GetNameOfAllLights(),
        g_currentScene->GetNumberOfLights()))
    {
        lights.SelectLight(selectedLight);
        isHoveringUi = true;
    }

    cLight lightSelected = lights.GetLightSelected();

    ImGui::Text("Selected Light: %s", lightSelected.lightFriendlyName);
    ImGui::Text("Position");
    ImGui::Text("x");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);

    glm::vec4 lightPosition = lightSelected.position;

    if (ImGui::InputFloat("##lightPositionX", &lightPosition.x))
    {
        lights.theLights[lights.GetSelectedLight()].position = lightPosition;
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat("##lightPositionY", &lightPosition.y))
    {
        lights.theLights[lights.GetSelectedLight()].position = lightPosition;
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat("##lightPositionZ", &lightPosition.z))
    {
        lights.theLights[lights.GetSelectedLight()].position = lightPosition;
        isHoveringUi = true;
    }

    // Direction

    glm::vec3 eulerAngles = glm::degrees(glm::vec3(lightSelected.direction.x, lightSelected.direction.y, lightSelected.direction.z));

    ImGui::Text("Direction");
    ImGui::Text("x");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);

    glm::vec4 lightDirection = lightSelected.direction;
    if (ImGui::InputFloat("##lightDirectionX", &lightDirection.x))
    {
        lights.theLights[lights.GetSelectedLight()].direction.x = lightDirection.x;
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat("##lightDirectionY", &lightDirection.y))
    {
        lights.theLights[lights.GetSelectedLight()].direction.y = lightDirection.y;
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat("##lightDirectionZ", &lightDirection.z))
    {
        lights.theLights[lights.GetSelectedLight()].direction.z = lightDirection.z;
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("w");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat("##lightDirectionW", &lightDirection.w))
    {
        lights.theLights[lights.GetSelectedLight()].direction.w = lightDirection.w;
        isHoveringUi = true;
    }

    //::g_pTheLights->theLights[1].param1.y = 55.0f; // x = lightType, y = inner angle, z = outer angle, w = TBD
    float innerAngle = lightSelected.param1.y;
        
    float outerAngle = lightSelected.param1.z;

    ImGui::Text("Inner Angle");
    ImGui::SameLine();
    if (ImGui::InputFloat("##lightInnerAngle", &innerAngle))
    {
        lights.theLights[lights.GetSelectedLight()].param1.y = innerAngle;
        isHoveringUi = true;
    }

    ImGui::Text("Outer Angle");
    ImGui::SameLine();
    if (ImGui::InputFloat("##lightOuterAngle", &outerAngle))
    {
        lights.theLights[lights.GetSelectedLight()].param1.z = outerAngle;
        isHoveringUi = true;
    }


    ImVec4 lightDiffuse = ImVec4(
        lightSelected.diffuse.x,
        lightSelected.diffuse.y,
        lightSelected.diffuse.z,
        lightSelected.diffuse.a);

    ImGui::Text("Diffuse");
    if (ImGui::ColorEdit3("##Diffuse", (float*)&lightDiffuse))
    {
        glm::vec4 newLightColour = glm::vec4(lightDiffuse.x, lightDiffuse.y, lightDiffuse.z, lightDiffuse.w);

        lights.theLights[lights.GetSelectedLight()].diffuse = newLightColour;
        isHoveringUi = true;
    }

    ImVec4 lightSpecular = ImVec4(
        lightSelected.specular.x,
        lightSelected.specular.y,
        lightSelected.specular.z,
        lightSelected.specular.a);

    ImGui::Text("Specular");
    if (ImGui::ColorEdit3("##Specular", (float*)&lightSpecular))
    {
        glm::vec4 newLightSpecular = glm::vec4(lightSpecular.x, lightSpecular.y, lightSpecular.z, lightSpecular.w);

        lights.theLights[lights.GetSelectedLight()].specular = newLightSpecular;
        isHoveringUi = true;
    }

    ImGui::Text("Attenuation");
    ImGui::Text("x");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f);

    glm::vec4 lightAttenuation = lightSelected.atten;

    if (ImGui::InputFloat("##lightAttenuationX", &lightAttenuation.x, 0.0f, 0.0f, "%.7f"))
    {
        lights.theLights[lights.GetSelectedLight()].atten = lightAttenuation;
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f);
    if (ImGui::InputFloat("##lightAttenuationY", &lightAttenuation.y, 0.0f, 0.0f, "%.7f"))
    {
        lights.theLights[lights.GetSelectedLight()].atten = lightAttenuation;
        //lightSelected.position = lightPosition;
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f);
    if (ImGui::InputFloat("##lightAttenuationZ", &lightAttenuation.z, 0.0f, 0.0f, "%.7f"))
    {
        lights.theLights[lights.GetSelectedLight()].atten = lightAttenuation;
        //lightSelected.position = lightPosition;
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("w");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f);
    if (ImGui::InputFloat("##lightAttenuationW", &lightAttenuation.w, 0.0f, 0.0f, "%.7f"))
    {
        lights.theLights[lights.GetSelectedLight()].atten = lightAttenuation;
        //lightSelected.position = lightPosition;
        isHoveringUi = true;
    }

    bool lightIsOn = (lightSelected.param2.x == 1.0f);
    ImGui::Checkbox("ON/OFF", &lightIsOn);

    if (lightIsOn)
    {
        lights.theLights[lights.GetSelectedLight()].param2.x = 1.0f;
    }
    else
    {
        lights.theLights[lights.GetSelectedLight()].param2.x = 0.0f;
    }

    ImGui::Checkbox("Debug light spheres", &(::g_drawDebugLightSpheres));      // Edit bools storing our window open/close state
    
    if (ImGui::Button("Turn off all lights"))
    {
        lights.TurnOffAllLights();
    }

    if (ImGui::Button("Turn on all lights"))
    {
        lights.TurnOnAllLights();
    }

    static char newLightNameBuffer[256] = "";
    ImGui::InputText("##NewName", newLightNameBuffer, IM_ARRAYSIZE(newLightNameBuffer));
    ImGui::SameLine();
    if (ImGui::Button("Update Name"))
    {
        std::string lightFriendlyName(newLightNameBuffer);
        lights.theLights[lights.GetSelectedLight()].lightFriendlyName = lightFriendlyName;
    }

    int lightTypeIndex = 0;
    const char* lightTypes[] = { "Point", "Spot", "Directional"};
    const char** lightTypesPtr = lightTypes;


    if (lightSelected.param1.x == 0.0f)
    {
        lightTypeIndex = 0;
    }
    else if (lightSelected.param1.x == 1.0f)
    {
        lightTypeIndex = 1;
    }
    else if(lightSelected.param1.x == 2.0f)
    {
        lightTypeIndex = 2;
    }
    
    ImGui::Text("Light type");
    if (ImGui::Combo("##lightTypes",
        &lightTypeIndex,
        lightTypes,
        3))
    {
        lights.theLights[lights.GetSelectedLight()].param1.x = (float)lightTypeIndex;
        isHoveringUi = true;
    }

    if (ImGui::Button("Close Me"))
    {
        showLightsEditor = false;
    }

    ImGui::End();
}

void cUiManager::DisplayAddModelEditor()
{
    ImGui::Begin("Add new model", &showAddModel);

    static int modelTypeIndex = 0;
    const char** allModelTypes = g_pMeshManager->GetTypesOfModels();
    ImGui::Text("Model Type");
    if (ImGui::Combo("##ModelType",
        &modelTypeIndex,
        allModelTypes,
        g_pMeshManager->GetNumberOfModelTypes()))
    {
        isHoveringUi = true;
    }

    static int selectedModelIndex = 0;

    //const char** allModels = ;

    ImGui::Text("Select Model");
    if (ImGui::Combo("##NewModel",
        &selectedModelIndex,
        g_pMeshManager->GetNameOfAllModelsByType(allModelTypes[modelTypeIndex]),
        g_pMeshManager->GetNumberOfModelsOfType(allModelTypes[modelTypeIndex])))
    {

        isHoveringUi = true;
    }

    std::string selectedModel = std::string(g_pMeshManager->GetNameOfAllModelsByType(allModelTypes[modelTypeIndex])[selectedModelIndex]);

    if (ImGui::Button("Create Model"))
    {
        cMesh* mesh = new cMesh();
        mesh->friendlyName = selectedModel;
        mesh->objectType = "prop";
        mesh->transparencyAlpha = 1.0f;

        mesh->meshName = selectedModel;
        mesh->textureName[0] = "Dungeons_2_Texture_01_A.bmp";
        mesh->textureRatios[0] = 1.0f;

        g_currentScene->AddMesh(mesh);
    }

    ImGui::End();
}

void cUiManager::DisplayObjectScaleInLine(std::string label, glm::vec3& vec3)
{
    ImGui::Text("x");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "X").c_str(), &vec3.x))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);

        if (physicsObject)
        {
            //physicsObject->drawScale.x = vec3.x;
            g_objectSelected->drawScale.x = vec3.x;
        }
        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("y");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Y").c_str(), &vec3.y))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);
        if (physicsObject)
        {
            //physicsObject->position.y = vec3.y;
            g_objectSelected->drawScale.y = vec3.y;
        }

        isHoveringUi = true;
    }
    ImGui::SameLine();

    ImGui::Text("z");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(55.0f);
    if (ImGui::InputFloat(("##" + label + "Z").c_str(), &vec3.z))
    {
        sPhysicsProperties* physicsObject = cGameManager::GetGMInstance()->FindPhysicsByFriendlyName(::g_objectSelected->friendlyName);

        if (physicsObject)
        {
            //physicsObject->position.z = vec3.z;
            g_objectSelected->drawScale.z = vec3.z;
        }
        isHoveringUi = true;
    }
}

