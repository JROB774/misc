/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void LightingInit ()
{
	light_map_handle = CreateTextureTarget(viewport_width, viewport_height);
	light_handle = LoadTextureFromFile("data/textures/light.png");

	light_map = &texture_list[light_map_handle];
	light = &texture_list[light_handle];

	// We want the light map to multiply with the current screenc contents.
	if (SDL_SetTextureBlendMode(light_map->data, SDL_BLENDMODE_MOD) != 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_SDL, "Failed to initialise lighting!");

		// If lighting cannot be set up then we don't want these set.
		light_map = NULL;
		light = NULL;
	} else {
		lighting_enabled = true;
	}
}
INLDEF void LightingQuit ()
{
	lighting_enabled = false;

	light_map = NULL;
	light = NULL;
}

INLDEF void RenderLight (LightHandle _light_handle, const Light& _light)
{
	// Do not render lights that are not currently active.
	if (!_light.active) { return; }

	float full_radius = light->width / 2;
	float flicker_radius = _light.radius + RandomFloat(0.0f, _light.flicker);

	int   x     = _light.x - CAST(int, flicker_radius);
	int   y     = _light.y - CAST(int, flicker_radius);
	float scale = (flicker_radius / full_radius);

	RenderTexture(_light_handle, x, y, scale, scale);
}

INLDEF void RenderLighting ()
{
	// We only want to render the lighting when it is turned on.
	if (!lighting_enabled) { return; }
	// We do not want to render light if it could not be initialised.
	if (!light_map || !light) { return; }

	render_offset_enabled = true;

	// Clear the light map of its previous contents.
	SetRendererTarget(light_map->data);
	SetRendererColor(shadow_color);

	SDL_RenderClear(renderer);

	// Render all of the active lights to the light map.
	for (size_t i=0; i<light_list.count; ++i) {
		const Light& light_source = light_list[i];
		RenderLight(light_handle, light_source);
	}

	render_offset_enabled = false;

	// Render the entire light map to the screen.
	SetRendererTarget(screen);
	RenderTexture(light_map_handle, 0, 0);
}

INLDEF LightHandle AddLight (int _x, int _y, float _radius, float _flicker, float _active)
{
	Light light_source;

	light_source.x       = _x;
	light_source.y       = _y;
	light_source.radius  = _radius;
	light_source.flicker = _flicker;
	light_source.active  = _active;

	LightHandle handle = light_list.count;
	light_list.AddElement(light_source);

	return handle;
}

INLDEF void RemoveAllLights ()
{
	light_list.Clear();
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
