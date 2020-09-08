/*******************************************************************************
 * EXPORT
 * A list of all the engine functions to be exported from C into Lua.
 *
*******************************************************************************/

#ifndef __TCE_EXPORT_H__ /*///////////////////////////////////////////////////*/
#define __TCE_EXPORT_H__

// Definition for functions to be exposed to the Lua side of the engine.
#define LUADEF extern "C" static

// Macros to speed up the process of writing out function exports.
#define EXPORT_LUA_FUNCTION(__function_name) LUADEF int __function_name(lua_State* _lua_state)
#define REGISTER_LUA_FUNCTION(__lua_state, __lua_name, __c_name) lua_register(__lua_state, __lua_name, __c_name)

namespace TCE
{

INLDEF void RegisterLuaExports (lua_State* _lua_state);

// INTERFACE - PRIVATE
EXPORT_LUA_FUNCTION(Lua_ReloadGame);
EXPORT_LUA_FUNCTION(Lua_ClearConsole);
EXPORT_LUA_FUNCTION(Lua_SetViewport);
// INTERFACE - TEXTURE
EXPORT_LUA_FUNCTION(Lua_LoadTextureFromFile);
EXPORT_LUA_FUNCTION(Lua_RenderTexture);
EXPORT_LUA_FUNCTION(Lua_RenderAnimatedTexture);
EXPORT_LUA_FUNCTION(Lua_RenderClippedTexture);
EXPORT_LUA_FUNCTION(Lua_SetTextureColor);
EXPORT_LUA_FUNCTION(Lua_GetTextureWidth);
EXPORT_LUA_FUNCTION(Lua_GetTextureHeight);
// INTERFACE - ANIMATION
EXPORT_LUA_FUNCTION(Lua_LoadAnimationFromFile);
EXPORT_LUA_FUNCTION(Lua_PauseAnimation);
EXPORT_LUA_FUNCTION(Lua_AnimationIsPaused);
EXPORT_LUA_FUNCTION(Lua_ResetAnimation);
EXPORT_LUA_FUNCTION(Lua_GetAnimationFrameWidth);
EXPORT_LUA_FUNCTION(Lua_GetAnimationFrameHeight);
EXPORT_LUA_FUNCTION(Lua_PauseAllAnimations);
EXPORT_LUA_FUNCTION(Lua_GetAnimationLength);
EXPORT_LUA_FUNCTION(Lua_AnimationIsDone);
// INTERFACE - FONT
EXPORT_LUA_FUNCTION(Lua_LoadFontFromFile);
EXPORT_LUA_FUNCTION(Lua_RenderText);
EXPORT_LUA_FUNCTION(Lua_RenderShadowedText);
EXPORT_LUA_FUNCTION(Lua_SetFontColor);
EXPORT_LUA_FUNCTION(Lua_GetFontGlyphWidth);
EXPORT_LUA_FUNCTION(Lua_GetFontGlyphHeight);
EXPORT_LUA_FUNCTION(Lua_GetTextWidth);
EXPORT_LUA_FUNCTION(Lua_GetTextHeight);
// INTERFACE - SOUND
EXPORT_LUA_FUNCTION(Lua_LoadSoundFromFile);
EXPORT_LUA_FUNCTION(Lua_PlaySound);
EXPORT_LUA_FUNCTION(Lua_StopSound);
// INTERFACE - MUSIC
EXPORT_LUA_FUNCTION(Lua_PlayMusic);
EXPORT_LUA_FUNCTION(Lua_StopMusic);
EXPORT_LUA_FUNCTION(Lua_FadeInMusic);
EXPORT_LUA_FUNCTION(Lua_FadeOutMusic);
// INTERFACE - EVENT
EXPORT_LUA_FUNCTION(Lua_GetMousePosition);
EXPORT_LUA_FUNCTION(Lua_GetRelativeMousePosition);
EXPORT_LUA_FUNCTION(Lua_GetGlobalMousePosition);
EXPORT_LUA_FUNCTION(Lua_IsKeyDown);
EXPORT_LUA_FUNCTION(Lua_IsKeyUp);
EXPORT_LUA_FUNCTION(Lua_KeyPressed);
EXPORT_LUA_FUNCTION(Lua_KeyReleased);
EXPORT_LUA_FUNCTION(Lua_IsButtonDown);
EXPORT_LUA_FUNCTION(Lua_IsButtonUp);
EXPORT_LUA_FUNCTION(Lua_ButtonPressed);
EXPORT_LUA_FUNCTION(Lua_ButtonReleased);
// INTERFACE - PRIMITIVE
EXPORT_LUA_FUNCTION(Lua_RenderPoint);
EXPORT_LUA_FUNCTION(Lua_RenderLine);
EXPORT_LUA_FUNCTION(Lua_RenderRectOutline);
EXPORT_LUA_FUNCTION(Lua_RenderRectFilled);
// INTERFACE - WINDOW
EXPORT_LUA_FUNCTION(Lua_GetViewportWidth);
EXPORT_LUA_FUNCTION(Lua_GetViewportHeight);
EXPORT_LUA_FUNCTION(Lua_ShowCursor);
EXPORT_LUA_FUNCTION(Lua_Exit);
// INTERFACE - VECTOR2D
EXPORT_LUA_FUNCTION(Lua_GetVectorLength);
EXPORT_LUA_FUNCTION(Lua_NormalizeVector);
EXPORT_LUA_FUNCTION(Lua_RotateVector);
EXPORT_LUA_FUNCTION(Lua_VectorDotProduct);
// INTERFACE - COLLISION
EXPORT_LUA_FUNCTION(Lua_PointAndBoxCollision);
EXPORT_LUA_FUNCTION(Lua_BoxAndBoxCollision);
// INTERFACE - UTILITY
EXPORT_LUA_FUNCTION(Lua_MakeColor);
EXPORT_LUA_FUNCTION(Lua_MakeRGBA);
EXPORT_LUA_FUNCTION(Lua_ConsolePrint);
EXPORT_LUA_FUNCTION(Lua_GetFPS);
EXPORT_LUA_FUNCTION(Lua_RandomSeed);
// INTERFACE - ZONE
EXPORT_LUA_FUNCTION(Lua_LoadZoneFromFile);
// INTERFACE - LIGHTING
EXPORT_LUA_FUNCTION(Lua_TurnLightingOn);
EXPORT_LUA_FUNCTION(Lua_RenderLighting);
EXPORT_LUA_FUNCTION(Lua_AddLight);
EXPORT_LUA_FUNCTION(Lua_UpdateLight);
EXPORT_LUA_FUNCTION(Lua_TurnOffLight);
EXPORT_LUA_FUNCTION(Lua_RemoveAllLights);
EXPORT_LUA_FUNCTION(Lua_SetShadowColor);
EXPORT_LUA_FUNCTION(Lua_SetLightColor);
// INTERFACE - CAMERA
EXPORT_LUA_FUNCTION(Lua_BeginCamera);
EXPORT_LUA_FUNCTION(Lua_EndCamera);
EXPORT_LUA_FUNCTION(Lua_CameraTrack);
EXPORT_LUA_FUNCTION(Lua_SetCameraPosition);
EXPORT_LUA_FUNCTION(Lua_GetCameraPosition);
// INTERFACE - PATHFINDING
EXPORT_LUA_FUNCTION(Lua_FindPath);

} // TCE

#endif /* __TCE_EXPORT_H__ ///////////////////////////////////////////////*/
