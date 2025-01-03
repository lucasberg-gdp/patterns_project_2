#include "GLWF_CallBacks.h"
#include "cGlobal.h"    // For GFLW header

#include <string>
#include <iostream>
#include <vector>
#include "cMesh.h"
#include "cLightManager.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "cInputHandler.h"
#include "cUiManager.h"
#include "cAnimationSystem.h"
#include "cLionPlayer.h"
#include "cScene.h"
#include "cParticleSystem.h"
#include "cSoftBodyVerlet.h"
#include "CharacterController.h"
#include "Game.h"

extern cScene* g_currentScene;

//extern std::vector<iCamera*> cameras;

extern glm::vec3 g_cameraEye;
bool bUseFlyCamera = true;

bool g_IsMouseInsideWindow = false;

//extern bool showUi;

extern cAnimationSystem* g_animationSystem;

extern int g_selectedLight;// = 0;

extern cMesh* g_objectSelected;
extern cUiManager* g_uiManager;
//extern cLionPlayer* lionPlayer;
extern cSoftBodyVerlet* g_softBody;

extern cParticleSystem g_anEmitter;

extern Game game;

//void mouseScrollWheel_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    float mouseScrollWheelSensitivity = 0.01f;
//    ::g_pFlyCamera->setMouseWheelDelta(yoffset * mouseScrollWheelSensitivity);
//    return;
//}
//
//void mouseEnteredWindow_callback(GLFWwindow* window, int entered)
//{
//    if (entered)
//    {
//        ::g_IsMouseInsideWindow = true;
//    }
//    else
//    {
//        ::g_IsMouseInsideWindow = false;
//    }
//    return;
//}

void cInputHandler::MouseScrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!g_uiManager->isHoveringUi)
    {
        float mouseScrollWheelSensitivity = 0.01f;
        ::g_pFlyCamera->setMouseWheelDelta(yoffset * mouseScrollWheelSensitivity);
    }
}

void cInputHandler::MouseEnteredWindowCallback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        ::g_IsMouseInsideWindow = true;
    }
    else
    {
        ::g_IsMouseInsideWindow = false;
    }
}


//void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
//{
//    switch (button)
//    {
//    case GLFW_MOUSE_BUTTON_1:
//        //std::cout << "GLFW_MOUSE_BUTTON_1" << std::endl;
//        break;
//    case GLFW_MOUSE_BUTTON_2:
//        //std::cout << "GLFW_MOUSE_BUTTON_2" << std::endl;
//        break;
//    case GLFW_MOUSE_BUTTON_3:
//        //std::cout << "GLFW_MOUSE_BUTTON_3" << std::endl;
//        break;
//    }
//    return;
//}

bool cInputHandler::AreAllModifiersUp(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { return false; }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) { return false; }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { return false; }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) { return false; }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) { return false; }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) { return false; }

    return true;
}

void cInputHandler::KeyDown(unsigned int key)
{
    m_Keys[key] = true;

    //std::cout << "pressed key" << key;
}

void cInputHandler::KeyUp(unsigned int key)
{
    m_Keys[key] = false;
}

void cInputHandler::ModeDown(unsigned char mode)
{
    m_Modes[mode] = true;
}

void cInputHandler::ModeUp(unsigned char mode)
{
    m_Modes[mode] = false;
}

void cInputHandler::AsyncMouseHandlingUpdate(GLFWwindow* window)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    ::g_pFlyCamera->setMouseXY(x, y);

    const float MOUSE_MOVE_SENSITIVITY = 0.1f;

    if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && ::g_IsMouseInsideWindow)
    {
        if (!g_uiManager->isHoveringUi)
        {
            // If mouse button is down, then aim the camera
            ::g_pFlyCamera->Yaw_LeftRight(-::g_pFlyCamera->getDeltaMouseX() * MOUSE_MOVE_SENSITIVITY);
            ::g_pFlyCamera->Pitch_UpDown(::g_pFlyCamera->getDeltaMouseY() * MOUSE_MOVE_SENSITIVITY);
        }
    }

    // TODO: This is pretty janky....
    if (::g_IsMouseInsideWindow)
    {
        const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

        ::g_pFlyCamera->movementSpeed += (::g_pFlyCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY);

        // We are looking for the CHANGE in the mouse wheel, but it's storing the 
        //  absolute value of the current mouse wheel
        ::g_pFlyCamera->clearMouseWheelValue();

        // Don't let the speed change go negative
        if (::g_pFlyCamera->movementSpeed <= 0.0f)
        {
            ::g_pFlyCamera->movementSpeed = 0.0f;
        }
    }

    return;
}

void cInputHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const float CAMERA_ROTATION_SPEED = 0.03f;
    const float LIGHT_MOVEMENT_SPEED = 1.0f;
    const float cameraMovementSpeed = 0.5f;

    if (action == GLFW_PRESS)
    {
        //std::cout << "pressing key: " << key << std::endl;
        // << "  mode is: " << mods << std::endl;
        KeyDown(key);
        OnPress(key);
    }

    if (action == GLFW_RELEASE)
    {
        //std::cout << "releasing key: " << key << std::endl;
        KeyUp(key);
        OnRelease(key);
    }

    if (mods == 0)
    {
        ModeUp(GLFW_MOD_SHIFT);
        ModeUp(GLFW_MOD_ALT);
        ModeUp(GLFW_MOD_CONTROL);
    }
    else
    {
        ModeDown(mods);
    }

    switch (currentMode)
    {
    case InputMode::EditorMode:

        if (::bUseFlyCamera)
        {
            // Old way
            // Left and Right
            //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { ::g_pFlyCamera->MoveLeftRight_X(-cameraMovementSpeed); }
            //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { ::g_pFlyCamera->MoveLeftRight_X(+cameraMovementSpeed); }

            //// Forward and backwards
            //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { ::g_pFlyCamera->MoveForward_Z(+cameraMovementSpeed); }
            //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { ::g_pFlyCamera->MoveForward_Z(-cameraMovementSpeed); }

            //// Up and down
            //if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { ::g_pFlyCamera->MoveUpDown_Y(-cameraMovementSpeed); }
            //if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { ::g_pFlyCamera->MoveUpDown_Y(+cameraMovementSpeed); }


            // TODO: Complete new way to move camera
            // Left and Right
            //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
            //{
            //    //::g_pFlyCamera->AddVelocityToCamera("l", glm::vec3()
            //    ::g_pFlyCamera->MoveLeftRight_X(-cameraMovementSpeed); 
            //}
            //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
            //{ 
            //    ::g_pFlyCamera->MoveLeftRight_X(+cameraMovementSpeed);
            //}

            //// Forward and backwards
            //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            //{ 
            //    ::g_pFlyCamera->MoveForward_Z(+cameraMovementSpeed); 
            //}
            //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
            //{ 
            //    ::g_pFlyCamera->MoveForward_Z(-cameraMovementSpeed); 
            //}

            //// Up and down
            //if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) 
            //{ 
            //    ::g_pFlyCamera->MoveUpDown_Y(-cameraMovementSpeed); 
            //}
            //if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) 
            //{ 
            //    ::g_pFlyCamera->MoveUpDown_Y(+cameraMovementSpeed); 
            //}
        }


        // load scene from a file
        if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
        {
            //TODO: add to load scene from json instead of txt
            //LoadVectorSceneFromFile("docs/meshesinfo.txt", "docs/lightsinfo.txt", "docs/physicsinfo.txt");
        }

        // light control
        if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL) // Shift key down (ignores other keys)
        {
            //g_camera->LookAtTarget(g_currentScene->m_sceneLights->theLights[g_currentScene->m_sceneLights->GetSelectedLight()].position);

            //if (key == GLFW_KEY_A)
            //{
            //    g_currentScene->m_sceneLights->theLights[g_currentScene->m_sceneLights->GetSelectedLight()].position.x -= LIGHT_MOVEMENT_SPEED;
            //    g_camera->LookAtTarget(g_currentScene->m_sceneLights->theLights[g_currentScene->m_sceneLights->GetSelectedLight()].position);

            //}
            //if (key == GLFW_KEY_D)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].position.x += LIGHT_MOVEMENT_SPEED;
            //    g_camera->LookAtTarget(::g_pTheLights->theLights[g_pTheLights->GetSelectedLight()].position);

            //}

            //if (key == GLFW_KEY_W)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].position.z += LIGHT_MOVEMENT_SPEED;
            //    g_camera->LookAtTarget(::g_pTheLights->theLights[g_pTheLights->GetSelectedLight()].position);

            //}
            //if (key == GLFW_KEY_S)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].position.z -= LIGHT_MOVEMENT_SPEED;
            //    g_camera->LookAtTarget(::g_pTheLights->theLights[g_pTheLights->GetSelectedLight()].position);

            //}

            //if (key == GLFW_KEY_Q)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].position.y -= LIGHT_MOVEMENT_SPEED;
            //    g_camera->LookAtTarget(::g_pTheLights->theLights[g_pTheLights->GetSelectedLight()].position);

            //}
            //if (key == GLFW_KEY_E)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].position.y += LIGHT_MOVEMENT_SPEED;
            //    g_camera->LookAtTarget(::g_pTheLights->theLights[g_pTheLights->GetSelectedLight()].position);

            //}

            //// Linear attenuation 
            //if (key == GLFW_KEY_1)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].atten.y *= 0.99f;      // Less 1%
            //}
            //if (key == GLFW_KEY_2)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].atten.y *= 1.01f;      // 1% more
            //}

            //// quadratic attenuation 
            //if (key == GLFW_KEY_3)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].atten.z *= 0.99f;      // Less 1%
            //}
            //if (key == GLFW_KEY_4)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].atten.z *= 1.01f;      // 1% more
            //}

            //// 5 & 8 are outter angle
            //if (key == GLFW_KEY_5)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].param1.z -= 0.1f;      // 0.1 degree
            //}
            //if (key == GLFW_KEY_8)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].param1.z += 0.1f;      // 0.1 degree
            //}
            //// 6 & 7 are inner angle
            //if (key == GLFW_KEY_6)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].param1.y -= 0.1f;      // 0.1 degree
            //}
            //if (key == GLFW_KEY_7)
            //{
            //    ::g_pTheLights->theLights[::g_pTheLights->GetSelectedLight()].param1.y += 0.1f;      // 0.1 degree
            //}

            //// quadratic attenuation 
            //if (key == GLFW_KEY_9)
            //{
            //    ::g_drawDebugLightSpheres = true;
            //}
            //if (key == GLFW_KEY_0)
            //{
            //    ::g_drawDebugLightSpheres = false;
            //}

            //// Select another light
            //if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
            //{
            //    ::g_pTheLights->SelectNextLight();
            //    g_camera->LookAtTarget(::g_pTheLights->theLights[g_pTheLights->GetSelectedLight()].position);
            //}
            //if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
            //{
            //    ::g_pTheLights->SelectPreviousLight();
            //    g_camera->LookAtTarget(::g_pTheLights->theLights[g_pTheLights->GetSelectedLight()].position);
            //}
        }//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)

        // camera control
        if (mods == 0)
        {
            // on pressing
            //if (key == GLFW_KEY_A && action == GLFW_PRESS)
            //{
            //    if (glfwGetKey(window, GLFW_KEY_W))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, 0.5f));
            //    }
            //    else if (glfwGetKey(window, GLFW_KEY_S))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, -0.5f));
            //    }
            //    else
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(1.0f, 0.0f, 0.0f));
            //    }
            //}

            //if (key == GLFW_KEY_D && action == GLFW_PRESS)
            //{
            //    if (glfwGetKey(window, GLFW_KEY_W))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(-0.5f, 0.0f, 0.5f));
            //    }
            //    else if (glfwGetKey(window, GLFW_KEY_S))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(-0.5f, 0.0f, -0.5f));
            //    }
            //    else
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
            //    }
            //}

            //if (key == GLFW_KEY_W && action == GLFW_PRESS)
            //{
            //    //if (glfwGetKey(window, GLFW_KEY_A))
            //    //{
            //    //    ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, 0.5f));
            //    //}
            //    //else if (glfwGetKey(window, GLFW_KEY_D))
            //    //{
            //    //    ::g_camera->MoveToDirection(glm::vec3(-0.5f, 0.0f, 0.5f));
            //    //}
            //    //else
            //    //{
            //    //    ::g_camera->MoveToDirection(glm::vec3(0.0f, 0.0f, 1.0f));
            //    //}
            //}

            //if (key == GLFW_KEY_S && action == GLFW_PRESS)
            //{
            //    if (glfwGetKey(window, GLFW_KEY_A))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, -0.5f));
            //    }
            //    else if (glfwGetKey(window, GLFW_KEY_D))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, -0.5f));
            //    }
            //    else
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.0f, 0.0f, -1.0f));
            //    }
            //}

            if (key == GLFW_KEY_Q && action == GLFW_PRESS)
            {
                g_camera->MoveCameraUp();
            }

            if (key == GLFW_KEY_E && action == GLFW_PRESS)
            {
                g_camera->MoveCameraDown();
            }

            // on releasing
            if (key == GLFW_KEY_A && action == GLFW_RELEASE)
            {
                ::g_camera->StopMovingInADirection(glm::vec3(1.0f, 0.0f, 0.0f));
            }

            if (key == GLFW_KEY_D && action == GLFW_RELEASE)
            {
                ::g_camera->StopMovingInADirection(glm::vec3(-1.0f, 0.0f, 0.0f));
            }

            if (key == GLFW_KEY_W && action == GLFW_RELEASE)
            {
                ::g_camera->StopMovingInADirection(glm::vec3(0.0f, 0.0f, 1.0f));
            }

            if (key == GLFW_KEY_S && action == GLFW_RELEASE)
            {
                ::g_camera->StopMovingInADirection(glm::vec3(0.0f, 0.0f, -1.0f));
            }

            if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
            {
                g_camera->StopCameraUp();
            }

            if (key == GLFW_KEY_E && action == GLFW_RELEASE)
            {
                g_camera->StopCameraDown();
            }
        }

        // object control
        //if (mods == GLFW_MOD_SHIFT)
        //{
        //    g_camera->LookAtTarget(g_objectSelected->drawPosition);

        //    //if (key == GLFW_KEY_W)
        //    //{
        //    //    ::g_objectSelected->drawPosition.y += OBJECT_MOVEMENT_SPEED;
        //    //    g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    //}
        //    //if (key == GLFW_KEY_D)
        //    //{
        //    //    ::g_objectSelected->drawPosition.x += OBJECT_MOVEMENT_SPEED;
        //    //    g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    //}

        //    //if (key == GLFW_KEY_S)
        //    //{
        //    //    ::g_objectSelected->drawPosition.y -= OBJECT_MOVEMENT_SPEED;
        //    //    g_camera->LookAtTarget(g_objectSelected->drawPosition);

        //    //}
        //    //if (key == GLFW_KEY_A)
        //    //{
        //    //    ::g_objectSelected->drawPosition.x -= OBJECT_MOVEMENT_SPEED;
        //    //    g_camera->LookAtTarget(g_objectSelected->drawPosition);

        //    //}

        //    //if (key == GLFW_KEY_Q)
        //    //{
        //    //    ::g_objectSelected->drawPosition.z += OBJECT_MOVEMENT_SPEED;
        //    //    g_camera->LookAtTarget(g_objectSelected->drawPosition);

        //    //}
        //    //if (key == GLFW_KEY_E)
        //    //{
        //    //    ::g_objectSelected->drawPosition.z -= OBJECT_MOVEMENT_SPEED;
        //    //    g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    //}

        //    if (key == GLFW_KEY_Z)
        //    {
        //        ::g_objectSelected->drawOrientation.y += OBJECT_ROTATION_SPEED;
        //        ::g_objectSelected->setRotationFromEuler(::g_objectSelected->drawOrientation);
        //    }
        //    if (key == GLFW_KEY_C)
        //    {
        //        ::g_objectSelected->drawOrientation.y -= OBJECT_ROTATION_SPEED;
        //        ::g_objectSelected->setRotationFromEuler(::g_objectSelected->drawOrientation);
        //    }

        //    if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
        //    {
        //        g_currentScene->SelectNextMesh();
        //        g_camera->LookAtTarget(g_objectSelected->drawPosition);

        //    }
        //    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
        //    {
        //        g_currentScene->SelectPreviousMesh();



        //        g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    }
        //}

        break;

    case InputMode::GameMode:

        // camera control
        if (mods == 0)
        {
            //// on pressing
            //if (key == GLFW_KEY_A && action == GLFW_PRESS)
            //{
            //    if (glfwGetKey(window, GLFW_KEY_W))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, 0.5f));
            //    }
            //    else if (glfwGetKey(window, GLFW_KEY_S))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, -0.5f));
            //    }
            //    else
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(1.0f, 0.0f, 0.0f));
            //    }
            //}

            //if (key == GLFW_KEY_D && action == GLFW_PRESS)
            //{
            //    if (glfwGetKey(window, GLFW_KEY_W))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(-0.5f, 0.0f, 0.5f));
            //    }
            //    else if (glfwGetKey(window, GLFW_KEY_S))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(-0.5f, 0.0f, -0.5f));
            //    }
            //    else
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
            //    }
            //}

            //if (key == GLFW_KEY_W && action == GLFW_PRESS)
            //{
            //    if (glfwGetKey(window, GLFW_KEY_A))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, 0.5f));
            //    }
            //    else if (glfwGetKey(window, GLFW_KEY_D))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(-0.5f, 0.0f, 0.5f));
            //    }
            //    else
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.0f, 0.0f, 1.0f));
            //    }
            //}

            //if (key == GLFW_KEY_S && action == GLFW_PRESS)
            //{
            //    if (glfwGetKey(window, GLFW_KEY_A))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, -0.5f));
            //    }
            //    else if (glfwGetKey(window, GLFW_KEY_D))
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.5f, 0.0f, -0.5f));
            //    }
            //    else
            //    {
            //        ::g_camera->MoveToDirection(glm::vec3(0.0f, 0.0f, -1.0f));
            //    }
            //}

            //if (key == GLFW_KEY_Q)
            //{
            //    //::g_camera

            //    ::g_camera->SetCameraOrientation(glm::rotate(::g_camera->GetCameraOrientation(), CAMERA_ROTATION_SPEED, g_camera->GetCameraUp()));
            //}

            //if (key == GLFW_KEY_E)
            //{
            //    ::g_camera->SetCameraOrientation(glm::rotate(::g_camera->GetCameraOrientation(), -CAMERA_ROTATION_SPEED, g_camera->GetCameraUp()));
            //}

            // on releasing
            if (key == GLFW_KEY_A && action == GLFW_RELEASE)
            {
                ::g_camera->StopMovingInADirection(glm::vec3(1.0f,0.0f,0.0f));
            }

            if (key == GLFW_KEY_D && action == GLFW_RELEASE)
            {
                ::g_camera->StopMovingInADirection(glm::vec3(-1.0f, 0.0f, 0.0f));
            }

            if (key == GLFW_KEY_W && action == GLFW_RELEASE)
            {
                ::g_camera->StopMovingInADirection(glm::vec3(0.0f, 0.0f, 1.0f));
            }

            if (key == GLFW_KEY_S && action == GLFW_RELEASE)
            {
                ::g_camera->StopMovingInADirection(glm::vec3(0.0f, 0.0f, -1.0f));
            }
        }


        break;
    }

    return;
}

void cInputHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_1:
        //std::cout << "GLFW_MOUSE_BUTTON_1" << std::endl;
        break;
    case GLFW_MOUSE_BUTTON_2:
        //std::cout << "GLFW_MOUSE_BUTTON_2" << std::endl;
        break;
    case GLFW_MOUSE_BUTTON_3:
        //std::cout << "GLFW_MOUSE_BUTTON_3" << std::endl;
        break;
    }
    return;
}

void cInputHandler::HandleInput() const
{
    if (g_currentScene->isScenePlaying)
    {
        iSystem* characterControllerSystem = g_currentScene->GetSystem("CharacterController");
        CharacterController* characterController = static_cast<CharacterController*> (characterControllerSystem);

        if (characterController)
        {
            if (m_Keys[340]) // shift
            {
                characterController->isPressingShift = true;
            }
            else
            {
                characterController->isPressingShift = false;
            }


            // horizontal
            if ((m_Keys['a'] || m_Keys['A']) && (m_Keys['d'] || m_Keys['D']))
            {
                characterController->horizontalMove = 0.0f;

                g_camera->RemoveVelocityFromCamera('a');
                g_camera->RemoveVelocityFromCamera('d');
            }
            else if (m_Keys['a'] || m_Keys['A'])
            {
                characterController->horizontalMove = -1.0f;

                g_camera->AddVelocityToCamera('a', glm::vec3(1.0f, 0.0f, 0.0f));
                g_camera->RemoveVelocityFromCamera('d');
            }
            else if (m_Keys['d'] || m_Keys['D'])
            {
                characterController->horizontalMove = +1.0f;

                g_camera->AddVelocityToCamera('d', glm::vec3(-1.0f, 0.0f, 0.0f));
                g_camera->RemoveVelocityFromCamera('a');
            }
            else
            {
                characterController->horizontalMove = 0.0f;

                g_camera->RemoveVelocityFromCamera('a');
                g_camera->RemoveVelocityFromCamera('d');
            }

            // vertical
            if ((m_Keys['w'] || m_Keys['W']) && (m_Keys['s'] || m_Keys['S']))
            {
                characterController->verticalMove = 0.0f;
                //lionPlayer->SetVerticalMovement(0.0f);
                g_camera->RemoveVelocityFromCamera('w');
                g_camera->RemoveVelocityFromCamera('s');
            }
            else if (m_Keys['w'] || m_Keys['W'])
            {
                characterController->verticalMove = -1.0f;

                g_camera->AddVelocityToCamera('w', glm::vec3(0.0f, 0.0f, 1.0f));
                g_camera->RemoveVelocityFromCamera('s');

            }
            else if (m_Keys['s'] || m_Keys['S'])
            {
                characterController->verticalMove = 1.0f;

                g_camera->AddVelocityToCamera('s', glm::vec3(0.0f, 0.0f, -1.0f));
                g_camera->RemoveVelocityFromCamera('w');
            }
            else
            {
                characterController->verticalMove = 0.0f;

                g_camera->RemoveVelocityFromCamera('w');
                g_camera->RemoveVelocityFromCamera('s');
            }
        }
    }

    for (unsigned int i = 0; i < NUMBER_OF_KEYS; i++)
    {
        if (m_Keys[i] == true)
        {
            WhilePressed(i);
        }
    }

    for (unsigned int i = 0; i < NUMBER_OF_MODS; i++)
    {
        if (m_Modes[i])
        {
            WhilePressed(m_Modes[i]);
        }
    }

    if (currentMode == InputMode::EditorMode)
    {
        // ESCAPE
        if (m_Keys['\x1B'])
        {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
        }

        // CameraMovement FRONT, BACK, LEFT, RIGHT 
        //if ((m_Keys[GLFW_KEY_LEFT_SHIFT] == false)
        // && (m_Keys[GLFW_KEY_LEFT_CONTROL] == false)
        // && (m_Keys[GLFW_KEY_LEFT_ALT] == false))
        //{
        //    if (m_Keys['w'] || m_Keys['W'])
        //    {
        //        g_camera->AddVelocityToCamera('w', glm::vec3(0.0f, 0.0f, 1.0f));
        //    }
        //    else
        //    {
        //        g_camera->RemoveVelocityFromCamera('w');
        //    }
        //    if (m_Keys['a'] || m_Keys['A'])
        //    {
        //        g_camera->AddVelocityToCamera('a', glm::vec3(1.0f, 0.0f, 0.0f));
        //    }
        //    else
        //    {
        //        g_camera->RemoveVelocityFromCamera('a');
        //    }
        //    if (m_Keys['s'] || m_Keys['S'])
        //    {
        //        g_camera->AddVelocityToCamera('s', glm::vec3(0.0f, 0.0f, -1.0f));
        //    }
        //    else
        //    {
        //        g_camera->RemoveVelocityFromCamera('s');
        //    }
        //    if (m_Keys['d'] || m_Keys['D'])
        //    {
        //        g_camera->AddVelocityToCamera('d', glm::vec3(-1.0f, 0.0f, 0.0f));
        //    }
        //    else
        //    {
        //        g_camera->RemoveVelocityFromCamera('d');
        //    }

        //    // CameraRotation UP and DOWN
        //    if (m_Keys['q'] || m_Keys['Q'])
        //    {
        //        g_camera->AddVelocityToCamera('q', glm::vec3(0.0f, 1.0f, 0.0f));
        //    }
        //    else
        //    {
        //        g_camera->RemoveVelocityFromCamera('q');
        //    }
        //    if (m_Keys['e'] || m_Keys['E'])
        //    {
        //        g_camera->AddVelocityToCamera('e', glm::vec3(0.0f, -1.0f, 0.0f));
        //    }
        //    else
        //    {
        //        g_camera->RemoveVelocityFromCamera('e');
        //    }
        //}

        // Object selected movement
        //if ((m_Keys[GLFW_KEY_LEFT_SHIFT] == true)
        // && (m_Keys[GLFW_KEY_LEFT_CONTROL] == false)
        // && (m_Keys[GLFW_KEY_LEFT_ALT] == false))
        //{
        //    g_camera->LookAtTarget(g_objectSelected->drawPosition);

        //    if (m_Keys['w'] || m_Keys['W'])
        //    {
        //        ::g_objectSelected->drawPosition.y += OBJECT_MOVEMENT_SPEED;
        //        g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    }
        //    if (m_Keys['d'] || m_Keys['D'])
        //    {
        //        ::g_objectSelected->drawPosition.x += OBJECT_MOVEMENT_SPEED;
        //        g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    }

        //    if (m_Keys['s'] || m_Keys['S'])
        //    {
        //        ::g_objectSelected->drawPosition.y -= OBJECT_MOVEMENT_SPEED;
        //        g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    }
        //    if (m_Keys['a'] || m_Keys['A'])
        //    {
        //        ::g_objectSelected->drawPosition.x -= OBJECT_MOVEMENT_SPEED;
        //        g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    }

        //    if (m_Keys['q'] || m_Keys['Q'])
        //    {
        //        ::g_objectSelected->drawPosition.z += OBJECT_MOVEMENT_SPEED;
        //        g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    }
        //    if (m_Keys['e'] || m_Keys['E'])
        //    {
        //        ::g_objectSelected->drawPosition.z -= OBJECT_MOVEMENT_SPEED;
        //        g_camera->LookAtTarget(g_objectSelected->drawPosition);
        //    }

        //    if (m_Keys['z'] || m_Keys['Z'])
        //    {
        //        ::g_objectSelected->drawOrientation.y += OBJECT_ROTATION_SPEED;
        //        ::g_objectSelected->setRotationFromEuler(::g_objectSelected->drawOrientation);
        //    }
        //    if (m_Keys['c'] || m_Keys['C'])
        //    {
        //        ::g_objectSelected->drawOrientation.y -= OBJECT_ROTATION_SPEED;
        //        ::g_objectSelected->setRotationFromEuler(::g_objectSelected->drawOrientation);
        //    }

        //    //    if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
        //    //    {
        //    //        bool isValidMesh = false;

        //    //        while (!isValidMesh)
        //    //        {
        //    //            ::g_selectedMesh++;

        //    //            if (::g_selectedMesh >= ::g_vec_pMeshesToDraw.size())
        //    //            {
        //    //                ::g_selectedMesh = 0;
        //    //            }

        //    //            if (::g_vec_pMeshesToDraw[::g_selectedMesh]->objectType != "tree")
        //    //            {
        //    //                std::cout << "Selected model: " << ::g_selectedMesh << std::endl;
        //    //                ::g_objectSelected = ::g_vec_pMeshesToDraw[::g_selectedMesh];
        //    //                isValidMesh = true;
        //    //            }
        //    //        }

        //    //        g_camera->LookAtTarget(g_objectSelected->drawPosition);

        //    //    }
        //    //    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
        //    //    {
        //    //        bool isValidMesh = false;

        //    //        while (!isValidMesh)
        //    //        {
        //    //            ::g_selectedMesh--;

        //    //            if (::g_selectedMesh < 0)//>= ::g_vec_pMeshesToDraw.size())
        //    //            {
        //    //                ::g_selectedMesh = (int)::g_vec_pMeshesToDraw.size() - 1;
        //    //            }

        //    //            if (::g_vec_pMeshesToDraw[::g_selectedMesh]->objectType != "tree")
        //    //            {
        //    //                ::g_objectSelected = ::g_vec_pMeshesToDraw[::g_selectedMesh];
        //    //                std::cout << "Selected model: " << ::g_selectedMesh << " Friendly name: " << ::g_objectSelected->friendlyName << std::endl;
        //    //                ::g_objectSelected = ::g_vec_pMeshesToDraw[::g_selectedMesh];
        //    //                isValidMesh = true;
        //    //            }
        //    //        }

        //    //        g_camera->LookAtTarget(g_objectSelected->drawPosition);

        //    //}
        //}

    }
    else if (currentMode == InputMode::GameMode)
    {
        if (m_Keys['w'] || m_Keys['W'])
        {
            glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);

            float leftDirection = direction.x;
            float forwardDirection = direction.z;

            glm::vec3 newPosition = g_camera->GetPosition();

            newPosition += g_camera->GetOrientation() * g_camera->GetLeft() * g_camera->GetCameraSpeed() * leftDirection;
            newPosition += g_camera->GetOrientation() * g_camera->GetForward() * g_camera->GetCameraSpeed() * forwardDirection;
            glm::vec3 normalizedDirection = glm::normalize(newPosition - g_camera->GetPosition());

            g_camera->AddVelocityToCamera('s', normalizedDirection * g_camera->GetPosition());
        }

        if (m_Keys['s'] || m_Keys['S'])
        {
            glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);

            float leftDirection = direction.x;
            float forwardDirection = direction.z;

            glm::vec3 newPosition = g_camera->GetPosition();

            newPosition += g_camera->GetOrientation() * g_camera->GetLeft() * g_camera->GetCameraSpeed() * leftDirection;
            newPosition += g_camera->GetOrientation() * g_camera->GetForward() * g_camera->GetCameraSpeed() * forwardDirection;
            glm::vec3 normalizedDirection = glm::normalize(newPosition - g_camera->GetPosition());

            g_camera->AddVelocityToCamera('s', normalizedDirection * g_camera->GetPosition());
        }
        if (m_Keys['s'] == false)
        {
            g_camera->RemoveVelocityFromCamera('s');
        }
        //if (m_Keys['a'] || m_Keys['A'])
        //{
        //    m_PlayerData.transform.position +=
        //        moveSpeed * m_PlayerData.transform.orientation * LEFT_VECTOR * m_DeltaTime;
        //}
        //if (m_Keys['d'] || m_Keys['D'])
        //{
        //    m_PlayerData.transform.position -=
        //        moveSpeed * m_PlayerData.transform.orientation * LEFT_VECTOR * m_DeltaTime;
        //}
    }
}

void cInputHandler::OnPress(int key)
{
    //float horizontalMovement = 0.0f;

    //if (key == 'A' || key == 'a')
    //{
    //    std::cout << "Is pressing A" << std::endl;

    //    horizontalMovement -= 1.0f;
    //}

    //if (key == 'D' || key == 'd')
    //{
    //    std::cout << "Is pressing D" << std::endl;

    //    horizontalMovement += 1.0f;
    //}

    //if ((horizontalMovement < 0.5f) && (horizontalMovement > -0.5f))
    //{
    //    horizontalMovement = 0.0f;
    //}


    //iSystem* characterControllerSystem = g_currentScene->GetSystem("CharacterController");
    //CharacterController* characterController = static_cast<CharacterController*> (characterControllerSystem);

    //characterController->horizontalMove = horizontalMovement;


    if ((key == 1) && bUseFlyCamera) // shift
    {
        ::g_pFlyCamera->SetCameraSpeed(100.0f);
        //g_softBody->acceleration.z = 0.1f;
    }

    if ((key == '2') && bUseFlyCamera) // shift
    {
        //if (screensAreOn)
        //{
        //    std::vector <cMesh*> cornerScreens = cGameManager::GetGMInstance()->FindAllMeshesByObjectType("cornerscreen");

        //    for (cMesh* screen : cornerScreens)
        //    {
        //        screen->textureIsFromFBO = false;
        //        screen->bUseDebugColours = true;
        //    }

        //    screensAreOn = false;
        //}
        //else
        //{
        //    std::vector <cMesh*> cornerScreens = cGameManager::GetGMInstance()->FindAllMeshesByObjectType("cornerscreen");

        //    for (cMesh* screen : cornerScreens)
        //    {
        //        screen->textureIsFromFBO = true;
        //        screen->bUseDebugColours = false;
        //    }

        //    screensAreOn = true;
        //}
    }


    //Gems midterm
    //if (key == '1')
    //{
    //    std::string animationName = "Captain_Idle.dae";

    //    g_animationSystem->SetAnimationByName(animationName);
    //    std::cout << "Setting animation to: " << animationName << std::endl;

    //    //std::cout << "pressed 1" << std::endl;
    //    //g_softBody->DisconnectRandomHole();
    //    //::g_anEmitter.Explode(1000, 10.0f, 15.0f);
    //    //g_softBody->acceleration.z = 0.7f;
    //}

    //if (key == '2')
    //{
    //    std::string animationName = "Captain_Walking.dae";

    //    g_animationSystem->SetAnimationByName(animationName);
    //    std::cout << "Setting animation to: " << animationName << std::endl;

    //    //g_softBody->acceleration.z = 0.0f;

    //    //std::cout << "pressed 2" << std::endl;

    //    //g_softBody->ActivateAllParticles();

    //    //g_softBody->acceleration.y = -0.5;
    //    //g_softBody->acceleration.z = -0.2;
    //    //g_softBody->isFlagConnected = false;
    //    //::g_anEmitter.Explode(1000, 10.0f, 15.0f);
    //}

    //if (key == '3')
    //{
    //    std::string animationName = "Captain_Running.dae";

    //    g_animationSystem->SetAnimationByName(animationName);
    //    std::cout << "Setting animation to: " << animationName << std::endl;

    //    //std::cout << "pressed 3" << std::endl;
    //    //g_softBody->ResetFlagPosition();
    //    //::g_anEmitter.Explode(1000, 10.0f, 15.0f);
    //}

    if ((key == ' ') && (currentMode == InputMode::EditorMode))
    {
        if (cGameManager::GetGMInstance()->showUi)
        {
            //if (g_camera == cameras[0])
            //{
            //    g_camera = cameras[1];
            //}

            //bUseFlyCamera = false;
            //cGameManager::GetGMInstance()->showUi = false;
            //g_animationSystem->StartAllAnimations(g_gameObjects);
            g_currentScene->isScenePlaying = true;
            g_animationSystem->m_isAnimating = true;
            //g_camera->MoveToDirection(glm::vec3(0.0f, 0.0f, 1.0f));
        }
        else
        {
            g_camera->StopMovingInADirection(glm::vec3(0.0f, 0.0f, 1.0f));
            cGameManager::GetGMInstance()->showUi = true;
            //g_animationSystem->StopAllAnimations(g_gameObjects);
            g_currentScene->isScenePlaying = false;

            for (unsigned int i = 0; i < g_currentScene->m_scenePhysics->m_vec_pPhysicalProps.size(); i++)
            {
                g_currentScene->m_scenePhysics->m_vec_pPhysicalProps[i]->velocity = glm::vec3(0.0f);
            }
        }

        //currentMode = InputMode::GameMode;
        //g_camera->ResetCameraInitialConfig();
    } 
    else if ((key == ' ') && (currentMode == InputMode::GameMode))
    {
        currentMode = InputMode::EditorMode;
        g_camera->ResetCameraInitialConfig();
    }

    //if (key == GLFW_KEY_1)
    //{
    //    g_animationSystem->SetAnimationSpeed(1.0f);
    //}

    //if (key == GLFW_KEY_2)
    //{
    //    g_animationSystem->SetAnimationSpeed(2.0f);
    //}

    //if (key == GLFW_KEY_3)
    //{
    //    g_animationSystem->SetAnimationSpeed(3.0f);
    //}

    //if (key == GLFW_KEY_4)
    //{
    //    g_animationSystem->SetAnimationSpeed(4.0f);
    //}

    //if (key == GLFW_KEY_5)
    //{
    //    g_animationSystem->SetAnimationSpeed(5.0f);
    //}


    //if (key == GLFW_KEY_RIGHT)
    //{
    //    if (g_camera == g_currentScene->GetCameras()[2])
    //    {
    //        g_camera = g_currentScene->GetCameras()[1];
    //    }
    //    else
    //    {
    //        g_camera = g_currentScene->GetCameras()[2];
    //    }
    //}

    //if (key == GLFW_KEY_LEFT)
    //{
    //    if (g_camera == g_currentScene->GetCameras()[2])
    //    {
    //        g_camera = g_currentScene->GetCameras()[1];
    //    }
    //    else
    //    {
    //        g_camera = g_currentScene->GetCameras()[2];
    //    }
    //}

    if (key == GLFW_KEY_R)
    {
        //g_animationSystem->ReverseAnimations(g_currentScene->m_gameObjects);
    }

    if (currentMode == InputMode::EditorMode)
    {
        if (key == 294) //F5
        {
            std::cout << "saving scene..." << std::endl;
            sCameraInfo cameraInfo;
            cameraInfo.Up = g_camera->GetUp();
            cameraInfo.Position = g_camera->GetPosition();
            cameraInfo.NearClip = g_camera->GetNearClip();
            cameraInfo.FarClip = g_camera->GetFarClip();
            cameraInfo.Forward = g_camera->GetForward();

            std::vector<sLightsInfo> lightsInfo = g_currentScene->m_sceneLights->SerializeLight();
            std::vector<sModel> sceneModels; 
            std::vector<cMesh* > meshesToDraw = g_currentScene->GetAllMeshes();
            for (cMesh* mesh : meshesToDraw)
            {
                sModel model;

                model.Alpha = mesh->transparencyAlpha;
                model.DebugColourRGBA = mesh->wholeObjectDebugColourRGBA;
                model.DrawScale = mesh->drawScale;
                model.FriendlyName = mesh->friendlyName;
                model.IsUsingDebugColors = mesh->bUseDebugColours;
                model.IsUsingBones = false;
                model.MeshName = mesh->meshName;
                model.ObjectType = mesh->objectType;

                for (unsigned int j = 0; j < mesh->NUM_TEXTURES; j++)
                {
                    sTextureInfo textureInfo;

                    textureInfo.Name = mesh->textureName[j];
                    textureInfo.Ratio = mesh->textureRatios[j];
                    
                    model.ObjectTextures.push_back(textureInfo);
                }

                sceneModels.push_back(model);
            }

            cJsonHandler jsonReaderWriter;
            jsonReaderWriter.SaveScene(cameraInfo, sceneModels, lightsInfo);
            //sonReaderWriter.WriteCameraConfig("docs/CameraInfo.json", cameraInfo);
        }
    }
}

void cInputHandler::WhilePressed(int key) const
{
    const float cameraMovementSpeed = 0.5f;

    if (bUseFlyCamera)
    {
        if (m_Modes[GLFW_MOD_SHIFT])
        {
            ::g_pFlyCamera->UseFastCamera();
        }
        else
        {
            g_pFlyCamera->UseSlowCamera();
        }

        if (m_Keys['a'] || m_Keys['A']) 
        { 
            ::g_pFlyCamera->MoveLeftRight_X(-cameraMovementSpeed); 
        }
        if (m_Keys['d'] || m_Keys['D'])
        {
            ::g_pFlyCamera->MoveLeftRight_X(+cameraMovementSpeed);
        }

        if (m_Keys['w'] || m_Keys['W'])
        {
            ::g_pFlyCamera->MoveForward_Z(+cameraMovementSpeed);
        }
        if (m_Keys['s'] || m_Keys['S'])
        {
            ::g_pFlyCamera->MoveForward_Z(-cameraMovementSpeed);
        }

        if (m_Keys['q'] || m_Keys['Q'])
        {
            ::g_pFlyCamera->MoveUpDown_Y(-cameraMovementSpeed);
        }
        if (m_Keys['e'] || m_Keys['E'])
        {
            ::g_pFlyCamera->MoveUpDown_Y(+cameraMovementSpeed);
        }
    }

    if (key == '1')
    {
        game.StartCutscene();
    }

    if (key == '2')
    {
        game.StopCutscene();
    }
}

void cInputHandler::OnRelease(int key)
{
    if (key == 'a' || key == 'A')
    {
        ::g_pFlyCamera->StopMovingInADirection('l');
    }
    if (key == 'd' || key == 'D')
    {
        ::g_pFlyCamera->StopMovingInADirection('r');
    }

    if (key == 'w' || key == 'W')
    {
        ::g_pFlyCamera->StopMovingInADirection('f');
    }
    if (key == 's' || key == 'S')
    {
        ::g_pFlyCamera->StopMovingInADirection('b');
    }

    if (m_Keys['q'] || m_Keys['Q'])
    {
        ::g_pFlyCamera->StopMovingInADirection('d');
    }
    if (m_Keys['e'] || m_Keys['E'])
    {
        ::g_pFlyCamera->StopMovingInADirection('u');
    }

    //float horizontalMovement = 0.0f;

    //if (key == 'A' || key == 'a')
    //{
    //    std::cout << "Is releasing A" << std::endl;

    //    horizontalMovement += 1.0f;
    //}

    //iSystem* characterControllerSystem = g_currentScene->GetSystem("CharacterController");
    //CharacterController* characterController = static_cast<CharacterController*> (characterControllerSystem);

    //characterController->horizontalMove -= horizontalMovement;

    //if (key == '1')
    //{
    //    g_softBody->acceleration.z = 0.0f;
    //}
}

void cInputHandler::SetObjectMovementSpeed(float objectSpeed)
{

}

void cInputHandler::SetObjectRotationSpeed(float objectRotationSpeed)
{

}
