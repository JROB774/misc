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

-- INTERFACE - TEXTURE
TCE.LoadTextureFromFile      = TCE_LoadTextureFromFile
TCE.RenderTexture            = TCE_RenderTexture
TCE.RenderAnimatedTexture    = TCE_RenderAnimatedTexture
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

return TCE
