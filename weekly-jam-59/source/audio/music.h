/*******************************************************************************
 * MUSIC
 * Systems for loading, handling, and playing back music tracks.
 *
*******************************************************************************/

#ifndef __TCE_MUSIC_H__ /*////////////////////////////////////////////////////*/
#define __TCE_MUSIC_H__

namespace TCE
{

GLOBAL constexpr const char* MUSIC_PATH = "data/music/";

GLOBAL Mix_Music* music;

INLDEF void LoadMusic (const char* _filename);
INLDEF void FreeMusic ();

INLDEF void PlayMusic (const char* _filename, int _loops);
INLDEF void StopMusic ();

INLDEF void FadeInMusic (const char* _filename, int _loops, float _fade_time);
INLDEF void FadeOutMusic (float _fade_time);

} // TCE

#endif /* __TCE_MUSIC_H__ ////////////////////////////////////////////////////*/
