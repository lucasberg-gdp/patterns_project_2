#ifndef _cLuaBrain_HG_
#define _cLuaBrain_HG_

extern "C" {
#include <Lua5.4.7/lua.h>
#include <Lua5.4.7/lauxlib.h>
#include <Lua5.4.7/lualib.h>
}

#include <string>
#include <iostream>
#include <vector>
#include <map>

class cGameObject;

class cLuaManager
{
	std::map< std::string,std::string> m_mapScripts;

	std::string decodeLuaErrorToString(int error);

public:
	static cGameObject* m_findObjectByID(int ID);

	static cGameObject* FindObjectByFriendlyName(std::string friendlyName);

	lua_State* m_LuaState;

	static std::vector< cGameObject* >* m_GameObjects;

	cLuaManager();
	~cLuaManager();

	void Update(double deltaTime);

	void RunScript(std::string script);

	void SetObjectVector(std::vector< cGameObject* >* p_vecGOs);

	void LoadScript(std::string scriptName,
		std::string scriptSource);
	void DeleteScript(std::string scriptName);

	// Called by Lua
	// Passes object ID, new velocity, etc.
	// Returns valid (true or false)
	// Passes: 
	// - position (xyz)
	// - velocity (xyz)
	// called "setObjectState" in lua
	static int l_UpdateObject(lua_State* L);

	// Passes object ID
	// Returns valid (true or false)
	// - position (xyz)
	// - velocity (xyz)
	// called "getObjectState" in lua
	static int l_GetObjectState(lua_State* L);

	static int l_GetMeshFileName(lua_State* L);
};

#endif