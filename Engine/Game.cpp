#include "Game.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "cColor.h"
#include "Engine.h"
#include "Basic Shader Manager/cShaderManager.h"
#include "cLuaManager.h"
#include "cMoveRelativeTime.h"
#include "cOrientTo.h"
#include "cCamera.h"

extern Engine engine;
extern cScene* g_currentScene;
extern iCamera* g_camera;
extern GLuint shaderProgramID;
extern cShaderManager* shaderManager;
extern bool bUseFlyCamera;

cLuaManager* luaManager = nullptr;

void Game::RunLuaScriptOnObjects(double deltaTime)
{
	for (int i = 0; i < luaHandledObjects.size(); i++)
	{
		if (luaHandledObjects[i])
		{
			luaHandledObjects[i]->m_LuaManager->Update(deltaTime);
		}
	}

	return;
}

void Game::CreateBeholder()
{
	cMesh* beholderMesh = g_currentScene->GetMeshByFriendlyName("beholder");
	beholderMesh->bIsVisible = true;
	beholder = new cGameObject(beholderMesh, true);


	beholder->m_objectPhysics = sPhysicsProperties::CreateSphere(10.0f);
	beholder->m_objectPhysics->SetRotation(beholderMesh->GetOrientation());

	beholder->m_objectPhysics->pTheAssociatedMesh = beholderMesh;
	beholder->m_objectPhysics->position = beholderMesh->drawPosition;

	beholder->m_objectPhysics->friendlyName = "beholder";
	beholder->m_objectPhysics->objectType = beholderMesh->objectType;
	beholder->m_objectPhysics->maxVelocity = 10000.0f;

	g_currentScene->m_scenePhysics->AddShape(beholder->m_objectPhysics);

	luaHandledObjects.push_back(beholder);
	g_currentScene->AddGameObject(beholder);

	beholder->CreateLuaManager(&luaHandledObjects);
}

void Game::CreateCamera()
{
	cMesh* cameraMesh = g_currentScene->GetMeshByFriendlyName("camera");
	cameraMesh->bIsVisible = true;
	camera = new cGameObject(cameraMesh, true);

	camera->m_objectPhysics = sPhysicsProperties::CreateSphere(1.0f);
	camera->m_objectPhysics->position = cameraMesh->drawPosition;
	camera->m_objectPhysics->pTheAssociatedMesh = cameraMesh;
	camera->m_objectPhysics->friendlyName = "camera";
	camera->m_objectPhysics->maxVelocity = 10000.0f;

	g_currentScene->m_scenePhysics->AddShape(camera->m_objectPhysics);

	luaHandledObjects.push_back(camera);
	g_currentScene->AddGameObject(camera);

	camera->CreateLuaManager(&luaHandledObjects);
}

void Game::CreateRubble(std::string rubbleName)
{
	cMesh* rubbleMesh = g_currentScene->GetMeshByFriendlyName(rubbleName);
	rubbleMesh->bIsVisible = true;
	cGameObject* rubble = new cGameObject(rubbleMesh, true);

	rubble->m_objectPhysics = sPhysicsProperties::CreateSphere(1.0f);
	rubble->m_objectPhysics->position = rubbleMesh->drawPosition;
	rubble->m_objectPhysics->pTheAssociatedMesh = rubbleMesh;
	rubble->m_objectPhysics->friendlyName = rubbleName;
	rubble->m_objectPhysics->maxVelocity = 10000.0f;
	rubble->m_objectPhysics->SetRotation(rubbleMesh->GetOrientation());

	g_currentScene->m_scenePhysics->AddShape(rubble->m_objectPhysics);
	luaHandledObjects.push_back(rubble);
	g_currentScene->AddGameObject(rubble);

	rubble->CreateLuaManager(&luaHandledObjects);
}

void Game::CreateTorchlight(std::string torch)
{
	cMesh* torchMesh = g_currentScene->GetMeshByFriendlyName(torch);
	torchMesh->bIsVisible = true;
	cGameObject* torchlight = new cGameObject(torchMesh, true);

	torchlight->m_objectPhysics = sPhysicsProperties::CreateSphere(1.0f);
	torchlight->m_objectPhysics->position = torchMesh->drawPosition;
	torchlight->m_objectPhysics->pTheAssociatedMesh = torchMesh;
	torchlight->m_objectPhysics->friendlyName = torch;
	torchlight->m_objectPhysics->maxVelocity = 10000.0f;
	torchlight->m_objectPhysics->SetRotation(torchMesh->GetOrientation());

	g_currentScene->m_scenePhysics->AddShape(torchlight->m_objectPhysics);
	luaHandledObjects.push_back(torchlight);
	g_currentScene->AddGameObject(torchlight);

	torchlight->CreateLuaManager(&luaHandledObjects);
}

void Game::StartCutscene()
{
	if (m_IsGameRunning)
	{
		return;
	}

	m_IsGameRunning = true;

	iCamera* cameraComponent = g_currentScene->GetCameras()[1];

	cameraComponent->SetPosition(camera->GetObjectPhysics()->position);

	bUseFlyCamera = false;
	g_camera = cameraComponent;

	g_currentScene->m_commandManager = new cCommandManager();
	g_currentScene->m_commandFactory = new cCommandFactory();
	g_currentScene->m_commandFactory->SetPhysics(g_currentScene->m_scenePhysics);

	// Load Lua file
	std::string luaFileName = "LuaScripts/Cutscene.lua";

	std::ifstream luaFile(luaFileName);

	if (!luaFile.is_open())
	{
		std::cout << luaFileName << " failed to load" << std::endl;
	}

	std::stringstream luaFileStringStream;

	std::vector<std::string> luaScriptComments;

	std::string lineString;
	unsigned int lineNumber = 0;
	while (std::getline(luaFile, lineString))
	{

		if (lineString[0] == '-' && lineString[1] == '-')
		{
			lineString = std::to_string(lineNumber) + lineString;
			luaScriptComments.push_back(lineString);
		}
		else
		{
			luaFileStringStream << lineString << "\n";
		}

		lineNumber++;
	}

	beholder->m_LuaManager->RunScript(luaFileStringStream.str());
		
	g_currentScene->m_commandManager->Start();
}

void Game::StopCutscene()
{
	g_currentScene->m_sceneLights->TurnOffLightByFriendlyName("beholder");
	g_currentScene->RestartTransforms();
	m_IsGameRunning = false;

	iCamera* cameraComponent = g_currentScene->GetCameras()[0];

	bUseFlyCamera = true;
	g_camera = cameraComponent;

	g_currentScene->m_commandManager = new cCommandManager();
}

void Game::LinkLightToBeholder()
{
	glm::vec3 beholderPosition = beholder->GetObjectMesh()->drawPosition;

	glm::vec4 beholderPositionVec4 = glm::vec4(beholderPosition.x, beholderPosition.y + 5.0f, beholderPosition.z, 0.0f);

	g_currentScene->m_sceneLights->SetLightPosition("beholder", beholderPositionVec4);

	glm::quat beholderOrientation = beholder->GetObjectMesh()->GetOrientation();

	glm::vec3 forward = beholderOrientation * glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec4 lightDirection = glm::normalize(glm::vec4(forward, 0.0f));
	g_currentScene->m_sceneLights->SetLightOrientation("beholder", lightDirection);
}

void Game::MoveBeholderToPoint(glm::vec3 point, double seconds, bool moveEaseIn, bool moveEaseOut)
{
	cMoveRelativeTime* beholderMove1 = new cMoveRelativeTime();
	beholderMove1->Init(beholder->GetObjectPhysics(), point, seconds, false, true);
	g_currentScene->m_commandGroup->addSerial(beholderMove1);
}

void Game::MoveBeholderToPointAndOrient(glm::vec3 point, double moveSeconds, bool moveEaseIn, bool moveEaseOut,
										glm::vec3 degrees, double orientSeconds, bool orientEaseIn, bool orientEaseOut)
{
	cCommandGroup* beholderMoveAndOrient = new cCommandGroup();

	cMoveRelativeTime* beholderMove = new cMoveRelativeTime();
	beholderMove->Init(beholder->GetObjectPhysics(), point, moveSeconds, moveEaseIn, moveEaseOut);

	cOrientTo* beholderOrient = new cOrientTo();
	beholderOrient->Init(beholder->GetObjectPhysics(), degrees, orientSeconds, orientEaseIn, orientEaseOut);

	beholderMoveAndOrient->addParallel(beholderMove);
	beholderMoveAndOrient->addParallel(beholderOrient);
	g_currentScene->m_commandGroup->addSerial(beholderMoveAndOrient);

}

Game::Game()
{

}

void Game::Awake(GLFWwindow& window)
{

}

void Game::Start(GLFWwindow& window)
{
	CreateBeholder();
	CreateCamera();
	CreateRubble("rubble1");
	CreateRubble("rubble2");
	CreateTorchlight("torch1");

	g_currentScene->m_commandGroup = new cCommandGroup();
	g_currentScene->m_commandManager = new cCommandManager();

	luaManager = new cLuaManager();

	luaManager->SetObjectVector(&(g_currentScene->m_gameObjects));

	return;
}

void Game::EarlyUpdate(GLFWwindow& window, double deltaTime)
{

}

void Game::Update(GLFWwindow& window, double deltaTime)
{
	if (m_IsGameRunning)
	{
		g_camera->SetPosition(camera->GetObjectMesh()->drawPosition);
		g_camera->SetOrientation(camera->GetObjectPhysics()->eulerOrientation);
	}

	LinkLightToBeholder();

	RunLuaScriptOnObjects(deltaTime);

	timeElapsed += deltaTime;

	if (g_currentScene->m_commandManager->AreAllCommandsFinished())
	{
		m_IsGameRunning = false;
	}

	// Waypoints to test
	//engine.g_DrawDebugSphere(glm::vec3(0.0f, 0.0f, 1000.f), 10.0f, cColor::COLOR_BLUE, 0.0);
	//engine.g_DrawDebugSphere(glm::vec3(1000.0f, 0.0f, 1000.f), 10.0f, cColor::COLOR_RED, 0.0);
	//engine.g_DrawDebugSphere(glm::vec3(1000.0f, 0.0f, 0.f), 10.0f, cColor::COLOR_WHITE, 0.0);
	//engine.g_DrawDebugSphere(glm::vec3(0.0f, 0.0f, 0.f), 10.0f, cColor::COLOR_GREEN, 0.0);

	// Debug
	//engine.g_DrawDebugSphere(glm::vec3(1000.0f, 0.0f, 1000.f), 800.0f, cColor::COLOR_GREEN, 0.0);
}

void Game::LateUpdate(GLFWwindow& window, double deltaTime)
{

}

void Game::End(GLFWwindow& window)
{

}

std::string Game::GetSystemType()
{
    return "Game";
}

// Lua functions

int LuaAddSerialCommand(lua_State* L)
{
	std::string friendlyName = lua_tostring(L, 1);

	cGameObject* pGO = cLuaManager::FindObjectByFriendlyName(friendlyName);

	if (pGO == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	std::vector<std::string> commandDetails;
	commandDetails.push_back(friendlyName);
	commandDetails.push_back(std::to_string(lua_tonumber(L, 3)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 4)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 5)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 6)));
	commandDetails.push_back("false");
	commandDetails.push_back("false");

	iCommand* luaCommand = g_currentScene->m_commandFactory->CreateCommandObject(lua_tostring(L, 2), commandDetails);

	if (g_currentScene->m_commandManager->m_IsCreatingCommandGroup)
	{
		g_currentScene->m_commandManager->m_CommandGroupBeingCreated->addSerial(luaCommand);
	}
	else
	{
		cCommandGroup* serialCommand = new cCommandGroup();
		serialCommand->addSerial(luaCommand);
		g_currentScene->m_commandManager->AddCommandGroup(serialCommand);
	}

	return 1;
}

int LuaCreateCommandGroup(lua_State* L)
{
	g_currentScene->m_commandManager->m_IsCreatingCommandGroup = true;
	g_currentScene->m_commandManager->m_CommandGroupBeingCreated = new cCommandGroup();
	return 1;
}

int LuaCompleteCommandGroup(lua_State* L)
{
	g_currentScene->m_commandManager->m_IsCreatingCommandGroup = false;
	g_currentScene->m_commandManager->AddCommandGroup(g_currentScene->m_commandManager->m_CommandGroupBeingCreated);
	return 1;
}

int LuaAddParallelCommand(lua_State* L)
{
	std::string friendlyName = lua_tostring(L, 1);

	cGameObject* pGO = cLuaManager::FindObjectByFriendlyName(friendlyName);

	if (pGO == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	std::vector<std::string> commandDetails;
	commandDetails.push_back(friendlyName);
	commandDetails.push_back(std::to_string(lua_tonumber(L, 3)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 4)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 5)));

	commandDetails.push_back(std::to_string(lua_tonumber(L, 6)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 7)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 8)));

	commandDetails.push_back(std::to_string(lua_tonumber(L, 9)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 10)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 11)));

	commandDetails.push_back(std::to_string(lua_tonumber(L, 12)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 13)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 14)));

	commandDetails.push_back(std::to_string(lua_tonumber(L, 15)));

	iCommand* luaCommand = g_currentScene->m_commandFactory->CreateCommandObject(lua_tostring(L, 2), commandDetails);

	if (g_currentScene->m_commandManager->m_IsCreatingCommandGroup)
	{
		g_currentScene->m_commandManager->m_CommandGroupBeingCreated->addParallel(luaCommand);
	}
	else
	{
		cCommandGroup* serialCommand = new cCommandGroup();
		serialCommand->addParallel(luaCommand);
		g_currentScene->m_commandManager->AddCommandGroup(serialCommand);
	}

	return 1;
}

int LuaAddLocationTriggerCommand(lua_State* L)
{
	std::string triggeredFriendlyName = lua_tostring(L, 1);
	std::string commandedFriendlyName = lua_tostring(L, 2);

	cGameObject* triggeredObject = cLuaManager::FindObjectByFriendlyName(triggeredFriendlyName);
	cGameObject* commandedObject = cLuaManager::FindObjectByFriendlyName(commandedFriendlyName);

	if (triggeredObject == nullptr || commandedObject == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	std::vector<std::string> commandDetails;
	commandDetails.push_back(triggeredFriendlyName);
	commandDetails.push_back(commandedFriendlyName);

	commandDetails.push_back(std::to_string(lua_tonumber(L, 4)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 5)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 6)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 7)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 8)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 9)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 10)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 11)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 12)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 13)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 14)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 15)));

	iCommand* luaCommand = g_currentScene->m_commandFactory->CreateTriggerCommandObject(lua_tostring(L, 3), commandDetails);

	if (g_currentScene->m_commandManager->m_IsCreatingCommandGroup)
	{
		g_currentScene->m_commandManager->m_CommandGroupBeingCreated->addParallel(luaCommand);
	}
	else
	{
		cCommandGroup* commandGroup = new cCommandGroup();
		commandGroup->addSerial(luaCommand);
		glm::vec3 triggerPosition = glm::vec3(
			atof(commandDetails[2].c_str()),
			atof(commandDetails[3].c_str()),
			atof(commandDetails[4].c_str()));

		g_currentScene->m_commandManager->AddTriggerCommandGroups(
			triggerPosition, commandGroup, triggeredFriendlyName, triggeredObject->GetObjectPhysics(), (float)atof(commandDetails[5].c_str()));
	}

	return 1;
}

int LuaAddLoopingAnimation(lua_State* L)
{
	std::string loopingObjectFriendlyName = lua_tostring(L, 1);

	cGameObject* loopingObject = cLuaManager::FindObjectByFriendlyName(loopingObjectFriendlyName);

	if (loopingObject == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	std::vector<std::string> commandDetails;
	commandDetails.push_back(loopingObjectFriendlyName);

	commandDetails.push_back(std::to_string(lua_tonumber(L, 3)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 4)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 5)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 6)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 7)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 8)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 9)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 10)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 11)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 12)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 13)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 14)));
	commandDetails.push_back(std::to_string(lua_tonumber(L, 15)));

	iCommand* luaCommand = g_currentScene->m_commandFactory->CreateLoopingCommandObject(lua_tostring(L, 2), commandDetails);
	cCommandGroup* commandGroup = new cCommandGroup();
	commandGroup->addSerial(luaCommand);
	g_currentScene->m_commandManager->AddLoopingCommandGroup(commandGroup);

	return 1;
}