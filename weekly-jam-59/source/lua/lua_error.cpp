/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void LogLuaError (lua_State* _lua_state)
{
	// Get the Lua error string and log it using the error logger.
	const char* error_string = lua_tostring(_lua_state, -1);
	TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Lua error has occurred:\n%s", error_string);

	// Open a custom message box that provides the ability to quit.
	constexpr const char* ERROR_FORMAT = "%s\nDo you want to quit?";
	int string_length = snprintf(NULL, 0, ERROR_FORMAT, error_string) + 1;
	String formatted_string;
	formatted_string.Resize(string_length);
	snprintf(formatted_string.c_string, string_length, ERROR_FORMAT, error_string);
	formatted_string.length = string_length;

	// If the user pressed yes then we want to stop running the game.
	MessageBoxReturn button_pressed = ShowMessageBox("Error!", formatted_string.c_string, MESSAGEBOX_TYPE_ERROR, MESSAGEBOX_BUTTON_YESNO);
	running = (button_pressed != MESSAGEBOX_RETURN_YES);

	// Remove the error from the stack.
	lua_pop(_lua_state, 1);
}

STDDEF void LuaDumpStack (lua_State* _lua_state)
{
	int stack_top = lua_gettop(_lua_state);
	for (int i=0; i<=stack_top; ++i) {
		// Print information on each of the types stored in the Lua stack.
		int type_id = lua_type(_lua_state, i);
		switch (type_id) {
			case (LUA_TNIL): {
				printf("\n%d of type %s.",    i, LUA_TYPE_NAME[type_id]);
			} break;
			case (LUA_TBOOLEAN): {
				printf("\n%d of type %s: %s", i, LUA_TYPE_NAME[type_id],
					lua_toboolean(_lua_state, i) ? "true" : "false");
			} break;
			case (LUA_TLIGHTUSERDATA): {
				printf("\n%d of tpye %s: %p", i, LUA_TYPE_NAME[type_id],
					lua_touserdata(_lua_state, i));
			} break;
			case (LUA_TNUMBER): {
				printf("\n%d of type %s: %g", i, LUA_TYPE_NAME[type_id],
					lua_tonumber(_lua_state, i));
			} break;
			case (LUA_TSTRING): {
				printf("\n%d of type %s: %s", i, LUA_TYPE_NAME[type_id],
					lua_tostring(_lua_state, i));
			} break;
			case (LUA_TTABLE): {
				printf("\n%d of type %s.",    i, LUA_TYPE_NAME[type_id]);
			} break;
			case (LUA_TFUNCTION): {
				printf("\n%d of type %s: %p", i, LUA_TYPE_NAME[type_id],
					lua_tocfunction(_lua_state, i));
			} break;
			case (LUA_TUSERDATA): {
				printf("\n%d of type %s: %p", i, LUA_TYPE_NAME[type_id],
					lua_touserdata(_lua_state, i));
			} break;
			case (LUA_TTHREAD): {
				printf("\n^%d of type %s.", i, LUA_TYPE_NAME[type_id]);
			} break;
		}
	}
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
