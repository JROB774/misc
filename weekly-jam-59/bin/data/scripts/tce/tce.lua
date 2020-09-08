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
TCE.Private.SetViewport      = TCE_SetViewport
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
TCE.GetAnimationLength       = TCE_GetAnimationLength
TCE.AnimationIsDone          = TCE_AnimationIsDone
-- INTERFACE - FONT
TCE.LoadFontFromFile         = TCE_LoadFontFromFile
TCE.RenderText               = TCE_RenderText
TCE.RenderShadowedText       = TCE_RenderShadowedText
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
TCE.RandomSeed               = TCE_RandomSeed
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
TCE.SetCameraPosition        = TCE_SetCameraPosition
TCE.GetCameraPosition        = TCE_GetCameraPosition
-- INTERFACE - PATHFINDING
TCE.FindPath                 = TCE_FindPath

--------------------------------------------------------------------------------

TCE.DumpTable = function (table)
  if type(table) == 'table' then
    local s = '{ '
    for k,v in pairs(table) do
      if type(k) ~= 'number' then k = '"'..k..'"' end
      s = s .. '['..k..'] = ' .. TCE.DumpTable(v) .. ','
    end
    return s .. '} '
  else
    return tostring(table)
  end
end

TCE.GetCameraBounds = function (table)
  local x, y = TCE.GetCameraPosition()
  return { x = x, y = y, w = TCE.GetViewportWidth(), h = TCE.GetViewportHeight() }
end

TCE.TableLength = function (table)
  local count = 0
  for _ in pairs(table) do
    count = count + 1
  end
  return count
end

TCE.TableLengthWhereItem = function (table, condition)
  local count = 0
  for i in pairs(table) do
    if condition(table[i]) then count = count + 1 end
  end
  return count
end

TCE.TestBitFlag = function (bitset, flag)
  return bitset % (2 * flag) >= flag
end

TCE.ClearBtiFlag = function (bitset, flag)
  if TCE.TestBitFlag(bitset, flag) then return bitset - flag
    else return bitset end
end

TCE.SetBitFlag = function (bitset, flag)
  if TCE.TestBitFlag(bitset, flag) then return bitset
  else return bitset + flag end
end

TCE.Normalize = function (value, min, max)
  return (value - min) / (max - min)
end

TCE.LowWeightedRandom = function (min, max)
  return math.floor(min + (max - min) * math.random() ^ 2)
end

TCE.RealNumberRandom = function (min, max)
  return math.random() * (max - min) + min
end

TCE.DeepCopy = function (o, seen)
  seen = seen or {}
  if o == nil then return nil end
  if seen[o] then return seen[o] end

  local no
  if type(o) == 'table' then
    no = {}
    seen[o] = no

    for k, v in next, o, nil do
      no[TCE.DeepCopy(k, seen)] = TCE.DeepCopy(v, seen)
    end
    setmetatable(no, TCE.DeepCopy(getmetatable(o), seen))
  else -- Number, String, Boolean, etc.
    no = o
  end
  return no
end

TCE.DeepCopyTable = function (o, seen)
  seen = seen or {}
  if o == nil then return nil end
  if seen[o] then return seen[o] end

  local no = {}
  seen[o] = no
  setmetatable(no, TCE.DeepCopyTable(getmetatable(o), seen))

  for k, v in next, o, nil do
    k = (type(k) == 'table') and TCE.DeepCopyTable(k, seen) or k
    v = (type(v) == 'table') and TCE.DeepCopyTable(v, seen) or v
    no[k] = v
  end
  return no
end

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

TCE.LookAtVector = function (target, inital)
    length = math.sqrt((target.x - inital.x )^2 + (target.y - inital.y)^2)
    xDiff = (target.x - inital.x) --/ length
    yDiff = (target.y - inital.y) --/ length

    return {x = xDiff, y = yDiff, length = length}
end

TCE.InnerProduct = function (target, inital)
  -- Doesn't work with 0,0 because of maths.
  xy = (target.x * inital.x + target.y * inital.y)
  x = (math.abs(target.x) ^ 2 + math.abs(target.y) ^ 2)^0.5
  y = (math.abs(inital.x) ^ 2 + math.abs(inital.y) ^ 2)^0.5
  theta = math.acos(xy/(x * y))
  return theta
end

return TCE
