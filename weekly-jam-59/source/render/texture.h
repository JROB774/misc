/*******************************************************************************
 * TEXTURE
 * Systems for loading, handling, and rendering textures.
 *
*******************************************************************************/

#ifndef __TCE_TEXTURE_H__ /*//////////////////////////////////////////////////*/
#define __TCE_TEXTURE_H__

namespace TCE
{

GLOBAL constexpr const char* TEXTURE_PATH = "data/textures/";

struct Texture
{
	static constexpr int BYTES_PER_PIXEL = 4;
	static constexpr int BITS_PER_PIXEL = BYTES_PER_PIXEL * 8;

	int width = 0, height = 0;
	SDL_Texture* data = NULL;
};

GLOBAL Array<Texture> texture_list;

GLOBAL int render_offset_x = 0, render_offset_y = 0;
GLOBAL bool render_offset_enabled = false;

INLDEF void FreeTexture (TextureHandle _texture_handle);
INLDEF void FreeTextureList ();

STDDEF TextureHandle CreateTextureTarget (int _width, int _height);

STDDEF void LoadTextureFromFile_Internal (TextureHandle& _texture_handle, const char* _filename);
INLDEF TextureHandle LoadTextureFromFile (const char* _filename);

INLDEF void RenderTexture (TextureHandle _texture_handle, int _x, int _y, float _scale_x = 1.0f, float _scale_y = 1.0f, float _angle = 0.0f, int _anchor_x = 0, int _anchor_y = 0, SDL_RendererFlip _flip = SDL_FLIP_NONE, const SDL_Rect* _clip = NULL);
INLDEF void RenderAnimatedTexture (TextureHandle _texture_handle, AnimationHandle _animation_handle, int _x, int _y, float _scale_x = 1.0f, float _scale_y = 1.0f, float _angle = 0.0f, int _anchor_x = 0, int _anchor_y = 0, SDL_RendererFlip _flip = SDL_FLIP_NONE);

INLDEF void SetTextureColor (TextureHandle _texture_handle, const SDL_Color& _color);

INLDEF int GetTextureWidth (TextureHandle _texture_handle);
INLDEF int GetTextureHeight (TextureHandle _texture_handle);

} // TCE

#endif /* __TCE_TEXTURE_H__ //////////////////////////////////////////////////*/
