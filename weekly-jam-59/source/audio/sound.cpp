/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void FreeSound (SoundHandle _sound_handle)
{
	// Do not bother handling invalid sound handles.
	if (_sound_handle >= sound_list.count) { return; }

	Sound& sound = sound_list[_sound_handle];
	if (sound.data) {
		Mix_FreeChunk(sound.data);
		sound.data = NULL;
	}
}
INLDEF void FreeSoundList ()
{
	// Go through and free all of the allocated sounds.
	for (size_t i=0; i<sound_list.count; ++i) { FreeSound(i); }
}

STDDEF void LoadSoundFromFile_Internal (SoundHandle& _sound_handle, const char* _filename)
{
	// If we don't want to reload a sound specified by a specific handle then
	// assign the invalid handle a value (at the end of the sound list).
	if (_sound_handle == INVALID_ASSET_HANDLE) {
		_sound_handle = sound_list.count;
		sound_list.AddElement(Sound {0});
	}

	// Free the old sound data to avoid memory leaks.
	Sound& sound = sound_list[_sound_handle];
	FreeSound(_sound_handle);

	sound.data = Mix_LoadWAV(_filename);
	if (!sound.data) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_MIX, "Failed to load sound file '%s'!", _filename);
	} else {
		// Register the asset with the hotloader, if not already registered.
		RegisterAssetForHotloading(_filename, ASSET_SOUND, _sound_handle);
	}
}

INLDEF SoundHandle LoadSoundFromFile (const char* _filename)
{
	SoundHandle sound_handle = INVALID_ASSET_HANDLE;
	LoadSoundFromFile_Internal(sound_handle, _filename);

	return sound_handle;
}

INLDEF void PlaySound (SoundHandle _sound_handle, int _loops)
{
	// Do not bother handling invalid sound handles.
	if (_sound_handle >= sound_list.count) { return; }
	const Sound& sound = sound_list[_sound_handle];

	// Attempt to play the sound then cache what channel it is being played on.
	int channel = Mix_PlayChannel(-1, sound.data, _loops);

	if (channel < 0) { TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_MIX, "Failed to play a sound!"); }
	else {track_reference_list[channel] = _sound_handle; }
}
INLDEF void StopSound (SoundHandle _sound_handle)
{
	// Do not bother handling invalid sound handles.
	if (_sound_handle >= sound_list.count) { return; }
	// Halt the sound on all the channels it is being played on.
	for (size_t i=0; i<track_reference_list.count; ++i) {
		if (track_reference_list[i] == _sound_handle) {
			Mix_HaltChannel(i);
		}
	}
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
