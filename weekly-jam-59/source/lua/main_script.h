/*******************************************************************************
 * MAIN SCRIPT
 * Handles setting up, loading, and executing the 'main.lua' file.
 *
*******************************************************************************/

#ifndef __TCE_MAIN_SCRIPT_H__ /*//////////////////////////////////////////////*/
#define __TCE_MAIN_SCRIPT_H__

namespace TCE
{

GLOBAL constexpr const char* LUA_MAIN_SCRIPT_PATH     = "data/scripts/main.lua";

GLOBAL constexpr const char* LUA_INIT_FUNCTION_NAME   = "initialize";
GLOBAL constexpr const char* LUA_UPDATE_FUNCTION_NAME = "update";
GLOBAL constexpr const char* LUA_RENDER_FUNCTION_NAME = "render";

GLOBAL lua_State* lua_main_state;
GLOBAL bool reload_lua_state = false;

INLDEF void LuaMainInit ();
INLDEF void LuaMainQuit ();

INLDEF void LoadLuaMainScript ();
INLDEF void ReloadLuaMainScript ();

INLDEF void LuaGameInit ();
INLDEF void LuaGameUpdate (float _dt);
INLDEF void LuaGameRender ();

} // TCE

#endif /* __TCE_MAIN_SCRIPT_H__ //////////////////////////////////////////////*/
