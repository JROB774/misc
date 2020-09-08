/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void MixerInit ()
{
	// Audio is not essential to gameplay so if we can't init then continue anyway.
	Mix_Init(0);
	if (Mix_OpenAudio(mixer_frequency, AUDIO_S16SYS, mixer_channels, mixer_chunksize) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_MIX, "Failed to initialize game audio!");
		return;
	}
	// The function callback used whenever a track finishes playing a given sound.
	Mix_ChannelFinished(TrackFinishedCallback);
	// Set the default mixer values (from variable tweak file).
	UpdateMixerValues();

	mixer_ready = true;
}

INLDEF void MixerQuit ()
{
	mixer_ready = false;

	Mix_CloseAudio();
	Mix_Quit();
}

STDDEF void TrackFinishedCallback (int _track)
{
	// Remove the reference to the sound from the track listing.
	track_reference_list[_track] = INVALID_ASSET_HANDLE;
}

INLDEF void UpdateMixerValues ()
{
	// We allocate tracks first so that setting volume applies to any new tracks.
	Mix_AllocateChannels(mixer_tracks);

	// By default all tracks should be set to INVALID_ASSET_HANDLE to avoid issues.
	size_t old_size = track_reference_list.count;
	track_reference_list.Resize(mixer_tracks);
	for (size_t i=old_size; i<CAST(size_t, mixer_tracks); ++i) {
		track_reference_list.AddElement(INVALID_ASSET_HANDLE);
	}

	// Set the default volume for the mixer to play sound and music at on all tracks.
	if (sound_volume < 0) { sound_volume = 0; }
	Mix_Volume(-1, sound_volume);
	if (music_volume < 0) { music_volume = 0; }
	Mix_VolumeMusic(music_volume);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
