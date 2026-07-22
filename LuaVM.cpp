#include "LuaVM.h"

namespace Scripting
{
	LuaVM::LuaVM()
	{
		m_State = luaL_newstate();

		luaL_openlibs(m_State);
	}

	LuaVM::~LuaVM()
	{
		if (m_State)
			lua_close(m_State);
	}

	bool LuaVM::LoadScript(const std::string& path)
	{
		if (luaL_dofile(m_State, path.c_str()) != LUA_OK)
		{
			lua_pop(m_State, 1);

			return false;
		}

		return true;
	}
}