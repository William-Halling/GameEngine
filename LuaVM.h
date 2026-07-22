#pragma once
#include <lua.hpp>
#include <string>

namespace Scripting
{
    class LuaVM
    {
    public:
        LuaVM();
        ~LuaVM();

        LuaVM(const LuaVM&) = delete;
        LuaVM& operator=(const LuaVM&) = delete;

        bool LoadScript(const std::string& path);
        lua_State* State() noexcept { return m_State; }

	private:
		lua_State* m_State = nullptr;
	};
}
