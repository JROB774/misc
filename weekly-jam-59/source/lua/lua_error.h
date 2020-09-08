/*******************************************************************************
 * LUA ERROR
 * Facilities for handling Lua script based errors.
 *
*******************************************************************************/

#ifndef __TCE_LUA_ERROR_H__ /*////////////////////////////////////////////////*/
#define __TCE_LUA_ERROR_H__

namespace TCE
{

GLOBAL constexpr const char* LUA_TYPE_NAME[LUA_NUMTAGS] =
{
	"NIL",
	"BOOLEAN",
	"LIGHT USERDATA",
	"NUMBER",
	"STRING",
	"TABLE",
	"FUNCTION",
	"USERDATA",
	"THREAD"
};

INLDEF void LogLuaError (lua_State* _lua_state);
STDDEF void LuaDumpStack (lua_State* _lua_state);

} // TCE

#endif /* __TCE_LUA_ERROR_H__ ////////////////////////////////////////////////*/
