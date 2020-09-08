/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void FreeTexture (TextureHandle _texture_handle)
{
	// Do not bother handling invalid texture handles.
	if (_texture_handle >= texture_list.count) { return; }

	Texture& texture = texture_list[_texture_handle];
	if (texture.data) {
		SDL_DestroyTexture(texture.data);
		texture.data = NULL;
	}
}
INLDEF void FreeTextureList ()
{
	// Go through and free all of the allocated textures.
	for (size_t i=0; i<texture_list.count; ++i) { FreeTexture(i); }
}

STDDEF TextureHandle CreateTextureTarget (int _width, int _height)
{
	TextureHandle texture_handle = texture_list.count;
	Texture& texture = texture_list.AddElement(Texture {0});

	texture.width = _width, texture.height = _height;
	texture.data = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, _width, _height);
	if (!texture.data) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to create a blank texture target!");
	}

	return texture_handle;
}

STDDEF void LoadTextureFromFile_Internal (TextureHandle& _texture_handle, const char* _filename)
{
	// If we don't want to reload a texture specified by a specific handle then
	// assign the invalid handle a value (at the end of the texture list).
	if (_texture_handle == INVALID_ASSET_HANDLE) {
		_texture_handle = texture_list.count;
		texture_list.AddElement(Texture {0});
	}

	// Free the old texture data to avoid memory leaks.
	Texture& texture = texture_list[_texture_handle];
	FreeTexture(_texture_handle);

	int width = 0, height = 0;
	SDL_Surface* surface = NULL;
	u8* raw_pixel_data = NULL;

	// We're using stb_image because SDL can only load BMP files natively.
	raw_pixel_data = stbi_load(_filename, &width, &height, NULL, Texture::BYTES_PER_PIXEL);
	if (!raw_pixel_data) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Failed to load texture data for '%s'!", _filename);
	} else {
		int pitch = width * Texture::BYTES_PER_PIXEL;
		surface = SDL_CreateRGBSurfaceWithFormatFrom(CAST(void*, raw_pixel_data),
			width, height, Texture::BITS_PER_PIXEL, pitch, SDL_PIXELFORMAT_RGBA32);
		if (!surface) {
			TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to create surface data for '%s'!", _filename);
		} else {
			texture.data = SDL_CreateTextureFromSurface(renderer, surface);
			if (!texture.data) {
				TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to create texture '%s'!", _filename);
			} else {
				texture.width = width;
				texture.height = height;

				// Register the asset with the hotloader, if not already registered.
				RegisterAssetForHotloading(_filename, ASSET_TEXTURE, _texture_handle);
			}
		}
	}

	stbi_image_free(raw_pixel_data);
	SDL_FreeSurface(surface);
}

INLDEF TextureHandle LoadTextureFromFile (const char* _filename)
{
	TextureHandle texture_handle = INVALID_ASSET_HANDLE;
	LoadTextureFromFile_Internal(texture_handle, _filename);

	return texture_handle;
}

INLDEF void RenderTexture (TextureHandle _texture_handle, int _x, int _y, float _scale_x, float _scale_y, float _angle, int _anchor_x, int _anchor_y, SDL_RendererFlip _flip, const SDL_Rect* _clip)
{
	// Do not bother handling invalid texture handles.
	if (_texture_handle >= texture_list.count) { return; }
	const Texture& texture = texture_list[_texture_handle];

	int scaled_width = CAST(int, CAST(float, (_clip) ? _clip->w : texture.width) * _scale_x);
	int scaled_height = CAST(int, CAST(float, (_clip) ? _clip->h : texture.height) * _scale_y);

	SDL_Rect destination = { _x, _y, scaled_width, scaled_height };
	SDL_Point anchor = {_anchor_x, _anchor_y };

	// If the render offset system is currently enabled offset all renders by its values.
	if (render_offset_enabled) { destination.x -= render_offset_x, destination.y -= render_offset_y; }

	float angle = ToDegrees(_angle); // We are using radians but SDL wants degrees.
	if (SDL_RenderCopyEx(renderer, texture.data, _clip, &destination, angle, &anchor, _flip) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to render a texture!");
	}
}

INLDEF void RenderAnimatedTexture (TextureHandle _texture_handle, AnimationHandle _animation_handle, int _x, int _y, float _scale_x, float _scale_y, float _angle, int _anchor_x, int _anchor_y, SDL_RendererFlip _flip)
{
	SDL_Rect animation_keyframe = GetCurrentAnimationKeyFrame(_animation_handle);
	RenderTexture(_texture_handle, _x, _y, _scale_x, _scale_y, _angle, _anchor_x, _anchor_y, _flip, &animation_keyframe);
}

INLDEF void SetTextureColor (TextureHandle _texture_handle, const SDL_Color& _color)
{
	// Do not bother handling invalid texture handles.
	if (_texture_handle >= texture_list.count) { return; }
	Texture& texture = texture_list[_texture_handle];

	// @ERROR: WE COULD DO ERROR LOGGING HERE?!?!?
	SDL_SetTextureColorMod(texture.data, _color.r, _color.g, _color.b);
	SDL_SetTextureAlphaMod(texture.data, _color.a);
}

INLDEF int GetTextureWidth (TextureHandle _texture_handle)
{
	// Do not bother handling invalid texture handles.
	if (_texture_handle >= texture_list.count) { return 0; }
	Texture& texture = texture_list[_texture_handle];

	// @ERROR: WE COULD DO ERROR LOGGING HERE?!?!?
	int texture_width;
	SDL_QueryTexture(texture.data, NULL, NULL, &texture_width, NULL);
	return texture_width;

}
INLDEF int GetTextureHeight (TextureHandle _texture_handle)
{
	// Do not bother handling invalid texture handles.
	if (_texture_handle >= texture_list.count) { return 0; }
	Texture& texture = texture_list[_texture_handle];

	// @ERROR: WE COULD DO ERROR LOGGING HERE?!?!?
	int texture_height;
	SDL_QueryTexture(texture.data, NULL, NULL, NULL, &texture_height);
	return texture_height;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
