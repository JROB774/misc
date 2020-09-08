/*******************************************************************************
 * HOTLOAD
 * A system for reloading various game assets in real-time, without restarting.
 *
*******************************************************************************/

#ifndef __TCE_HOTLOAD_H__ /*//////////////////////////////////////////////////*/
#define __TCE_HOTLOAD_H__

namespace TCE
{

GLOBAL constexpr const char* HOTLOAD_DATA_DIRECTORY = "data/";

enum AssetType
{
	ASSET_VARIABLES,
	ASSET_TEXTURE,
	ASSET_ANIMATION,
	ASSET_FONT,
	ASSET_SOUND,
	ASSET_MAIN_SCRIPT,

	ASSET_TOTAL
};

GLOBAL constexpr const char* ASSET_TYPE_NAME[ASSET_TOTAL] =
{
	"VARIABLE FILE",
	"TEXTURE",
	"ANIMATION",
	"FONT",
	"SOUND",
	"MAIN SCRIPT",
};

struct AssetInfo
{
	String filename;

	AssetType type;
	AssetHandle handle;
};

GLOBAL Array<AssetInfo> asset_list;
GLOBAL AssetInfo asset_to_update;

GLOBAL SDL_Thread* hotloader_thread;
GLOBAL SDL_mutex* hotloader_lock;

// Whether or not hotloading is enabled is determined by a variable in the tweak file.
// This needs a default value in case the variable tweak value cannot be loaded!
GLOBAL bool hotloader_enabled = false;

INLDEF void HotloadInit ();
INLDEF void HotloadQuit ();

STDDEF int HotloaderMainCallback (void* _data);
STDDEF void HotloadChangedAsset ();

STDDEF void RegisterAssetForHotloading (const char* _filename, AssetType _type, AssetHandle _handle);
STDDEF bool AssetIsRegisteredForHotloading (const char* _filename);

} // TCE

#endif /* __TCE_HOTLOAD_H__ //////////////////////////////////////////////////*/
