/*******************************************************************************
 * SOUND
 * Systems for loading, and playing back, WAV sound effects.
 *
*******************************************************************************/

#ifndef __TCE_SOUND_H__ /*////////////////////////////////////////////////////*/
#define __TCE_SOUND_H__

namespace TCE
{

GLOBAL constexpr const char* SOUND_PATH = "data/sounds/";

struct Sound
{
	Mix_Chunk* data;
};

GLOBAL Array<Sound> sound_list;

INLDEF void FreeSound (SoundHandle _sound_handle);
INLDEF void FreeSoundList ();

STDDEF void LoadSoundFromFile_Internal (SoundHandle& _sound_handle, const char* _filename);
INLDEF SoundHandle LoadSoundFromFile (const char* _filename);

INLDEF void PlaySound (SoundHandle _sound_handle, int _loops = 0);
INLDEF void StopSound (SoundHandle _sound_handle);

} // TCE

#endif /* __TCE_SOUND_H__ ////////////////////////////////////////////////////*/
