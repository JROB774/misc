/*******************************************************************************
 * LIGHTING
 * Handles rendering the lighting, as well as managing different light sources.
 *
*******************************************************************************/

#ifndef __TCE_LIGHTING_H__ /*/////////////////////////////////////////////////*/
#define __TCE_LIGHTING_H__

namespace TCE
{

struct Light
{
	int x, y;

	float radius; // How large the light is.
	float flicker; // How much the light flickers.

	bool active;
};

GLOBAL TextureHandle light_map_handle;
GLOBAL TextureHandle light_handle;

// We cache the actual textures for easier/faster access.
GLOBAL Texture* light_map;
GLOBAL Texture* light;

GLOBAL SDL_Color shadow_color = { 0, 0, 0, 255 };

GLOBAL Array<Light> light_list;
GLOBAL bool lighting_enabled;

INLDEF void LightingInit ();
INLDEF void LightingQuit ();

INLDEF void RenderLighting ();

INLDEF LightHandle AddLight (int _x, int _y, float _radius, float _strength, float _active);

INLDEF void RemoveAllLights ();

} // TCE

#endif /* __TCE_LIGHTING_H__ /////////////////////////////////////////////////*/
