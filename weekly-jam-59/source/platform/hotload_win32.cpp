/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

/* PLATFORM DEPENDENT - DO NOT USE OUTSIDE OF THIS FILE ***********************/

GLOBAL HANDLE stop_hotloader_event;

/******************************************************************************/

INLDEF void HotloadInit ()
{
	// We setup the hotloader on a separate thread so it does not interfere
	// and because it needs to freeze the thread whilst waiting for a file
	// update event (on Windows, this may be different on other platforms).

	// Do not bother, if hotloading is not meant to be enabled
	if (!hotloader_enabled) { return; }

	// Make sure this is set before starting the hotloading system.
	asset_to_update.filename = "\0";

	// This event is called on exit to short-circuit the thread.
	stop_hotloader_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!stop_hotloader_event) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_STD, "Failed to create hotloader stop event!");
		return;
	}

	// Setup and execute the thread to begin hotloading assets.
	hotloader_lock = SDL_CreateMutex();
	if (!hotloader_lock) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_SDL, "Failed to create hotloader mutex!");
		return;
	}
	hotloader_thread = SDL_CreateThread(HotloaderMainCallback, "Hotloader", NULL);
	if (!hotloader_thread) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_SDL, "Failed to create the hotloader thread!");
		return;
	}
}

INLDEF void HotloadQuit ()
{
	hotloader_enabled = false;

	// End the hotloader thread gracefully.
	SetEvent(stop_hotloader_event);

	SDL_WaitThread(hotloader_thread, NULL);
	SDL_DestroyMutex(hotloader_lock);
}

STDDEF int HotloaderMainCallback (void* _data)
{
	//
	// Do not call TCE_ERROR_LOG with a value higher than ERROR_LEVEL_MIN on this thread.
	// This is because ShowMessageBox (using in TCE_ERROR_LOG) could be implemented using
	// SDL, depending on what platform the engine is running on, and SDL's documentation
	// states that their SDL_ShowMessageBox function should be called on the same thread
	// that created the parent window. Fortunately, this is not an issue for this system.
	//
	// The page of the documentation: https://wiki.libsdl.org/SDL_ShowMessageBox
	//

	// Gain access to the directory whilst allowing all ops to continue.
	DWORD share_mode = FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE;
	DWORD flags = FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED;

	HANDLE data_directory = CreateFile(HOTLOAD_DATA_DIRECTORY, FILE_LIST_DIRECTORY, share_mode, NULL, OPEN_EXISTING, flags, NULL);
	if (data_directory == INVALID_HANDLE_VALUE) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Hotloader failed to access data directory!");
		return 0;
	}

	// Needed to allow for asynchronous I/O on files.
	OVERLAPPED polling_overlapped = {0};

	polling_overlapped.OffsetHigh = 0;
	polling_overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!polling_overlapped.hEvent) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Hotloader failed to setup asynchronous I/O!");
		CloseHandle(data_directory); // @IMPROVE: THIS IS REDUNDANT, IS IT NECESSARY?
		return 0;
	}

	// These are the events WaitForMultipleObjects will look for.
	HANDLE events[] = { polling_overlapped.hEvent, stop_hotloader_event };

	bool hotloader_running = true;
	while (hotloader_running) {
		// Monitor the data directory and all its sub-directories.
		char info_buffer[2048]; // @IMPROVE: MAKE THIS DYNAMIC!!!
		DWORD bytes_returned;

		BOOL result = ReadDirectoryChangesW(data_directory, &info_buffer, sizeof(info_buffer),
			TRUE, FILE_NOTIFY_CHANGE_SIZE, &bytes_returned, &polling_overlapped, NULL);
		if (!result) { continue; } // If we failed to monitor changes, try again.

		// Wait here until either a file changes or the hotloader thread is requested to stop.
		DWORD event_fired = WaitForMultipleObjects(ARRAY_COUNT(events), events, FALSE, INFINITE);
		switch (event_fired) {
			// FILE CHANGE EVENT
			case (WAIT_OBJECT_0): {
				SDL_LockMutex(hotloader_lock);

				FILE_NOTIFY_INFORMATION* notify_info = NULL;
				int offset = 0;
				do {
					notify_info = CAST(FILE_NOTIFY_INFORMATION*, info_buffer+offset);
					// We only care if files have been modified, nothing else.
					if (notify_info->Action == FILE_ACTION_MODIFIED) {
						// Convert the wide-character filename to ASCII.
						char filename[256] = {0}; // @IMPROVE: MAKE THIS DYNAMIC!!!
						int length = notify_info->FileNameLength / 2;
						WideCharToMultiByte(CP_ACP, 0, notify_info->FileName, length, filename, sizeof(filename), NULL, NULL);
						filename[length] = '\0';
						// We are using forward slashes for compatibility on Linux so we need
						// to change the filename provided by Windows to use forward slashes.
						for (char* c=filename; *c; ++c) {
							if (*c == '\\') { *c = '/'; }
						}

						// Store the info of the asset that needs updating.
						for (size_t i=0; i<asset_list.count; ++i) {
							AssetInfo& asset_info = asset_list[i];

							// We strip the data directory part of the asset name for matching.
							size_t pos = asset_info.filename.FindFirst("/\\");
							if (pos == String::UNDEFINED_POSITION) { pos = 0; }
							else { pos += 1; }

							const char* asset_name_trimmed = asset_info.filename.c_string + pos;
							if (strcmp(filename, asset_name_trimmed) == 0) {
								asset_to_update.filename = asset_info.filename;
								asset_to_update.type     = asset_info.type;
								asset_to_update.handle   = asset_info.handle;

								break; // No need to continue searching.
							}
						}
					}

					// Move on to the next entry, if there is one.
					offset += notify_info->NextEntryOffset;
				}
				while (notify_info->NextEntryOffset);

				SDL_UnlockMutex(hotloader_lock);
			} break;
			// QUIT EVENT
			case (WAIT_OBJECT_0+1): {
				hotloader_running = false;
			} break;
		}
	}

	CloseHandle(data_directory);

	return 0;
}

STDDEF void HotloadChangedAsset ()
{
	SDL_LockMutex(hotloader_lock);

	// If there is no file that needs reloading then we have nothing to do.
	if (asset_to_update.filename[0] == '\0') {
		SDL_UnlockMutex(hotloader_lock);
		return;
	}

	// Determine what type of file it is so we know how to reload it.
	String filename = asset_to_update.filename;
	switch (asset_to_update.type) {
		case (ASSET_VARIABLES): {
			LoadAllVariables();
			TCE_DEBUG_LOG("HOTLOAD", "Reloaded %s '%s'!", ASSET_TYPE_NAME[asset_to_update.type], filename.c_string);
		} break;
		case (ASSET_TEXTURE): {
			LoadTextureFromFile_Internal(asset_to_update.handle, filename.c_string);
			TCE_DEBUG_LOG("HOTLOAD", "Reloaded %s '%s'!", ASSET_TYPE_NAME[asset_to_update.type], filename.c_string);
		} break;
		case (ASSET_ANIMATION): {
			LoadAnimationFromFile_Internal(asset_to_update.handle, filename.c_string);
			TCE_DEBUG_LOG("HOTLOAD", "Reloaded %s '%s'!", ASSET_TYPE_NAME[asset_to_update.type], filename.c_string);
		} break;
		case (ASSET_FONT): {
			LoadFontFromFile_Internal(asset_to_update.handle, filename.c_string);
			TCE_DEBUG_LOG("HOTLOAD", "Reloaded %s '%s'!", ASSET_TYPE_NAME[asset_to_update.type], filename.c_string);
		} break;
		case (ASSET_SOUND): {
			LoadSoundFromFile_Internal(asset_to_update.handle, filename.c_string);
			TCE_DEBUG_LOG("HOTLOAD", "Reloaded %s '%s'!", ASSET_TYPE_NAME[asset_to_update.type], filename.c_string);
		} break;
		case (ASSET_MAIN_SCRIPT): {
			LoadLuaMainScript();
			TCE_DEBUG_LOG("HOTLOAD", "Reloaded %s '%s'!", ASSET_TYPE_NAME[asset_to_update.type], filename.c_string);
		} break;

		// Inform of unknown file/asset types.
		default: {
			TCE_DEBUG_LOG("HOTLOAD", "Attempt to reload file '%s' of unknown type '%d'!", filename.c_string, asset_to_update.type);
		} break;
	}

	// Reset this so that we don't try updating on the next frame.
	asset_to_update.filename.Clear();

	SDL_UnlockMutex(hotloader_lock);
}

STDDEF void RegisterAssetForHotloading (const char* _filename, AssetType _type, AssetHandle _handle)
{
	// Don't bother if hotloading isn't enabled.
	if (!hotloader_enabled) { return; }
	// Don't bother registering an asset a second time.
	if (AssetIsRegisteredForHotloading(_filename)) { return; }

	// Add the asset info to the list of registered assets.
	AssetInfo asset_info;

	asset_info.filename = _filename;
	asset_info.type     = _type;
	asset_info.handle   = _handle;

	asset_list.AddElement(asset_info);

	// Informs that an asset was successfully registered
	TCE_DEBUG_LOG("HOTLOAD", "Registered asset '%s' of type '%s'!", _filename, ASSET_TYPE_NAME[_type]);
}

STDDEF bool AssetIsRegisteredForHotloading (const char* _filename)
{
	// Don't bother if hotloading isn't enabled.
	if (hotloader_enabled) {
		for (size_t i=0; i<asset_list.count; ++i) {
			if (_filename == asset_list[i].filename) {
				return true;
			}
		}
	}
	return false;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
