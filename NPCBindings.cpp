#include "NPCBindings.h"

namespace Game::NPC::Bindings
{
	static int Lua_NPC_GetPosition(lua_State* L)
	{
		auto* npc = static_cast<NPCComponent*>(lua_touserdata(L, 1));
		lua_pushnumber(L, npc->position.x);
		lua_pushnumber(L, npc->position.y);
		lua_pushnumber(L, npc->position.z);

		return 3;
	}


	static int Lua_NPC_SetState(lua_State* L)
	{
		auto* npc = static_cast<NPCComponent*>(lua_touserdata(L, 1));
		int state = static_cast<int>(lua_tointeger(L, 2));
		npc->state = static_cast<NPCState>(state);

		return 0;
	}


	void Register(lua_State* L)
	{
		lua_register(L, "NPC_GetPosition", Lua_NPC_GetPosition);

		lua_register(L, "NPC_SetState", Lua_NPC_SetState);
	}
}