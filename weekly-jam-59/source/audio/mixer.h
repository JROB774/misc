/*******************************************************************************
 * MIXER
 * Handles managing sound channels, volume, and other audio mixing systems.
 *
*******************************************************************************/

#ifndef __TCE_MIXER_H__ /*////////////////////////////////////////////////////*/
#define __TCE_MIXER_H__

namespace TCE
{

// The track_reference_list stores a reference to what sound is currently being
// played on a channel. The indicies of the array correspond to the channel and
// the stored handle is that of the sound being played on said channel.
//
// Storing data in this way is useful as it allows for operations to then be
// carried out on certain channels, such as halting the playing of sound, etc.
GLOBAL Array<SoundHandle> track_reference_list;

// These need default values in case the variable tweak value cannot be loaded!
GLOBAL int mixer_frequency = 22050;
GLOBAL int mixer_channels = 2;
GLOBAL int mixer_chunksize = 2048;
GLOBAL int mixer_tracks = 32;

GLOBAL int sound_volume = 128;
GLOBAL int music_volume = 128;

GLOBAL bool mixer_ready = false;

INLDEF void MixerInit ();
INLDEF void MixerQuit ();

STDDEF void TrackFinishedCallback (int _track);

INLDEF void UpdateMixerValues ();

} // TCE

#endif /* __TCE_MIXER_H__ ////////////////////////////////////////////////////*/
