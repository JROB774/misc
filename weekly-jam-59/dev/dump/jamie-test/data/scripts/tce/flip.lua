--[[----------------------------------------------------------------------------

 This is a list of various flip values that can be passed to many TCE render
 functions. These values map one-to-one with the SDL_RendererFlip values.

----------------------------------------------------------------------------]]--

local TCE_RenderFlip =
{
	NONE = 0,
	HORZ = 1,
	VERT = 2,
	BOTH = 3
}

return TCE_RenderFlip
