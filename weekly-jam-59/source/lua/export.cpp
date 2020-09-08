/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void RegisterLuaExports (lua_State* _lua_state)
{
	// INTERFACE - PRIVATE
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_ReloadGame",               Lua_ReloadGame);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_ClearConsole",             Lua_ClearConsole);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_SetViewport",              Lua_SetViewport);
	// INTERFACE - TEXTURE
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_LoadTextureFromFile",      Lua_LoadTextureFromFile);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderTexture",            Lua_RenderTexture);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderAnimatedTexture",    Lua_RenderAnimatedTexture);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderClippedTexture",     Lua_RenderClippedTexture);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_SetTextureColor",          Lua_SetTextureColor);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetTextureWidth",          Lua_GetTextureWidth);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetTextureHeight",         Lua_GetTextureHeight);
	// INTERFACE - ANIMATION
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_LoadAnimationFromFile",    Lua_LoadAnimationFromFile);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_PauseAnimation",           Lua_PauseAnimation);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_AnimationIsPaused",        Lua_AnimationIsPaused);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_ResetAnimation",           Lua_ResetAnimation);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetAnimationFrameWidth",   Lua_GetAnimationFrameWidth);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetAnimationFrameHeight",  Lua_GetAnimationFrameHeight);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_PauseAllAnimations",       Lua_PauseAllAnimations);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetAnimationLength",       Lua_GetAnimationLength);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_AnimationIsDone",          Lua_AnimationIsDone);
	// INTERFACE - FONT
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_LoadFontFromFile",         Lua_LoadFontFromFile);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderText",               Lua_RenderText);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderShadowedText",       Lua_RenderShadowedText);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_SetFontColor",             Lua_SetFontColor);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetFontGlyphWidth",        Lua_GetFontGlyphWidth);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetFontGlyphHeight",       Lua_GetFontGlyphHeight);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetTextWidth",             Lua_GetTextWidth);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetTextHeight",            Lua_GetTextHeight);
	// INTERFACE - SOUND
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_LoadSoundFromFile",        Lua_LoadSoundFromFile);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_PlaySound",                Lua_PlaySound);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_StopSound",                Lua_StopSound);
	// INTERFACE - MUSIC
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_PlayMusic",                Lua_PlayMusic);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_StopMusic",                Lua_StopMusic);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_FadeInMusic",              Lua_FadeInMusic);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_FadeOutMusic",             Lua_FadeOutMusic);
	// INTERFACE - EVENT
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetMousePosition",         Lua_GetMousePosition);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetRelativeMousePosition", Lua_GetRelativeMousePosition);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetGlobalMousePosition",   Lua_GetGlobalMousePosition);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_IsKeyDown",                Lua_IsKeyDown);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_IsKeyUp",                  Lua_IsKeyUp);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_KeyPressed",               Lua_KeyPressed);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_KeyReleased",              Lua_KeyReleased);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_IsButtonDown",             Lua_IsButtonDown);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_IsButtonUp",               Lua_IsButtonUp);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_ButtonPressed",            Lua_ButtonPressed);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_ButtonReleased",           Lua_ButtonReleased);
	// INTERFACE - PRIMITIVE
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderPoint",              Lua_RenderPoint);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderLine",               Lua_RenderLine);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderRectOutline",        Lua_RenderRectOutline);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderRectFilled",         Lua_RenderRectFilled);
	// INTERFACE - WINDOW
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetViewportWidth",         Lua_GetViewportWidth);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetViewportHeight",        Lua_GetViewportHeight);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_ShowCursor",               Lua_ShowCursor);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_Exit",                     Lua_Exit);
	// INTERFACE - VECTOR2D
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetVectorLength",          Lua_GetVectorLength);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_NormalizeVector",          Lua_NormalizeVector);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RotateVector",             Lua_RotateVector);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_VectorDotProduct",         Lua_VectorDotProduct);
	// INTERFACE - COLLISION
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_PointAndBoxCollision",     Lua_PointAndBoxCollision);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_BoxAndBoxCollision",       Lua_BoxAndBoxCollision);
	// INTERFACE - UTILITY
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_MakeColor",                Lua_MakeColor);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_MakeRGBA",                 Lua_MakeRGBA);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_ConsolePrint",             Lua_ConsolePrint);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetFPS",                   Lua_GetFPS);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RandomSeed",               Lua_RandomSeed);
	// INTERFACE - ZONE
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_LoadZoneFromFile",         Lua_LoadZoneFromFile);
	// INTERFACE - LIGHTING
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_TurnLightingOn",           Lua_TurnLightingOn);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RenderLighting",           Lua_RenderLighting);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_AddLight",                 Lua_AddLight);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_UpdateLight",              Lua_UpdateLight);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_TurnOffLight",             Lua_TurnOffLight);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_RemoveAllLights",          Lua_RemoveAllLights);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_SetShadowColor",           Lua_SetShadowColor);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_SetLightColor",            Lua_SetLightColor);
	// INTERFACE - CAMERA
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_BeginCamera",              Lua_BeginCamera);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_EndCamera",                Lua_EndCamera);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_CameraTrack",              Lua_CameraTrack);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_SetCameraPosition",        Lua_SetCameraPosition);
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_GetCameraPosition",        Lua_GetCameraPosition);
	// INTERFACE - PATHFINDING
	REGISTER_LUA_FUNCTION(_lua_state, "TCE_FindPath",                 Lua_FindPath);
}

/* INTERFACE - PRIVATE ********************************************************/
EXPORT_LUA_FUNCTION(Lua_ReloadGame)
{
	reload_lua_state = true;
	return 0;
}
EXPORT_LUA_FUNCTION(Lua_ClearConsole)
{
	ClearConsole();
	return 0;
}
EXPORT_LUA_FUNCTION(Lua_SetViewport)
{
	lua_settop(_lua_state, 2);

	viewport_width  = CAST(int, luaL_checknumber(_lua_state, 1));
	viewport_height = CAST(int, luaL_checknumber(_lua_state, 2));

	UpdateWindowValues();

	return 0;
}
/******************************************************************************/

/* INTERFACE - TEXTURE ********************************************************/
EXPORT_LUA_FUNCTION(Lua_LoadTextureFromFile)
{
	lua_settop(_lua_state, 1);

	const char* filename = luaL_checkstring(_lua_state, 1);

	// Combine the texture path with the filename.
	String full_filename = TEXTURE_PATH;
	full_filename.AddCString(filename);

	TextureHandle texture = LoadTextureFromFile(full_filename.c_string);
	lua_pushinteger(_lua_state, texture);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_RenderTexture)
{
	lua_settop(_lua_state, 2);

	TextureHandle texture = luaL_checkinteger(_lua_state, 1);
	luaL_checktype(_lua_state, 2, LUA_TTABLE);

	lua_getfield(_lua_state, 2, "x");
	lua_getfield(_lua_state, 2, "y");
	lua_getfield(_lua_state, 2, "scale_x");
	lua_getfield(_lua_state, 2, "scale_y");
	lua_getfield(_lua_state, 2, "angle");
	lua_getfield(_lua_state, 2, "anchor_x");
	lua_getfield(_lua_state, 2, "anchor_y");
	lua_getfield(_lua_state, 2, "flip");

	int   x        = CAST(int, luaL_checknumber(_lua_state, -8));
	int   y        = CAST(int, luaL_checknumber(_lua_state, -7));
	float scale_x  =           luaL_optnumber  (_lua_state, -6, 1.0f);
	float scale_y  =           luaL_optnumber  (_lua_state, -5, 1.0f);
	float angle    =           luaL_optnumber  (_lua_state, -4, 0.0f);
	int   anchor_x = CAST(int, luaL_optnumber  (_lua_state, -3, CAST(float, GetTextureWidth(texture) / 2)));
	int   anchor_y = CAST(int, luaL_optnumber  (_lua_state, -2, CAST(float, GetTextureHeight(texture) / 2)));
	int   flip     =           luaL_optinteger (_lua_state, -1, SDL_FLIP_NONE);

	lua_pop(_lua_state, 8);

	anchor_x = CAST(int, CAST(float, anchor_x) * scale_x);
	anchor_y = CAST(int, CAST(float, anchor_y) * scale_y);

	RenderTexture(texture, x, y, scale_x, scale_y, angle, anchor_x, anchor_y, CAST(SDL_RendererFlip, flip));

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_RenderAnimatedTexture)
{
	lua_settop(_lua_state, 3);

	TextureHandle texture = luaL_checkinteger(_lua_state, 1);
	AnimationHandle animation = luaL_checkinteger(_lua_state, 2);
	luaL_checktype(_lua_state, 3, LUA_TTABLE);

	lua_getfield(_lua_state, 3, "x");
	lua_getfield(_lua_state, 3, "y");
	lua_getfield(_lua_state, 3, "scale_x");
	lua_getfield(_lua_state, 3, "scale_y");
	lua_getfield(_lua_state, 3, "angle");
	lua_getfield(_lua_state, 3, "anchor_x");
	lua_getfield(_lua_state, 3, "anchor_y");
	lua_getfield(_lua_state, 3, "flip");

	int   x        = CAST(int, luaL_checknumber(_lua_state, -8));
	int   y        = CAST(int, luaL_checknumber(_lua_state, -7));
	float scale_x  =           luaL_optnumber  (_lua_state, -6, 1.0f);
	float scale_y  =           luaL_optnumber  (_lua_state, -5, 1.0f);
	float angle    =           luaL_optnumber  (_lua_state, -4, 0.0f);
	int   anchor_x = CAST(int, luaL_optnumber  (_lua_state, -3, CAST(float, GetAnimationFrameWidth(animation) / 2)));
	int   anchor_y = CAST(int, luaL_optnumber  (_lua_state, -2, CAST(float, GetAnimationFrameHeight(animation) / 2)));
	int   flip     =           luaL_optinteger (_lua_state, -1, SDL_FLIP_NONE);

	lua_pop(_lua_state, 8);

	anchor_x = CAST(int, CAST(float, anchor_x) * scale_x);
	anchor_y = CAST(int, CAST(float, anchor_y) * scale_y);

	RenderAnimatedTexture(texture, animation, x, y, scale_x, scale_y, angle, anchor_x, anchor_y, CAST(SDL_RendererFlip, flip));

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_RenderClippedTexture)
{
	lua_settop(_lua_state, 3);

	TextureHandle texture = luaL_checkinteger(_lua_state, 1);
	luaL_checktype(_lua_state, 2, LUA_TTABLE);
	luaL_checktype(_lua_state, 3, LUA_TTABLE);

	lua_getfield(_lua_state, 2, "x");
	lua_getfield(_lua_state, 2, "y");
	lua_getfield(_lua_state, 2, "w");
	lua_getfield(_lua_state, 2, "h");

	lua_getfield(_lua_state, 3, "x");
	lua_getfield(_lua_state, 3, "y");
	lua_getfield(_lua_state, 3, "scale_x");
	lua_getfield(_lua_state, 3, "scale_y");
	lua_getfield(_lua_state, 3, "angle");
	lua_getfield(_lua_state, 3, "anchor_x");
	lua_getfield(_lua_state, 3, "anchor_y");
	lua_getfield(_lua_state, 3, "flip");

	SDL_Rect clip;
	clip.x = CAST(int, luaL_checknumber(_lua_state, -12));
	clip.y = CAST(int, luaL_checknumber(_lua_state, -11));
	clip.w = CAST(int, luaL_checknumber(_lua_state, -10));
	clip.h = CAST(int, luaL_checknumber(_lua_state,  -9));

	int   x        = CAST(int, luaL_checknumber(_lua_state, -8));
	int   y        = CAST(int, luaL_checknumber(_lua_state, -7));
	float scale_x  =           luaL_optnumber  (_lua_state, -6, 1.0f);
	float scale_y  =           luaL_optnumber  (_lua_state, -5, 1.0f);
	float angle    =           luaL_optnumber  (_lua_state, -4, 0.0f);
	int   anchor_x = CAST(int, luaL_optnumber  (_lua_state, -3, CAST(float, GetTextureWidth(texture) / 2)));
	int   anchor_y = CAST(int, luaL_optnumber  (_lua_state, -2, CAST(float, GetTextureHeight(texture) / 2)));
	int   flip     =           luaL_optinteger (_lua_state, -1, SDL_FLIP_NONE);

	lua_pop(_lua_state, 12);

	anchor_x = CAST(int, CAST(float, anchor_x) * scale_x);
	anchor_y = CAST(int, CAST(float, anchor_y) * scale_y);

	RenderTexture(texture, x, y, scale_x, scale_y, angle, anchor_x, anchor_y, CAST(SDL_RendererFlip, flip), &clip);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_SetTextureColor)
{
	lua_settop(_lua_state, 2);

	Color color;

	TextureHandle texture = luaL_checkinteger(_lua_state, 1);
	color.numeric = SDL_SwapBE32(lua_tointeger(_lua_state, 2));

	SetTextureColor(texture, color.component);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_GetTextureWidth)
{
	lua_settop(_lua_state, 1);

	TextureHandle texture = luaL_checkinteger(_lua_state, 1);

	int texture_width = GetTextureWidth(texture);
	lua_pushinteger(_lua_state, texture_width);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_GetTextureHeight)
{
	lua_settop(_lua_state, 1);

	TextureHandle texture = luaL_checkinteger(_lua_state, 1);

	int texture_height = GetTextureHeight(texture);
	lua_pushinteger(_lua_state, texture_height);

	return 1;
}
/******************************************************************************/

/* INTERFACE - ANIMATION ******************************************************/
EXPORT_LUA_FUNCTION(Lua_LoadAnimationFromFile)
{
	lua_settop(_lua_state, 1);

	const char* filename = luaL_checkstring(_lua_state, 1);

	// Combine the animation path with the filename.
	String full_filename = ANIMATION_PATH;
	full_filename.AddCString(filename);

	AnimationHandle animation = LoadAnimationFromFile(full_filename.c_string);
	lua_pushinteger(_lua_state, animation);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_PauseAnimation)
{
	lua_settop(_lua_state, 2);

	AnimationHandle animation = luaL_checkinteger(_lua_state, 1);
	bool pause = lua_toboolean(_lua_state, 2);

	PauseAnimation(animation, pause);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_AnimationIsPaused)
{
	lua_settop(_lua_state, 1);

	AnimationHandle animation = luaL_checkinteger(_lua_state, 1);

	bool paused = AnimationIsPaused(animation);
	lua_pushboolean(_lua_state, paused);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_ResetAnimation)
{
	lua_settop(_lua_state, 1);

	AnimationHandle animation = luaL_checkinteger(_lua_state, 1);
	ResetAnimation(animation);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_GetAnimationFrameWidth)
{
	lua_settop(_lua_state, 1);

	AnimationHandle animation = luaL_checkinteger(_lua_state, 1);

	int frame_width = GetAnimationFrameWidth(animation);
	lua_pushinteger(_lua_state, frame_width);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_GetAnimationFrameHeight)
{
	lua_settop(_lua_state, 1);

	AnimationHandle animation = luaL_checkinteger(_lua_state, 1);

	int frame_height = GetAnimationFrameHeight(animation);
	lua_pushinteger(_lua_state, frame_height);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_PauseAllAnimations)
{
	lua_settop(_lua_state, 1);

	all_animations_paused = lua_toboolean(_lua_state, 1);
	return 0;
}
EXPORT_LUA_FUNCTION(Lua_GetAnimationLength)
{
	lua_settop(_lua_state, 1);

	AnimationHandle animation = luaL_checkinteger(_lua_state, 1);

	float length = GetAnimationLength(animation);
	lua_pushnumber(_lua_state, length);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_AnimationIsDone)
{
	lua_settop(_lua_state, 1);

	AnimationHandle animation = luaL_checkinteger(_lua_state, 1);

	bool is_done = AnimationIsDone(animation);
	lua_pushboolean(_lua_state, is_done);

	return 1;
}
/******************************************************************************/

/* INTERFACE - FONT ***********************************************************/
EXPORT_LUA_FUNCTION(Lua_LoadFontFromFile)
{
	lua_settop(_lua_state, 1);

	const char* filename = luaL_checkstring(_lua_state, 1);

	// Combine the font path with the filename.
	String full_filename = FONT_PATH;
	full_filename.AddCString(filename);

	FontHandle font = LoadFontFromFile(full_filename.c_string);
	lua_pushinteger(_lua_state, font);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_RenderText)
{
	lua_settop(_lua_state, 3);

	FontHandle font = luaL_checkinteger(_lua_state, 1);
	const char* text = luaL_checkstring(_lua_state, 2);
	luaL_checktype(_lua_state, 3, LUA_TTABLE);

	Font& raw_font = font_list[font];

	lua_getfield(_lua_state, 3, "x");
	lua_getfield(_lua_state, 3, "y");
	lua_getfield(_lua_state, 3, "scale_x");
	lua_getfield(_lua_state, 3, "scale_y");
	lua_getfield(_lua_state, 3, "angle");
	lua_getfield(_lua_state, 3, "anchor_x");
	lua_getfield(_lua_state, 3, "anchor_y");
	lua_getfield(_lua_state, 3, "flip");

	int   x        = CAST(int, luaL_checknumber(_lua_state, -8));
	int   y        = CAST(int, luaL_checknumber(_lua_state, -7));
	float scale_x  =           luaL_optnumber  (_lua_state, -6, 1.0f);
	float scale_y  =           luaL_optnumber  (_lua_state, -5, 1.0f);
	float angle    =           luaL_optnumber  (_lua_state, -4, 0.0f);
	int   anchor_x = CAST(int, luaL_optnumber  (_lua_state, -3, CAST(float, GetTextureWidth(raw_font.glyph_cache) / 2)));
	int   anchor_y = CAST(int, luaL_optnumber  (_lua_state, -2, CAST(float, GetTextureHeight(raw_font.glyph_cache) / 2)));
	int   flip     =           luaL_optinteger (_lua_state, -1, SDL_FLIP_NONE);

	lua_pop(_lua_state, 8);

	anchor_x = CAST(int, CAST(float, anchor_x) * scale_x);
	anchor_y = CAST(int, CAST(float, anchor_y) * scale_y);

	RenderText(font, text, x, y, scale_x, scale_y, angle, anchor_x, anchor_y, CAST(SDL_RendererFlip, flip));

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_RenderShadowedText)
{
	lua_settop(_lua_state, 7);

	Color text_color, text_shadow_color;

	FontHandle  font          = luaL_checkinteger(_lua_state, 1);
	const char* text          = luaL_checkstring(_lua_state, 2);
	text_color.numeric        = SDL_SwapBE32(lua_tointeger(_lua_state, 3));
	text_shadow_color.numeric = SDL_SwapBE32(lua_tointeger(_lua_state, 4));
	int         x_off         = CAST(int, luaL_checknumber(_lua_state, 5));
	int         y_off         = CAST(int, luaL_checknumber(_lua_state, 6));
	                            luaL_checktype(_lua_state, 7, LUA_TTABLE);

	Font& raw_font = font_list[font];

	lua_getfield(_lua_state, 7, "x");
	lua_getfield(_lua_state, 7, "y");
	lua_getfield(_lua_state, 7, "scale_x");
	lua_getfield(_lua_state, 7, "scale_y");
	lua_getfield(_lua_state, 7, "angle");
	lua_getfield(_lua_state, 7, "anchor_x");
	lua_getfield(_lua_state, 7, "anchor_y");
	lua_getfield(_lua_state, 7, "flip");

	int   x        = CAST(int, luaL_checknumber(_lua_state, -8));
	int   y        = CAST(int, luaL_checknumber(_lua_state, -7));
	float scale_x  =           luaL_optnumber  (_lua_state, -6, 1.0f);
	float scale_y  =           luaL_optnumber  (_lua_state, -5, 1.0f);
	float angle    =           luaL_optnumber  (_lua_state, -4, 0.0f);
	int   anchor_x = CAST(int, luaL_optnumber  (_lua_state, -3, CAST(float, GetTextureWidth(raw_font.glyph_cache) / 2)));
	int   anchor_y = CAST(int, luaL_optnumber  (_lua_state, -2, CAST(float, GetTextureHeight(raw_font.glyph_cache) / 2)));
	int   flip     =           luaL_optinteger (_lua_state, -1, SDL_FLIP_NONE);

	lua_pop(_lua_state, 8);

	SetFontColor(font, text_shadow_color.component);
	RenderText(font, text, x+x_off, y+y_off, scale_x, scale_y, angle, anchor_x, anchor_y, CAST(SDL_RendererFlip, flip));
	SetFontColor(font, text_color.component);
	RenderText(font, text, x, y, scale_x, scale_y, angle, anchor_x, anchor_y, CAST(SDL_RendererFlip, flip));

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_SetFontColor)
{
	lua_settop(_lua_state, 2);

	Color color;

	FontHandle font = luaL_checkinteger(_lua_state, 1);
	color.numeric = SDL_SwapBE32(lua_tointeger(_lua_state, 2));

	SetFontColor(font, color.component);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_GetFontGlyphWidth)
{
	lua_settop(_lua_state, 1);

	FontHandle font = luaL_checkinteger(_lua_state, 1);

	int glyph_width = GetFontGlyphWidth(font);
	lua_pushinteger(_lua_state, glyph_width);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_GetFontGlyphHeight)
{
	lua_settop(_lua_state, 1);

	FontHandle font = luaL_checkinteger(_lua_state, 1);

	int glyph_height = GetFontGlyphHeight(font);
	lua_pushinteger(_lua_state, glyph_height);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_GetTextWidth)
{
	lua_settop(_lua_state, 2);

	FontHandle font = luaL_checkinteger(_lua_state, 1);
	const char* text = luaL_checkstring(_lua_state, 2);

	int text_width = GetTextWidth(font, text);
	lua_pushinteger(_lua_state, text_width);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_GetTextHeight)
{
	lua_settop(_lua_state, 2);

	FontHandle font = luaL_checkinteger(_lua_state, 1);
	const char* text = luaL_checkstring(_lua_state, 2);

	int text_height = GetTextHeight(font, text);
	lua_pushinteger(_lua_state, text_height);

	return 1;
}
/******************************************************************************/

/* INTERFACE - SOUND **********************************************************/
EXPORT_LUA_FUNCTION(Lua_LoadSoundFromFile)
{
	lua_settop(_lua_state, 1);

	const char* filename = luaL_checkstring(_lua_state, 1);

	// Combine the sound path with the filename.
	String full_filename = SOUND_PATH;
	full_filename.AddCString(filename);

	SoundHandle sound = LoadSoundFromFile(full_filename.c_string);
	lua_pushinteger(_lua_state, sound);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_PlaySound)
{
	lua_settop(_lua_state, 2);

	SoundHandle sound = luaL_checkinteger(_lua_state, 1);
	int loops = luaL_optinteger(_lua_state, 2, 0);

	PlaySound(sound, loops);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_StopSound)
{
	lua_settop(_lua_state, 1);

	SoundHandle sound = luaL_checkinteger(_lua_state, 1);
	StopSound(sound);

	return 0;
}
/******************************************************************************/

/* INTERFACE - MUSIC **********************************************************/
EXPORT_LUA_FUNCTION(Lua_PlayMusic)
{
	lua_settop(_lua_state, 2);

	const char* filename =            luaL_checkstring(_lua_state, 1);
	int         loops     = CAST(int, luaL_checknumber(_lua_state, 2));

	PlayMusic(filename, loops);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_StopMusic)
{
	StopMusic();

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_FadeInMusic)
{
	lua_settop(_lua_state, 2);

	const char* filename =           luaL_checkstring(_lua_state, 1);
	int         loops    = CAST(int, luaL_checknumber(_lua_state, 2));
	float       time     =           luaL_checknumber(_lua_state, 3);

	FadeInMusic(filename, loops, time);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_FadeOutMusic)
{
	lua_settop(_lua_state, 1);

	float time = luaL_checknumber(_lua_state, 3);
	FadeOutMusic(time);

	return 0;
}
/******************************************************************************/

/* INTERFACE - EVENT **********************************************************/
EXPORT_LUA_FUNCTION(Lua_GetMousePosition)
{
	int mouse_x, mouse_y;
	GetMousePosition(mouse_x, mouse_y);

	lua_pushinteger(_lua_state, mouse_x);
	lua_pushinteger(_lua_state, mouse_y);

	return 2;
}
EXPORT_LUA_FUNCTION(Lua_GetRelativeMousePosition)
{
	int mouse_x, mouse_y;
	GetRelativeMousePosition(mouse_x, mouse_y);

	lua_pushinteger(_lua_state, mouse_x);
	lua_pushinteger(_lua_state, mouse_y);

	return 2;
}
EXPORT_LUA_FUNCTION(Lua_GetGlobalMousePosition)
{
	int mouse_x, mouse_y;
	GetGlobalMousePosition(mouse_x, mouse_y);

	lua_pushinteger(_lua_state, mouse_x);
	lua_pushinteger(_lua_state, mouse_y);

	return 2;
}
EXPORT_LUA_FUNCTION(Lua_IsKeyDown)
{
	lua_settop(_lua_state, 1);

	SDL_Scancode scancode = CAST(SDL_Scancode, luaL_checkinteger(_lua_state, 1));
	bool key_down = IsKeyDown(scancode);

	lua_pushboolean(_lua_state, key_down);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_IsKeyUp)
{
	lua_settop(_lua_state, 1);

	SDL_Scancode scancode = CAST(SDL_Scancode, luaL_checkinteger(_lua_state, 1));
	bool key_up = IsKeyUp(scancode);

	lua_pushboolean(_lua_state, key_up);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_KeyPressed)
{
	lua_settop(_lua_state, 1);

	SDL_Scancode scancode = CAST(SDL_Scancode, luaL_checkinteger(_lua_state, 1));
	bool key_pressed = KeyPressed(scancode);

	lua_pushboolean(_lua_state, key_pressed);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_KeyReleased)
{
	lua_settop(_lua_state, 1);

	SDL_Scancode scancode = CAST(SDL_Scancode, luaL_checkinteger(_lua_state, 1));
	bool key_released = KeyReleased(scancode);

	lua_pushboolean(_lua_state, key_released);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_IsButtonDown)
{
	lua_settop(_lua_state, 1);

	int button = luaL_checkinteger(_lua_state, 1);
	bool button_down = IsButtonDown(button);

	lua_pushboolean(_lua_state, button_down);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_IsButtonUp)
{
	lua_settop(_lua_state, 1);

	int button = luaL_checkinteger(_lua_state, 1);
	bool button_up = IsButtonUp(button);

	lua_pushboolean(_lua_state, button_up);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_ButtonPressed)
{
	lua_settop(_lua_state, 1);

	int button = luaL_checkinteger(_lua_state, 1);
	bool button_pressed = ButtonPressed(button);

	lua_pushboolean(_lua_state, button_pressed);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_ButtonReleased)
{
	lua_settop(_lua_state, 1);

	int button = luaL_checkinteger(_lua_state, 1);
	bool button_released = ButtonReleased(button);

	lua_pushboolean(_lua_state, button_released);

	return 1;
}
/******************************************************************************/

/* INTERFACE - PRIMITIVE ******************************************************/
EXPORT_LUA_FUNCTION(Lua_RenderPoint)
{
	lua_settop(_lua_state, 3);

	int x = CAST(int, luaL_checknumber(_lua_state, 1));
	int y = CAST(int, luaL_checknumber(_lua_state, 2));

	Color color;
	color.numeric = SDL_SwapBE32(lua_tointeger(_lua_state, 3));

	RenderPoint(x, y, color.component);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_RenderLine)
{
	lua_settop(_lua_state, 5);

	int x1 = CAST(int, luaL_checknumber(_lua_state, 1));
	int y1 = CAST(int, luaL_checknumber(_lua_state, 2));
	int x2 = CAST(int, luaL_checknumber(_lua_state, 3));
	int y2 = CAST(int, luaL_checknumber(_lua_state, 4));

	Color color;
	color.numeric = SDL_SwapBE32(lua_tointeger(_lua_state, 5));

	RenderLine(x1, y1, x2, y2, color.component);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_RenderRectOutline)
{
	lua_settop(_lua_state, 5);

	int x = CAST(int, luaL_checknumber(_lua_state, 1));
	int y = CAST(int, luaL_checknumber(_lua_state, 2));
	int w = CAST(int, luaL_checknumber(_lua_state, 3));
	int h = CAST(int, luaL_checknumber(_lua_state, 4));

	Color color;
	color.numeric = SDL_SwapBE32(lua_tointeger(_lua_state, 5));

	RenderRectOutline(x, y, w, h, color.component);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_RenderRectFilled)
{
	lua_settop(_lua_state, 5);

	int x = CAST(int, luaL_checknumber(_lua_state, 1));
	int y = CAST(int, luaL_checknumber(_lua_state, 2));
	int w = CAST(int, luaL_checknumber(_lua_state, 3));
	int h = CAST(int, luaL_checknumber(_lua_state, 4));

	Color color;
	color.numeric = SDL_SwapBE32(lua_tointeger(_lua_state, 5));

	RenderRectFilled(x, y, w, h, color.component);

	return 0;
}
/******************************************************************************/

/* INTERFACE - WINDOW *********************************************************/
EXPORT_LUA_FUNCTION(Lua_GetViewportWidth)
{
	lua_pushinteger(_lua_state, viewport_width);
	return 1;
}
EXPORT_LUA_FUNCTION(Lua_GetViewportHeight)
{
	lua_pushinteger(_lua_state, viewport_height);
	return 1;
}
EXPORT_LUA_FUNCTION(Lua_ShowCursor)
{
	lua_settop(_lua_state, 1);

	bool show = lua_toboolean(_lua_state, 1);
	if (SDL_ShowCursor(show) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to %s cursor!",
			(show) ? "show" : "hide");
	}

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_Exit)
{
	running = false;
	return 0;
}
/******************************************************************************/

/* INTERFACE - VECTOR2D *******************************************************/
EXPORT_LUA_FUNCTION(Lua_GetVectorLength)
{
	lua_settop(_lua_state, 1);

	luaL_checktype(_lua_state, 1, LUA_TTABLE);

	lua_getfield(_lua_state, 1, "x");
	lua_getfield(_lua_state, 1, "y");

	Vector2D vector;
	vector.x = luaL_checknumber(_lua_state, -2);
	vector.y = luaL_checknumber(_lua_state, -1);

	lua_pop(_lua_state, 2);

	float length = GetVectorLength(vector);
	lua_pushnumber(_lua_state, length);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_NormalizeVector)
{
	lua_settop(_lua_state, 1);

	luaL_checktype(_lua_state, 1, LUA_TTABLE);

	lua_getfield(_lua_state, 1, "x");
	lua_getfield(_lua_state, 1, "y");

	Vector2D vector;
	vector.x = luaL_checknumber(_lua_state, -2);
	vector.y = luaL_checknumber(_lua_state, -1);

	lua_pop(_lua_state, 2);

	Vector2D normalized = NormalizeVector(vector);

	lua_newtable(_lua_state);
	lua_pushnumber(_lua_state, normalized.x);
	lua_setfield(_lua_state, -2, "x");
	lua_pushnumber(_lua_state, normalized.y);
	lua_setfield(_lua_state, -2, "y");

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_RotateVector)
{
	lua_settop(_lua_state, 2);

	luaL_checktype(_lua_state, 1, LUA_TTABLE);
	float radians = luaL_checknumber(_lua_state, 2);

	lua_getfield(_lua_state, 1, "x");
	lua_getfield(_lua_state, 1, "y");

	Vector2D vector;
	vector.x = luaL_checknumber(_lua_state, -2);
	vector.y = luaL_checknumber(_lua_state, -1);

	lua_pop(_lua_state, 2);

	Vector2D rotated = RotateVector(vector, radians);

	lua_newtable(_lua_state);
	lua_pushnumber(_lua_state, rotated.x);
	lua_setfield(_lua_state, -2, "x");
	lua_pushnumber(_lua_state, rotated.y);
	lua_setfield(_lua_state, -2, "y");

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_VectorDotProduct)
{
	lua_settop(_lua_state, 2);

	luaL_checktype(_lua_state, 1, LUA_TTABLE);
	luaL_checktype(_lua_state, 2, LUA_TTABLE);

	lua_getfield(_lua_state, 1, "x");
	lua_getfield(_lua_state, 1, "y");
	lua_getfield(_lua_state, 2, "x");
	lua_getfield(_lua_state, 2, "y");

	Vector2D a, b;
	a.x = luaL_checknumber(_lua_state, -4);
	a.y = luaL_checknumber(_lua_state, -3);
	b.x = luaL_checknumber(_lua_state, -2);
	b.y = luaL_checknumber(_lua_state, -1);

	lua_pop(_lua_state, 4);

	float dot_product = VectorDotProduct(a, b);
	lua_pushnumber(_lua_state, dot_product);

	return 1;
}
/******************************************************************************/

/* INTERFACE - COLLISION *****************************************************/
EXPORT_LUA_FUNCTION(Lua_PointAndBoxCollision)
{
	lua_settop(_lua_state, 3);

	int x = CAST(int, luaL_checknumber(_lua_state, 1));
	int y = CAST(int, luaL_checknumber(_lua_state, 2));
	luaL_checktype(_lua_state, 3, LUA_TTABLE);

	lua_getfield(_lua_state, 3, "x");
	lua_getfield(_lua_state, 3, "y");
	lua_getfield(_lua_state, 3, "w");
	lua_getfield(_lua_state, 3, "h");

	SDL_Rect box;
	box.x = CAST(int, luaL_checknumber(_lua_state, -4));
	box.w = CAST(int, luaL_checknumber(_lua_state, -2));
	box.y = CAST(int, luaL_checknumber(_lua_state, -3));
	box.h = CAST(int, luaL_checknumber(_lua_state, -1));

	lua_pop(_lua_state, 4);

	bool result = PointAndBoxCollision(x, y, box);
	lua_pushboolean(_lua_state, result);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_BoxAndBoxCollision)
{
	lua_settop(_lua_state, 2);

	luaL_checktype(_lua_state, 1, LUA_TTABLE);
	luaL_checktype(_lua_state, 2, LUA_TTABLE);

	lua_getfield(_lua_state, 1, "x");
	lua_getfield(_lua_state, 1, "y");
	lua_getfield(_lua_state, 1, "w");
	lua_getfield(_lua_state, 1, "h");
	lua_getfield(_lua_state, 2, "x");
	lua_getfield(_lua_state, 2, "y");
	lua_getfield(_lua_state, 2, "w");
	lua_getfield(_lua_state, 2, "h");

	SDL_Rect a, b;
	a.x = CAST(int, luaL_checknumber(_lua_state, -8));
	a.y = CAST(int, luaL_checknumber(_lua_state, -7));
	a.w = CAST(int, luaL_checknumber(_lua_state, -6));
	a.h = CAST(int, luaL_checknumber(_lua_state, -5));
	b.x = CAST(int, luaL_checknumber(_lua_state, -4));
	b.y = CAST(int, luaL_checknumber(_lua_state, -3));
	b.w = CAST(int, luaL_checknumber(_lua_state, -2));
	b.h = CAST(int, luaL_checknumber(_lua_state, -1));

	lua_pop(_lua_state, 8);

	bool result = BoxAndBoxCollision(a, b);
	lua_pushboolean(_lua_state, result);

	return 1;
}
/******************************************************************************/

/* INTERFACE - UTILITY ********************************************************/
EXPORT_LUA_FUNCTION(Lua_MakeColor)
{
	lua_settop(_lua_state, 4);

	Color color;
	color.component.r = CAST(u8, luaL_checknumber(_lua_state, 1));
	color.component.g = CAST(u8, luaL_checknumber(_lua_state, 2));
	color.component.b = CAST(u8, luaL_checknumber(_lua_state, 3));
	color.component.a = CAST(u8, luaL_checknumber(_lua_state, 4));

	color.numeric = SDL_SwapBE32(color.numeric);

	lua_pushinteger(_lua_state, color.numeric);
	lua_tolstring(_lua_state, -1, NULL);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_MakeRGBA)
{
	lua_settop(_lua_state, 1);

	Color color;
	color.numeric = SDL_SwapBE32(luaL_checkinteger(_lua_state, 1));

	lua_pushinteger(_lua_state, color.component.r);
	lua_pushinteger(_lua_state, color.component.g);
	lua_pushinteger(_lua_state, color.component.b);
	lua_pushinteger(_lua_state, color.component.a);

	return 4;
}
EXPORT_LUA_FUNCTION(Lua_ConsolePrint)
{
	lua_settop(_lua_state, 1);

	const char* text = luaL_checkstring(_lua_state, 1);
	AddConsoleHistoryElement(String(text));

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_GetFPS)
{
	lua_pushnumber(_lua_state, current_fps);
	return 1;
}
EXPORT_LUA_FUNCTION(Lua_RandomSeed)
{
	srand(time(NULL));
	int seed = rand();

	lua_pushinteger(_lua_state, seed);

	return 1;
}
/******************************************************************************/

/* INTERFACE - ZONE ***********************************************************/
EXPORT_LUA_FUNCTION(Lua_LoadZoneFromFile)
{
	lua_settop(_lua_state, 1);

	// Loads into global current_zone (found in zone.h).
	const char* filename = luaL_checkstring(_lua_state, 1);
	LoadZoneFromFile(filename);

	lua_newtable(_lua_state);

	lua_pushnumber(_lua_state, current_zone.width);
	lua_setfield(_lua_state, -2, "width");
	lua_pushnumber(_lua_state, current_zone.height);
	lua_setfield(_lua_state, -2, "height");

	lua_pushinteger(_lua_state, current_zone.seed);
	lua_setfield(_lua_state, -2, "seed");

	lua_newtable(_lua_state);

	for (int i=0; i<(current_zone.width*current_zone.height); ++i) {
		lua_pushinteger(_lua_state, i+1);
		lua_newtable(_lua_state);

		lua_pushliteral(_lua_state, "id");
		lua_pushinteger(_lua_state, current_zone.tile_data[i].id);
		lua_settable(_lua_state, -3);

		lua_pushliteral(_lua_state, "mask_flags");
		lua_pushinteger(_lua_state, current_zone.tile_data[i].mask_flags);
		lua_settable(_lua_state, -3);

		lua_settable(_lua_state, -3);
	}

	lua_setfield(_lua_state, -2, "tile_data");

	return 1;
}
/******************************************************************************/

/* INTERFACE - LIGHTING *******************************************************/
EXPORT_LUA_FUNCTION(Lua_TurnLightingOn)
{
	lua_settop(_lua_state, 1);
	lighting_enabled = lua_toboolean(_lua_state, 1);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_RenderLighting)
{
	RenderLighting();

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_AddLight)
{
	lua_settop(_lua_state, 4);

	int   x       = CAST(int,  luaL_checknumber(_lua_state, 1));
	int   y       = CAST(int,  luaL_checknumber(_lua_state, 2));
	float radius  =            luaL_checknumber(_lua_state, 3);
	float flicker =            luaL_checknumber(_lua_state, 4);
	bool  active  = CAST(bool, luaL_optinteger (_lua_state, 5, true));

	LightHandle handle = AddLight(x, y, radius, flicker, active);
	lua_pushinteger(_lua_state, handle);

	return 1;
}
EXPORT_LUA_FUNCTION(Lua_UpdateLight)
{
	lua_settop(_lua_state, 4);

	LightHandle handle = luaL_checkinteger(_lua_state, 1);
	if (handle >= light_list.count) { return 0; }

	Light& light_source = light_list[handle];

	light_source.x       = CAST(int, luaL_optnumber(_lua_state, 2, light_source.x));
	light_source.y       = CAST(int, luaL_optnumber(_lua_state, 3, light_source.y));
	light_source.radius  =           luaL_optnumber(_lua_state, 4, light_source.radius);
	light_source.flicker =           luaL_optnumber(_lua_state, 5, light_source.flicker);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_TurnOffLight)
{
	lua_settop(_lua_state, 2);

	LightHandle handle = luaL_checkinteger(_lua_state, 1);
	if (handle >= light_list.count) { return 0; }

	Light& light_source = light_list[handle];
	light_source.active = !lua_toboolean(_lua_state, 2);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_RemoveAllLights)
{
	RemoveAllLights();
	return 0;
}
EXPORT_LUA_FUNCTION(Lua_SetShadowColor)
{
	lua_settop(_lua_state, 1);

	Color color;
	color.numeric = SDL_SwapBE32(luaL_checkinteger(_lua_state, 1));

	shadow_color = color.component;

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_SetLightColor)
{
	lua_settop(_lua_state, 1);

	Color color;
	color.numeric = SDL_SwapBE32(luaL_checkinteger(_lua_state, 1));

	SetTextureColor(light_handle, color.component);

	return 0;
}
/******************************************************************************/

/******************************************************************************/
EXPORT_LUA_FUNCTION(Lua_BeginCamera)
{
	render_offset_enabled = true;
	return 0;
}
EXPORT_LUA_FUNCTION(Lua_EndCamera)
{
	render_offset_enabled = false;
	return 0;
}
EXPORT_LUA_FUNCTION(Lua_CameraTrack)
{
	lua_settop(_lua_state, 1);

	luaL_checktype(_lua_state, 1, LUA_TTABLE);

	lua_getfield(_lua_state,  1, "pos");
	lua_getfield(_lua_state,  1, "width");
	lua_getfield(_lua_state,  1, "height");

	luaL_checktype(_lua_state, -3, LUA_TTABLE);

	lua_getfield(_lua_state, -3, "x");
	lua_getfield(_lua_state, -4, "y");

	int x = CAST(int, luaL_checknumber(_lua_state, -2));
	int y = CAST(int, luaL_checknumber(_lua_state, -1));
	int w = CAST(int, luaL_checknumber(_lua_state, -4));
	int h = CAST(int, luaL_checknumber(_lua_state, -3));

	lua_pop(_lua_state, 5);

	render_offset_x = (x + (w / 2)) - (viewport_width / 2);
	render_offset_y = (y + (h / 2)) - (viewport_height / 2);

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_SetCameraPosition)
{
	lua_settop(_lua_state, 2);

	int x = CAST(int, luaL_checknumber(_lua_state, 1));
	int y = CAST(int, luaL_checknumber(_lua_state, 2));

	render_offset_x = x;
	render_offset_y = y;

	return 0;
}
EXPORT_LUA_FUNCTION(Lua_GetCameraPosition)
{
	lua_pushinteger(_lua_state, render_offset_x);
	lua_pushinteger(_lua_state, render_offset_y);

	return 2;
}
/******************************************************************************/

/* INTERFACE - PATHFINDING ****************************************************/
EXPORT_LUA_FUNCTION(Lua_FindPath)
{
	lua_settop(_lua_state, 2);

	luaL_checktype(_lua_state, 1, LUA_TTABLE);
	luaL_checktype(_lua_state, 2, LUA_TTABLE);

	lua_getfield(_lua_state, 1, "x");
	lua_getfield(_lua_state, 1, "y");
	lua_getfield(_lua_state, 2, "x");
	lua_getfield(_lua_state, 2, "y");

	Point start, end;
	start.x = CAST(int, luaL_checknumber(_lua_state, -4));
	start.y = CAST(int, luaL_checknumber(_lua_state, -3));
	end.x   = CAST(int, luaL_checknumber(_lua_state, -2));
	end.y   = CAST(int, luaL_checknumber(_lua_state, -1));

	lua_pop(_lua_state, 4);

	Array<Point> path = FindPath(start, end);

	lua_newtable(_lua_state);

	for (int i=path.count-1; i>=0; --i) {
		lua_newtable(_lua_state);
		lua_pushinteger(_lua_state, path[i].x);
		lua_setfield(_lua_state, -2, "x");
		lua_pushinteger(_lua_state, path[i].y);
		lua_setfield(_lua_state, -2, "y");

		lua_rawseti(_lua_state, -2, path.count - i);
	}

	return 1;
}
/******************************************************************************/

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
