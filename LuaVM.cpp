#include "LuaVM.h"
#include <iostream>

namespace Scripting
{
	LuaVM::LuaVM()
	{
        m_State = luaL_newstate();
        if (m_State)
        {
            luaL_openlibs(m_State);
        }
	}

	LuaVM::~LuaVM()
	{
		if (m_State)
			lua_close(m_State);
	}

    LuaVM& LuaVM::operator=(LuaVM&& other) noexcept
    {
        if (this != &other)
        {
            if (m_State) 
				lua_close(m_State);
            
				m_State = other.m_State;
            other.m_State = nullptr;
        }
     
		return *this;
    }

	bool LuaVM::LoadScript(const std::string& path)
	{
		if (!m_State) 
			return false;
        
			if (luaL_dofile(m_State, path.c_str()) != LUA_OK)
        {
            const char* err = lua_tostring(m_State, -1);
            std::cerr << "Lua error: " << (err ? err : "unknown") << '\n';

            lua_pop(m_State, 1);
        
			return false;
        }
        return true;
	}
}