/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void ParseAnimationTextureOffset (Animation& _animation, Tokenizer& _tokenizer)
{
	_animation.texture_offset_x = TokenToInteger(_tokenizer);
	_animation.texture_offset_y = TokenToInteger(_tokenizer);
}

INLDEF void ParseAnimationFrameSize (Animation& _animation, Tokenizer& _tokenizer)
{
	_animation.frame_width = TokenToInteger(_tokenizer);
	_animation.frame_height = TokenToInteger(_tokenizer);
}

INLDEF void ParseAnimationLooped (Animation& _animation, Tokenizer& _tokenizer)
{
	_animation.looped = TokenToBoolean(_tokenizer);
}

INLDEF void ParseAnimationFrameTime (Animation& _animation, Tokenizer& _tokenizer)
{
	_animation.key_frame.AddElement(TokenToNumber(_tokenizer));
}

STDDEF void LoadAnimationFromFile_Internal (AnimationHandle& _animation_handle, const char* _filename)
{
	// If we don't want to reload an animation specified by a specific handle then
	// assign the invalid handle a value (at the end of the animation list).
	if (_animation_handle == INVALID_ASSET_HANDLE) {
		_animation_handle = animation_list.count;
		animation_list.AddElement(Animation {0});
	}

	// Reset the old animation data in case the file does not reset all the values.
	Animation& animation = animation_list[_animation_handle];
	memset(&animation, 0, sizeof(Animation));

	// Split the entire animation file into lines for easy parsing.
	Array<String> lines = ReadEntireFileIntoLines(_filename);
	if (!lines.data) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Failed to load animation '%s'!", _filename);
		return;
	}

	// Parse the animation file data into something usable by the game code.
	Tokenizer tokenizer = {0};
	tokenizer.delim = " \n\r\t\v\f";
	for (size_t i=0; i<lines.count; ++i) {
		tokenizer.cursor = EatLeadingDelimiters(lines[i].c_string, tokenizer.delim);
		if (tokenizer.cursor[0] == '\0' || tokenizer.cursor[0] == '#') { // Ignore blanks and comments.
			continue;
		} else {
			GetToken(tokenizer);
			// Read for known animation file keywords to get the data.
			if (strncmp(tokenizer.token, "texture_offset", tokenizer.token_length) == 0) {
				ParseAnimationTextureOffset(animation, tokenizer);
			} else if (strncmp(tokenizer.token, "frame_size", tokenizer.token_length) == 0) {
				ParseAnimationFrameSize(animation, tokenizer);
			} else if (strncmp(tokenizer.token, "looped", tokenizer.token_length) == 0) {
				ParseAnimationLooped(animation, tokenizer);
			} else if (strncmp(tokenizer.token, "time", tokenizer.token_length) == 0) {
				ParseAnimationFrameTime(animation, tokenizer);
			} else {
				TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Unknown keyword '%.*s' on line %d in animation '%s'!",
					tokenizer.token_length, tokenizer.token, i, _filename);
			}
		}
	}

	// Register the asset with the hotloader, if not already registered.
	RegisterAssetForHotloading(_filename, ASSET_ANIMATION, _animation_handle);
}

INLDEF AnimationHandle LoadAnimationFromFile (const char* _filename)
{
	AnimationHandle animation_handle = INVALID_ASSET_HANDLE;
	LoadAnimationFromFile_Internal(animation_handle, _filename);

	return animation_handle;
}

INLDEF void UpdateAnimationTimers (float _dt)
{
	// If all animations are currently paused then we have no reason to update.
	if (all_animations_paused) { return; }

	for (size_t i=0; i<animation_list.count; ++i) {
		// We do not need to advance the time of pauses animations.
		Animation& animation = animation_list[i];
		if (animation.paused) { continue; }

		animation.timer += _dt;
		// If the animation is looped we want to loop back around.
		if (animation.looped) {
			float animation_length = GetAnimationLength_Internal(animation);
			if (animation_length > 0.0f) {
				while (animation.timer >= animation_length) {
					animation.timer -= animation_length;
				}
			}
		}
	}
}

INLDEF SDL_Rect GetCurrentAnimationKeyFrame (AnimationHandle _animation_handle)
{
	// Do not bother handling invalid animation handles.
	if (_animation_handle >= animation_list.count) { return SDL_Rect {0}; }
	Animation& animation = animation_list[_animation_handle];

	// Do not bother if the animation is empty.
	if (animation.key_frame.count == 0) { return SDL_Rect {0}; }

	// Determine what frame we are on based on the current value of the timer.
	float accumulated_time = 0.0f;
	size_t current_frame = 0;
	for (size_t i=0; i<animation.key_frame.count; ++i) {
		accumulated_time += animation.key_frame[i];
		if (animation.timer >= accumulated_time) { ++current_frame; }
		else { break; }
	}
	// Prevent going out of bounds on non-looped animations.
	if (current_frame >= animation.key_frame.count) {
		current_frame = animation.key_frame.count-1;
	}

	// Create the sprite clip/bounds for the key frame.
	int frame_x_offset = current_frame * animation.frame_width;
	SDL_Rect key_frame;

	key_frame.x = animation.texture_offset_x + frame_x_offset;
	key_frame.y = animation.texture_offset_y;
	key_frame.w = animation.frame_width;
	key_frame.h = animation.frame_height;

	return key_frame;
}

INLDEF void PauseAnimation (AnimationHandle _animation_handle, bool _pause)
{
	// Do not bother handling invalid animation handles.
	if (_animation_handle >= animation_list.count) { return; }
	Animation& animation = animation_list[_animation_handle];

	animation.paused = _pause;
}
INLDEF bool AnimationIsPaused (AnimationHandle _animation_handle)
{
	// Do not bother handling invalid animation handles.
	if (_animation_handle >= animation_list.count) { return false; }
	Animation& animation = animation_list[_animation_handle];

	return animation.paused;
}

INLDEF void ResetAnimation (AnimationHandle _animation_handle)
{
	// Do not bother handling invalid animation handles.
	if (_animation_handle >= animation_list.count) { return; }
	Animation& animation = animation_list[_animation_handle];

	// Set the animation's timer back to the start
	animation.timer = 0.0f;
}

INLDEF int GetAnimationFrameWidth (AnimationHandle _animation_handle)
{
	// Do not bother handling invalid animation handles.
	if (_animation_handle > animation_list.count) { return 0; }
	Animation& animation = animation_list[_animation_handle];

	return animation.frame_width;
}
INLDEF int GetAnimationFrameHeight (AnimationHandle _animation_handle)
{
	// Do not bother handling invalid animation handles.
	if (_animation_handle >= animation_list.count) { return 0; }
	Animation& animation = animation_list[_animation_handle];

	return animation.frame_height;
}

INLDEF float GetAnimationLength_Internal (const Animation& _animation)
{
	float length = 0.0f;
	for (size_t i=0; i<_animation.key_frame.count; ++i) {
		length += _animation.key_frame[i];
	}
	return length;
}

INLDEF float GetAnimationLength (AnimationHandle _animation_handle)
{
	// Do not bother handling invalid animation handles.
	if (_animation_handle >= animation_list.count) { return 0; }
	Animation& animation = animation_list[_animation_handle];

	return GetAnimationLength_Internal(animation);
}

INLDEF bool AnimationIsDone (AnimationHandle _animation_handle)
{
	// Do not bother handling invalid animation handles.
	if (_animation_handle >= animation_list.count) { return 0; }
	Animation& animation = animation_list[_animation_handle];

	if (animation.looped) { return false; } // Looped can never be done.
	else { return animation.timer >= GetAnimationLength_Internal(animation); }
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
