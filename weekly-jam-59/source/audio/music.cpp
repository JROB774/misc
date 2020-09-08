/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void LoadMusic (const char* _filename)
{
	// Free the old music track, if there was one, to avoid leaks.
	if (music) { FreeMusic(); }

	// Combine the music path with the filename.
	String filename = MUSIC_PATH;
	filename.AddCString(_filename);

	// Load the music as the new currently loaded music track.
	music = Mix_LoadMUS(filename.c_string);
	if (!music) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_MIX, "Failed to load the music track '%s'!", filename.c_string);
	}
}
INLDEF void FreeMusic ()
{
	Mix_FreeMusic(music);
	music = NULL;
}

INLDEF void PlayMusic (const char* _filename, int _loops)
{
	LoadMusic(_filename);
	if (music) {
		if (Mix_PlayMusic(music, _loops) < 0) {
			TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_MIX, "Failed to play the current music track!");
		}
	}
}
INLDEF void StopMusic ()
{
	Mix_HaltMusic();
}

INLDEF void FadeInMusic (const char* _filename, int _loops, float _fade_time)
{
	LoadMusic(_filename);
	if (music) {
		// Convert the fade time from seconds into milliseconds.
		int milliseconds = CAST(int, (_fade_time * 1000.0f));
		if (Mix_FadeInMusic(music, _loops, milliseconds) < 0) {
			TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_MIX, "Failed to fade in the current music track!");
		}
	}
}
INLDEF void FadeOutMusic (float _fade_time)
{
	// Convert the fade time from seconds into milliseconds.
	int milliseconds = CAST(int, (_fade_time * 1000.0f));
	if (Mix_FadeOutMusic(milliseconds) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_MIX, "Failed to fade out the current music track!");
	}
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
