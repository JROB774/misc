/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void LuaMainInit ()
{
	// Sets up the Lua State used to interface between C and Lua.
	lua_main_state = luaL_newstate();
	if (!lua_main_state) {
		TCE_ERROR_LOG(ERROR_LEVEL_MAX, ERROR_TYPE_STD, "Failed to create Lua state!");
		return;
	}

	// Provides our main.lua access to standard library features.
	luaL_openlibs(lua_main_state);
	// Provides out main.lua access to TCE library features.
	RegisterLuaExports(lua_main_state);
	// Loads the main script into the state, for execution.
	LoadLuaMainScript();
}

INLDEF void LuaMainQuit ()
{
	lua_close(lua_main_state);
	lua_main_state = NULL;
}

INLDEF void LoadLuaMainScript ()
{
	// Attempts to load and run the 'main.lua' file where the game loop functions are stored.
	if (luaL_dofile(lua_main_state, LUA_MAIN_SCRIPT_PATH) != 0) {
		LogLuaError(lua_main_state);
	}

	// Because there is only one main script file we do not need a handle to let the hotloader
	// know where it can find the file. It will simply call the LoadMainScript function.
	RegisterAssetForHotloading(LUA_MAIN_SCRIPT_PATH, ASSET_MAIN_SCRIPT, INVALID_ASSET_HANDLE);
}
INLDEF void ReloadLuaMainScript ()
{
	// Unloads the entire Lua state for the game.
	LuaMainQuit();

	FreeTextureList();
	FreeSoundList();

	texture_list.Clear();
	sound_list.Clear();
	animation_list.Clear();
	font_list.Clear();
	light_list.Clear();

	// Reloads the entire Lua state for the game.
	ConsoleInit();
	LightingInit();

	LuaMainInit();
	LuaGameInit();

	reload_lua_state = false;
}

INLDEF void LuaGameInit ()
{
	lua_getglobal(lua_main_state, LUA_INIT_FUNCTION_NAME);
	if (lua_pcall(lua_main_state, 0, 0, 0) != 0) {
		LogLuaError(lua_main_state);
	}
}
INLDEF void LuaGameUpdate (float _dt)
{
	lua_getglobal(lua_main_state, LUA_UPDATE_FUNCTION_NAME);
	lua_pushnumber(lua_main_state, _dt);
	if (lua_pcall(lua_main_state, 1, 0, 0) != 0) {
		LogLuaError(lua_main_state);
	}
}
INLDEF void LuaGameRender ()
{
	lua_getglobal(lua_main_state, LUA_RENDER_FUNCTION_NAME);
	if (lua_pcall(lua_main_state, 0, 0, 0) != 0) {
		LogLuaError(lua_main_state);
	}
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
