/*******************************************************************************
 * ANIMATION
 * Systems for loading, and playing, simple frame-by-frame animations.
 *
*******************************************************************************/

#ifndef __TCE_ANIMATION_H__ /*////////////////////////////////////////////////*/
#define __TCE_ANIMATION_H__

namespace TCE
{

GLOBAL constexpr const char* ANIMATION_PATH = "data/animations/";

struct Animation
{
	int frame_width;
	int frame_height;

	int texture_offset_x;
	int texture_offset_y;

	Array<float> key_frame;
	float timer;

	bool paused;
	bool looped;
};

GLOBAL Array<Animation> animation_list;
GLOBAL bool all_animations_paused = false;

STDDEF void LoadAnimationFromFile_Internal (AnimationHandle& _animation_handle, const char* _filename);
INLDEF AnimationHandle LoadAnimationFromFile (const char* _filename);

INLDEF void UpdateAnimationTimers (float _dt);

INLDEF SDL_Rect GetCurrentAnimationKeyFrame (AnimationHandle _animation_handle);

INLDEF void PauseAnimation (AnimationHandle _animation_handle, bool _pause);
INLDEF bool AnimationIsPaused (AnimationHandle _animation_handle);

INLDEF void ResetAnimation (AnimationHandle _animation_handle);

INLDEF int GetAnimationFrameWidth (AnimationHandle _animation_handle);
INLDEF int GetAnimationFrameHeight (AnimationHandle _animation_handle);

INLDEF float GetAnimationLength_Internal (const Animation& _animation);
INLDEF float GetAnimationLength (AnimationHandle _animation_handle);

INLDEF bool AnimationIsDone (AnimationHandle _animation_handle);

} // TCE

#endif /* __TCE_ANIMATION_H__ ////////////////////////////////////////////////*/
