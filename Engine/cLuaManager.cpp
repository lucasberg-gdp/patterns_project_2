#include "cLuaManager.h"
#include <sstream>
#include "cGameObject.h"

std::vector< cGameObject* >* cLuaManager::m_GameObjects;

int LuaAddSerialCommand(lua_State* L);
int LuaCreateCommandGroup(lua_State* L);
int LuaCompleteCommandGroup(lua_State* L);
int LuaAddParallelCommand(lua_State* L);

int LuaAddLocationTriggerCommand(lua_State* L);
int LuaAddLoopingAnimation(lua_State* L);

cLuaManager::cLuaManager()
{
	m_GameObjects = nullptr;

	m_LuaState = luaL_newstate();

	if (m_LuaState == nullptr)
	{
		std::cout << "Lua failed to start" << std::endl;
	}

	luaL_openlibs(m_LuaState);

	lua_pushcfunction(m_LuaState, cLuaManager::l_UpdateObject);
	lua_setglobal(m_LuaState, "setObjectState");

	lua_pushcfunction(m_LuaState, cLuaManager::l_GetObjectState);
	lua_setglobal(m_LuaState, "getObjectState");

	lua_pushcfunction(m_LuaState, cLuaManager::l_GetMeshFileName);
	lua_setglobal(m_LuaState, "getGameObjectFileName");

	lua_pushcfunction(m_LuaState, LuaAddSerialCommand);
	lua_setglobal(m_LuaState, "AddSerialCommand");

	lua_pushcfunction(m_LuaState, LuaAddParallelCommand);
	lua_setglobal(m_LuaState, "AddParallelCommand");

	lua_pushcfunction(m_LuaState, LuaCreateCommandGroup);
	lua_setglobal(m_LuaState, "CreateCommandGroup");

	lua_pushcfunction(m_LuaState, LuaCompleteCommandGroup);
	lua_setglobal(m_LuaState, "CompleteCommandGroup");

	lua_pushcfunction(m_LuaState, LuaAddLocationTriggerCommand);
	lua_setglobal(m_LuaState, "AddLocationTriggerCommand");

	lua_pushcfunction(m_LuaState, LuaAddLoopingAnimation);
	lua_setglobal(m_LuaState, "AddLoopingAnimation");
	

	return;
}

cLuaManager::~cLuaManager()
{
	lua_close(m_LuaState);
}

void cLuaManager::Update(double deltaTime)
{
	// Here, you could pass a delta time and save it into the Lua state as a 
// variable. On the 1st call, the variable will be created and loaded, 
// but after that, it would just be updated. 

// Uses the RunScriptImmediately() method

	std::stringstream deltaTimeStringStream;
	deltaTimeStringStream << "deltaTime = " << deltaTime;
	this->RunScript(deltaTimeStringStream.str().c_str());

	// At this point, there is a variable called "deltaTime" that's in Lua.



//	std::cout << "cLuaBrain::Update() called" << std::endl;
	for (std::map< std::string /*name*/, std::string /*source*/>::iterator itScript =
		this->m_mapScripts.begin(); itScript != this->m_mapScripts.end(); itScript++)
	{
		// Pass the script into Lua and update
//		int error = luaL_loadbuffer(L, buffer, strlen(buffer), "line");

		std::string curLuaScript = itScript->second;

		int error = luaL_loadstring(this->m_LuaState,
			curLuaScript.c_str());

		if (error != 0 /*no error*/)
		{
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << this->decodeLuaErrorToString(error) << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			continue;
		}

		// execute funtion in "protected mode", where problems are 
		//  caught and placed on the stack for investigation
		error = lua_pcall(this->m_LuaState,	/* lua state */
			0,	/* nargs: number of arguments pushed onto the lua stack */
			0,	/* nresults: number of results that should be on stack at end*/
			0);	/* errfunc: location, in stack, of error function.
					 if 0, results are on top of stack. */
		if (error != 0 /*no error*/)
		{
			std::cout << "Lua: There was an error..." << std::endl;
			std::cout << decodeLuaErrorToString(error) << std::endl;

			std::string luaError;
			// Get error information from top of stack (-1 is top)
			luaError.append(lua_tostring(this->m_LuaState, -1));

			// Make error message a little more clear
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << luaError << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			// We passed zero (0) as errfunc, so error is on stack)
			lua_pop(this->m_LuaState, 1);  /* pop error message from the stack */

			continue;
		}

	}

	return;
}

void cLuaManager::RunScript(std::string script)
{
	int error = luaL_loadstring(m_LuaState, script.c_str());

	if (error != 0)
	{
		std::cout << "Error running Lua script: " << decodeLuaErrorToString(error) << std::endl;
	}

	error = lua_pcall(this->m_LuaState,
		0,
		0,
		0);	

	if (error != 0 /*no error*/)
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << this->decodeLuaErrorToString(error) << std::endl;

		std::string luaError;

		luaError.append(lua_tostring(this->m_LuaState, -1));

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(this->m_LuaState, 1);  /* pop error message from the stack */
	}

	return;
}

void cLuaManager::SetObjectVector(std::vector<cGameObject*>* gameObjects)
{
	m_GameObjects = gameObjects;
}


// Saves (and overwrites) any script
// scriptName is just so we can delete them later
void cLuaManager::LoadScript(std::string scriptName,
	std::string scriptSource)
{
	this->m_mapScripts[scriptName] = scriptSource;
	return;
}


void cLuaManager::DeleteScript(std::string scriptName)
{
	this->m_mapScripts.erase(scriptName);
	return;
}

int cLuaManager::l_UpdateObject(lua_State* L)
{
	int objectID = (int)lua_tonumber(L, 1);	/* get argument */

	cGameObject* pGO = cLuaManager::m_findObjectByID(objectID);

	if (pGO == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	pGO->m_objectPhysics->position.x = (float)lua_tonumber(L, 2);
	pGO->m_objectPhysics->position.y = (float)lua_tonumber(L, 3);
	pGO->m_objectPhysics->position.z = (float)lua_tonumber(L, 4);
	pGO->m_objectPhysics->velocity.x = (float)lua_tonumber(L, 5);
	pGO->m_objectPhysics->velocity.y = (float)lua_tonumber(L, 6);
	pGO->m_objectPhysics->velocity.z = (float)lua_tonumber(L, 7);

	lua_pushboolean(L, true);

	return 1;
}

int cLuaManager::l_GetObjectState(lua_State* L)
{
	int objectID = (int)lua_tonumber(L, 1);	/* get argument */

	cGameObject* pGO = cLuaManager::m_findObjectByID(objectID);

	if (pGO == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, true);
	lua_pushnumber(L, pGO->m_objectPhysics->position.x);
	lua_pushnumber(L, pGO->m_objectPhysics->position.y);
	lua_pushnumber(L, pGO->m_objectPhysics->position.z);
	lua_pushnumber(L, pGO->m_objectPhysics->velocity.x);
	lua_pushnumber(L, pGO->m_objectPhysics->velocity.y);
	lua_pushnumber(L, pGO->m_objectPhysics->velocity.z);

	return 7;
}

int cLuaManager::l_GetMeshFileName(lua_State* L)
{
	int objectId = (int)lua_tonumber(L, 1);

	cGameObject* gameObject = cLuaManager::m_findObjectByID(objectId);

	if (gameObject == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, true);
	lua_pushstring(L, gameObject->m_objectMesh->meshName.c_str());

	return 2;
}

std::string cLuaManager::decodeLuaErrorToString(int error)
{
	switch (error)
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error";
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}

	return "Lua: UNKNOWN error";
}

cGameObject* cLuaManager::m_findObjectByID(int ID)
{
	for (std::vector<cGameObject*>::iterator itGO = cLuaManager::m_GameObjects->begin();
		itGO != cLuaManager::m_GameObjects->end(); itGO++)
	{
		if ((*itGO)->GetObjectMesh()->GetUniqueID() == ID)
		{
			return (*itGO);
		}
	}

	return nullptr;
}

cGameObject* cLuaManager::FindObjectByFriendlyName(std::string friendlyName)
{
	for (std::vector<cGameObject*>::iterator itGO = cLuaManager::m_GameObjects->begin();
		itGO != cLuaManager::m_GameObjects->end(); itGO++)
	{
		if ((*itGO)->GetObjectMesh()->friendlyName == friendlyName)
		{
			return (*itGO);
		}
	}

	return nullptr;
}
