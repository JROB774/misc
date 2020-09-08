--[[----------------------------------------------------------------------------

 The entire TCE library implementation containing various constants, enums,
 functions, etc. to be used by the game code. This library table should be
 required locally at the start of 'main.lua' for easy access to the various
 TCE library features and utilities -- without slow global access.

----------------------------------------------------------------------------]]--

local TCE = {}

TCE.Keycodes = require("data.scripts.tce.keys")
TCE.Buttons = require("data.scripts.tce.buttons")
TCE.MathConstants = require("data.scripts.tce.maths")
TCE.RenderFlips = require("data.scripts.tce.flip")

TCE.Private = {}

-- IMPORTS FROM C/C++ ----------------------------------------------------------

-- INTERFACE - PRIVATE
TCE.Private.ReloadGame       = TCE_ReloadGame
TCE.Private.ClearConsole     = TCE_ClearConsole
-- INTERFACE - TEXTURE
TCE.LoadTextureFromFile      = TCE_LoadTextureFromFile
TCE.RenderTexture            = TCE_RenderTexture
TCE.RenderAnimatedTexture    = TCE_RenderAnimatedTexture
TCE.RenderClippedTexture     = TCE_RenderClippedTexture
TCE.SetTextureColor          = TCE_SetTextureColor
TCE.GetTextureWidth          = TCE_GetTextureWidth
TCE.GetTextureHeight         = TCE_GetTextureHeight
-- INTERFACE - ANIMATION
TCE.LoadAnimationFromFile    = TCE_LoadAnimationFromFile
TCE.PauseAnimation           = TCE_PauseAnimation
TCE.AnimationIsPaused        = TCE_AnimationIsPaused
TCE.ResetAnimation           = TCE_ResetAnimation
TCE.GetAnimationFrameWidth   = TCE_GetAnimationFrameWidth
TCE.GetAnimationFrameHeight  = TCE_GetAnimationFrameHeight
TCE.PauseAllAnimations       = TCE_PauseAllAnimations
-- INTERFACE - FONT
TCE.LoadFontFromFile         = TCE_LoadFontFromFile
TCE.RenderText               = TCE_RenderText
TCE.SetFontColor             = TCE_SetFontColor
TCE.GetFontGlyphWidth        = TCE_GetFontGlyphWidth
TCE.GetFontGlyphHeight       = TCE_GetFontGlyphHeight
TCE.GetTextWidth             = TCE_GetTextWidth
TCE.GetTextHeight            = TCE_GetTextHeight
-- INTERFACE - SOUND
TCE.LoadSoundFromFile        = TCE_LoadSoundFromFile
TCE.PlaySound                = TCE_PlaySound
TCE.StopSound                = TCE_StopSound
-- INTERFACE - MUSIC
TCE.PlayMusic                = TCE_PlayMusic
TCE.StopMusic                = TCE_StopMusic
TCE.FadeInMusic              = TCE_FadeInMusic
TCE.FadeOutMusic             = TCE_FadeOutMusic
-- INTERFACE - EVENT
TCE.GetMousePosition         = TCE_GetMousePosition
TCE.GetRelativeMousePosition = TCE_GetRelativeMousePosition
TCE.GetGlobalMousePosition   = TCE_GetGlobalMousePosition
TCE.IsKeyDown                = TCE_IsKeyDown
TCE.IsKeyUp                  = TCE_IsKeyUp
TCE.KeyPressed               = TCE_KeyPressed
TCE.KeyReleased              = TCE_KeyReleased
TCE.IsButtonDown             = TCE_IsButtonDown
TCE.IsButtonUp               = TCE_IsButtonUp
TCE.ButtonPressed            = TCE_ButtonPressed
TCE.ButtonReleased           = TCE_ButtonReleased
-- INTERFACE - PRIMITIVE
TCE.RenderPoint              = TCE_RenderPoint
TCE.RenderLine               = TCE_RenderLine
TCE.RenderRectOutline        = TCE_RenderRectOutline
TCE.RenderRectFilled         = TCE_RenderRectFilled
-- INTERFACE - WINDOW
TCE.GetViewportWidth         = TCE_GetViewportWidth
TCE.GetViewportHeight        = TCE_GetViewportHeight
TCE.ShowCursor               = TCE_ShowCursor
TCE.Exit                     = TCE_Exit
-- INTERFACE - VECTOR2D
TCE.GetVectorLength          = TCE_GetVectorLength
TCE.NormalizeVector          = TCE_NormalizeVector
TCE.RotateVector             = TCE_RotateVector
TCE.VectorDotProduct         = TCE_VectorDotProduct
-- INTERFACE - COLLISION
TCE.PointAndBoxCollision     = TCE_PointAndBoxCollision
TCE.BoxAndBoxCollision       = TCE_BoxAndBoxCollision
-- INTERFACE - UTILITY
TCE.MakeColor                = TCE_MakeColor
TCE.MakeRGBA                 = TCE_MakeRGBA
TCE.ConsolePrint             = TCE_ConsolePrint
TCE.GetFPS                   = TCE_GetFPS
-- INTERFACE - ROOM
TCE.LoadZoneFromFile         = TCE_LoadZoneFromFile
-- INTERFACE - LIGHTING
TCE.TurnLightingOn           = TCE_TurnLightingOn
TCE.RenderLighting           = TCE_RenderLighting
TCE.AddLight                 = TCE_AddLight
TCE.UpdateLight              = TCE_UpdateLight
TCE.RemoveLight              = TCE_RemoveLight
TCE.RemoveAllLights          = TCE_RemoveAllLights
TCE.SetShadowColor           = TCE_SetShadowColor
TCE.SetLightColor            = TCE_SetLightColor
-- INTERFACE - CAMERA
TCE.BeginCamera              = TCE_BeginCamera
TCE.EndCamera                = TCE_EndCamera
TCE.CameraTrack              = TCE_CameraTrack

--------------------------------------------------------------------------------

TCE.Lerp = function (source, destination, step)
	return (1.0 - step) * source + step * destination
end

TCE.RoundNum = function (target, decimalPoints)
	target = target * 10^decimalPoints
	num = target % 1 >= 0.5 and math.ceil(target) or math.floor(target)
	return (num / (10^decimalPoints))
end

TCE.Round = function (value)
	return math.floor(value + 0.5)
end

TCE.lookAtVector = function (target, inital)
    length = math.sqrt((target.x - inital.x )^2 + (target.y - inital.y)^2)
    xDiff = (target.x - inital.x) / length
    yDiff = (target.y - inital.y) / length

    return {x = xDiff, y = yDiff, length = length}

end

return TCE
