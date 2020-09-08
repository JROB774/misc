namespace LevelEditor
{
	namespace
	{
		struct TileInfo
		{
			std::uint8_t layer;
			std::uint32_t pos;
			std::uint16_t oldid;
			std::uint16_t newid;
		};

		enum Action
		{
			ACTION_STANDARD,
			ACTION_FLIP_HORZ,
			ACTION_FLIP_VERT,
			ACTION_RESIZE
		};

		struct History
		{
			Action action;
			std::vector<TileInfo> state;

			// used for flips (stores what layers were active / got flipped)
			std::array<bool, LVL::LAYER_TOTAL> layerstate;
			// used for resize (stores the old and new size of the level and dir)
			SDL_Point oldlevelsize, newlevelsize;
			Control::Resize::Dir resizedir;
		};

		enum State
		{
			STATE_NONE,  // standard state
			STATE_DRAW,  // state whilst drawing
			STATE_ERASE, // state whilst erasing
			STATE_PROMPT // state whilst in new-level control/prompt
		};

		// these must correspond to buttons for ease-of-use
		enum Tool
		{
			TOOL_PENCIL = Hotbar::BUTTON_PENCIL,
			TOOL_FILL = Hotbar::BUTTON_FILL,
			TOOL_SELECT = Hotbar::BUTTON_SELECT
		};

		enum Panning
		{
			PANNING_UP,
			PANNING_RIGHT,
			PANNING_DOWN,
			PANNING_LEFT,
			PANNING_TOTAL
		};

		constexpr const char* GAME_EXECUTABLE_NAME = "TheEndIsNigh.exe";
		constexpr const char* GAME_SETTINGS_NAME = "settings.txt";

		constexpr const char* SETTINGS_MODDING_FASTSTART = "modding_faststart";

		constexpr const char* BACKUPS_DIR = "backups\\";

		constexpr const char* HELP_PAGE_URL = "http://tein-modders-manual.000webhostapp.com/Manual.html";

		constexpr std::uint32_t CAMERA_TILE_ID = LVL::OFFSET[LVL::CATEGORY_TAG];

		constexpr const char* UNSAVED_CAPTION = "Unsaved Changes";
		constexpr const char* UNSAVED_TEXT = "There are unsaved changes!\nWould you like to save?";

		// the padding surrounding the level editor area and other gui elements
		constexpr int PADDING = 32;

		// the default size for a level when the app is opened
		constexpr std::uint32_t DEFAULT_LEVELWIDTH = 54;
		constexpr std::uint32_t DEFAULT_LEVELHEIGHT = 32;

		// the padding area where dragging a selection will pan the camera
		constexpr int SELECT_DRAG_FRINGE = 16;

		constexpr float CAMERA_PAN_SPEED = 0.85f; // this is in terms of tiles

		constexpr float ZOOM_MIN = 0.25f;
		constexpr float ZOOM_MAX = 4.00f;

		std::vector<History> history;
		int historypos = -1;

		// uses underscores because impossible to read otherwise...
		SDL_Color camerabounds_valid_color;
		SDL_Color camerabounds_invalid_color;
		SDL_Color selectbox_color;
		SDL_Color cursor_tile_color;
		SDL_Color leveleditor_background_color;
		SDL_Color leveleditor_grid_color;
		SDL_Color symmetry_lines_color;

		std::uint8_t layer_active_alpha[LVL::LAYER_TOTAL];
		std::uint8_t layer_inactive_alpha[LVL::LAYER_TOTAL];

		std::string currentfile;

		int tilesize = 0;

		// these offsets track the current position of the "camera" which are altered
		// by panning around. used to determine what tile is being interacted with and
		// what tiles to currently render

		float xoffset_def = 0, yoffset_def = 0;
		float xoffset = 0, yoffset = 0;

		bool panning = false; // for middle-mouse button panning
		bool panningdir[PANNING_TOTAL] = { false }; // for arrow key panning

		// used in middle-mouse button panning
		int relativecurx = 0;
		int relativecury = 0;

		float currentzoom = 1.0f;

		GPU_Rect area;

		std::array<std::vector<std::uint32_t>, LVL::LAYER_TOTAL> selectbuffer;
		int selectwidth = 0, selectheight = 0;
		GPU_Rect selectbox = {};
		bool selectstart = false;
		bool select = false;
		bool selectcursor = false;

		std::array<std::vector<std::pair<std::uint32_t, std::uint32_t>>, LVL::LAYER_TOTAL> flipmaphorz;
		std::array<std::vector<std::pair<std::uint32_t, std::uint32_t>>, LVL::LAYER_TOTAL> flipmapvert;

		bool selectvisible = false;
		bool gridvisible = true;
		bool cameravisible = false;

		bool mirrorhorz = false;
		bool mirrorvert = false;

		Cursor::Type cursor = Cursor::CURSOR_ARROW;
		bool cursorunset = false;

		bool unsavedchanges = false;

		bool replacefill = false;

		std::string gamepath;
		bool allowsettingsmods = false;
		bool nosettingsbackup = false;

		// the level data itself
		std::uint32_t levelwidth = DEFAULT_LEVELWIDTH;
		std::uint32_t levelheight = DEFAULT_LEVELHEIGHT;
		LVL::MapArray tiledata;

		// handles whether layrs are locked/invisible or not
		std::array<bool, LVL::LAYER_TOTAL> islayeractive = { true, true, true, true, true };

		State state = STATE_NONE;
		Tool tool = TOOL_PENCIL;

		// used in debug timing calculations
		#ifndef RELEASE
		std::int64_t perfcountfreq;
		#endif

		inline int GetXOffsetPos ()
		{
			return (xoffset * (float)tilesize) + TilePanel::WIDTH;
		}

		inline int GetYOffsetPos ()
		{
			return (yoffset * (float)tilesize) + Hotbar::HEIGHT;
		}

		inline void GetTilePos (int& _x, int& _y)
		{
			// get the current mouse position
			int curx, cury;
			SDL_GetMouseState(&curx, &cury);
			// convert into the x and y of current tile being hovered on
			_x = std::floor((curx - GetXOffsetPos()) / tilesize);
			_y = std::floor((cury - GetYOffsetPos()) / tilesize);

			// bounds checking, as offset could put you out of bounds
			if (_x < 0) { _x = 0; }
			else if (_x >= (int)levelwidth) { _x = levelwidth-1; }
			if (_y < 0) { _y = 0; }
			else if (_y >= (int)levelheight) { _y = levelheight-1; }
		}

		std::uint32_t GetTileHorzFlipID (std::uint8_t _layer, std::uint32_t _id)
		{
			// checks to see if the tile has an appropriate horizontal flip counterpart
			for (auto& tilepair: flipmaphorz.at(_layer)) {
				if (_id == tilepair.first) { return tilepair.second; }
				else if (_id == tilepair.second) { return tilepair.first; }
			}
			// otherwise it just returns the input
			return _id;
		}

		std::uint32_t GetTileVertFlipID (std::uint8_t _layer, std::uint32_t _id)
		{
			// checks to see if the tile has an appropriate vertical flip counterpart
			for (auto& tilepair: flipmapvert.at(_layer)) {
				if (_id == tilepair.first) { return tilepair.second; }
				else if (_id == tilepair.second) { return tilepair.first; }
			}
			// otherwise it just returns the input
			return _id;
		}

		void CalculateArea ()
		{
			// get available area for editor to be placed in
			int validareaw = (App::GetWidth() - TilePanel::WIDTH) - PADDING;
			int validareah = (App::GetHeight() - Hotbar::HEIGHT) - PADDING;

			// the reason for using the default level size in this calculation is because we want
			// the scaling handled by window resizing to work the same no matter how big the level
			// is, the zoom scale is then a modifier applied on top of that
			// it also ensures that we never end up with too many tiles on screen that slow down
			// the rendering process

			// determine how big tiles need to be to fit in this area
			tilesize = std::min<int>(
				(int)std::floor(validareaw / DEFAULT_LEVELWIDTH),
				(int)std::floor(validareah / DEFAULT_LEVELHEIGHT)
			);
			// ensure the zoom factor is considered in the size
			tilesize = std::floor(tilesize * currentzoom);

			// using tilesize determine the offset and size of the level editor window
			area.x = 0.0f; // area.x = (float)((float)TilePanel::WIDTH + (((float)validareaw - ((tilesize * (float)DEFAULT_LEVELWIDTH) - (float)PADDING)) / 2.0f));
			area.y = 0.0f; // area.y = (float)((float)Hotbar::HEIGHT + (((float)validareah - ((tilesize * (float)DEFAULT_LEVELHEIGHT) - (float)PADDING)) / 2.0f));
			area.w = (float)levelwidth * tilesize;
			area.h = (float)levelheight * tilesize;
		}

		void NewHistoryState (Action _action)
		{
			// don't bother creating a new state if the current standard action is empty
			if (historypos > -1) {
				auto& current = history.at(historypos);
				if (current.state.empty() && current.action == _action && _action == ACTION_STANDARD) {
					return;
				}
			}

			// clear all the history after the current history position, it will no longer apply now
			if (historypos+1 < (int)history.size()) { history.erase(history.begin()+(historypos+1), history.end()); }
			// add a new history state based on the current tool being used
			history.push_back(History());
			history.back().action = _action;
			// if we're flipping then we need to know what layers got flipped
			if (_action == ACTION_FLIP_HORZ || _action == ACTION_FLIP_VERT) { history.back().layerstate = islayeractive; }
			// if we're resizing then we need to know the current level size
			if (_action == ACTION_RESIZE) { history.back().oldlevelsize = { (int)levelwidth, (int)levelheight }; }
			historypos++;
		}

		bool HistoryAddToState (std::uint8_t _layer, std::uint32_t _pos, std::uint16_t _oldid, std::uint16_t _newid)
		{
			// there's no current state so don't do anything (this shouldn't really happen)
			if (historypos <= -1) { return false; }

			// don't add the same tiles again
			auto& historystate = history.at(historypos).state;
			if (!historystate.empty()) {
				for (auto& tile: historystate) {
					if (tile.pos == _pos && tile.layer == _layer && tile.newid == _newid) { return false; }
					else if (tile.pos == _pos && tile.layer == _layer && tile.newid != _newid) {
						tile.newid = _newid;
						return true;
					}
				}
			}
			// otherwise add it!
			historystate.push_back(TileInfo { _layer, _pos, _oldid, _newid });
			return true;
		}

		// _mirrorhorz and _mirrorvert used to stop being stuck in a loop of mirroring
		bool CreateTile (int _x, int _y, std::uint8_t _layer, std::uint32_t _id, bool _mirrorhorz = true, bool _mirrorvert = true)
		{
			// store the old and new state of the tile to history
			int tilepos = _y * levelwidth + _x;
			if (!HistoryAddToState(_layer, tilepos, tiledata.at(_layer).at(tilepos), _id)) { return false; }

			// replace the tile with the new tile (wether it be empty or not)
			tiledata.at(_layer).at(tilepos) = _id;

			// @IMPROVE: THIS COULD PROBS BE HANDLED IN A MUCH BETTER WAY, IM JUST BEING DUMB POTENTIALLY
			// handles mirroring horizontally and vertically
			if (mirrorhorz && _mirrorhorz && mirrorvert && _mirrorvert) {
				CreateTile((levelwidth-1) - _x, _y, _layer, GetTileHorzFlipID(_layer, _id), false, true);
				CreateTile(_x, (levelheight-1) - _y, _layer, GetTileVertFlipID(_layer, _id), false, false);
			}
			else if (mirrorhorz && _mirrorhorz) { CreateTile((levelwidth-1) - _x, _y, _layer, GetTileHorzFlipID(_layer, _id), false, false); }
			else if (mirrorvert && _mirrorvert) { CreateTile(_x, (levelheight-1) - _y, _layer, GetTileVertFlipID(_layer, _id), false, false); }

			return true;
		}

		void GetSelectRect (int& _sx, int& _sy, int& _sw, int& _sh)
		{
			if (selectbox.x >= selectbox.w) { _sx = selectbox.w-1, _sw = selectbox.x+1; }
			else { _sx = selectbox.x, _sw = selectbox.w; }
			if (selectbox.y >= selectbox.h) { _sy = selectbox.h-1, _sh = selectbox.y+1; }
			else { _sy = selectbox.y, _sh = selectbox.h; }
		}

		void SaveFile ()
		{
			// save the level to a file
			LVL::Save(currentfile, tiledata, levelwidth, levelheight);

			// backup saved data for the level to the backups folder

			// no point attempting the following code if no backups are allowed
			std::size_t backupcount = Settings::GetInteger("backup_lvl_quantity");
			if (backupcount <= 0) { return; }

			// cache this because certain platform layer functions can't have the \\ on the end
			std::string backupsdir = BACKUPS_DIR;
			backupsdir.pop_back(); // removes the \\

			// create the backups directory if for some reason it is no longer there
			if (!Platform::FileOrDirectoryExists(backupsdir)) {
				if (!Platform::CreateNewDirectory(backupsdir)) {
					ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to save a level backup!");
					return;
				}
			}

			// get the name of the level file to backup
			std::string lvlname = Utility::StrSection(currentfile, "\\/", ".");

			// get all the files currently in the backups directory
			std::vector<std::string> lvlfiles = Platform::ListDirectoryContents(backupsdir);
			// go through and see how many are for the current level
			std::size_t count = 0;
			for (auto& lvlfile: lvlfiles) {
				if (!Platform::IsDirectory(lvlfile)) { // we don't care about dirs
					if (lvlname == Utility::StrSection(lvlfile, "\\/", ".")) { ++count; }
				}
			}

			// create the basis for the backup file name
			std::string outputname  = BACKUPS_DIR;
			            outputname += lvlname;
			            outputname += ".bak";

			// if there's still slots left then make a new backup, otherwise overwrite an earlier backup
			if (count < backupcount) {
				outputname += std::to_string(count);
				LVL::Save(outputname, tiledata, levelwidth, levelheight);
			}
			else {
				// stores the index of which backup has been around longest
				std::uint64_t oldest = UINT64_MAX, current = 0;
				std::size_t oldestfile = 0;

				// go through each file and find out which has around the longest, overwrite that
				for (std::size_t i = 0; i < count; ++i) {
					// get the time the file was last written to
					std::string existingname = outputname + std::to_string(i);
					current = Platform::GetFileLastWriteTime(outputname + std::to_string(i));
					// compares the oldest file time with the time just checked, to see which is oldest
					if (current < oldest) { oldest = current, oldestfile = i; }
				}

				// overwrite the oldest backup file with new data
				outputname += std::to_string(oldestfile);
				LVL::Save(outputname, tiledata, levelwidth, levelheight);
			}
		}

		void Save ()
		{
			// don't bother with a save-as style dialog if the file is already known
			if (currentfile.empty()) {
				// ensure that nothing breaks if a file isn't selected
				std::string temp = Platform::SaveDialog(Platform::DIALOG_TYPE_LVL);
				if (temp.empty()) { return; }
				else { currentfile = temp; }
			}
			// perform actual save file code
			SaveFile();

			// sets a new caption for the window (using the new filename)
			App::AppendCaption(" (" + currentfile + ")");
			unsavedchanges = false;
		}

		void UnsavedChanges ()
		{
			// inefficient to do this if already set.
			if (!unsavedchanges) {
				// unsaved changes are now present
				unsavedchanges = true;
				std::string newcaption = " (" + ((currentfile.empty()) ? "Untitled"  : currentfile) + ")*";
				App::AppendCaption(newcaption);
			}
		}

		Platform::MessageBoxResult SaveChanges ()
		{
			// prompts the user to save before clearing
			Platform::MessageBoxResult result = Platform::MSGBOX_RESULT_NULL;
			if (unsavedchanges) {
				result = Platform::MessageBox(Platform::MSGBOX_TYPE_YESNOCANCEL, UNSAVED_CAPTION, UNSAVED_TEXT);
				if (result == Platform::MSGBOX_RESULT_YES) { Save(); }
			}

			// return this value for use by callee (to cancel operation)
			return result;
		}

		void SaveAs ()
		{
			// always perform a save-as style dialog, even if the file is known
			// ensure that nothing breaks if a file isn't selected
			std::string temp = Platform::SaveDialog(Platform::DIALOG_TYPE_LVL);
			if (temp.empty()) { return; }
			else { currentfile = temp; }
			// perform actual save file code
			SaveFile();

			// sets a new caption for the window (using the new filename)
			App::AppendCaption(" (" + currentfile + ")");
			unsavedchanges = false;
		}

		// @IMPROVE: THIS IS SO STUPID AT THIS POINT LOL
		// prototype for this so Open() and New_Underlying can find it
		void RealignCamera();

		// inits the prompt for creating a new level - New_Underlying does actual creation
		void New ()
		{
			// sets the state for the new level control/prompt
			state = STATE_PROMPT;

			// initialise a control prompt for a new level
			Control::Create(Control::TYPE_NEW);
			// set the cursor to arrow whilst handling a control/prompt
			Cursor::SetCursor(Cursor::CURSOR_ARROW);

			// unset any current tooltips
			Tooltip::Unset();
		}

		// the actual implementation of creating a new level
		void New_Underlying ()
		{
			// prompts the user to save before clearing (if cancel then do not perform operation)
			if (SaveChanges() == Platform::MSGBOX_RESULT_CANCEL) { return; }

			// resets everything about the currently loaded level
			currentfile.clear();
			App::AppendCaption(" (Untitled)");
			unsavedchanges = false;
			history.clear();
			historypos = -1;

			// resizes the tile data to that of the new level
			levelwidth = Control::GetWidth(), levelheight = Control::GetHeight();
			for (auto& layer: tiledata) { layer.resize(levelwidth * levelheight); }
			// resets the tile data so the new level is blank
			for (std::size_t i = 0; i < LVL::LAYER_TOTAL; ++i) {
				std::fill(tiledata.at(i).begin(), tiledata.at(i).end(), 0);
			}

			// resets the editor and the viewport content
			CalculateArea();
			RealignCamera();
		}

		void Open ()
		{
			// prompts the user to save before opening
			if (SaveChanges() == Platform::MSGBOX_RESULT_CANCEL) { return; }

			// ensure that nothing breaks if a file isn't selected
			std::string temp = Platform::OpenDialog(Platform::DIALOG_TYPE_LVL);
			if (temp.empty()) { return; }
			else { currentfile = temp; }
			// load the chosen file
			LVL::Load(currentfile, tiledata, levelwidth, levelheight);

			// update the level editor area + grid
			CalculateArea();

			// reset the camera pan/offset and zoom for the new level
			RealignCamera();

			// reset the history for the loaded file
			history.clear();
			historypos = -1;

			// sets a new caption for the window (using the new filename)
			App::AppendCaption(" (" + currentfile + ")");
			unsavedchanges = false;
		}

		void Unpack ()
		{
			// get the gpak filename path for unpacking
			std::string gpakfile = Platform::OpenDialog(Platform::DIALOG_TYPE_GPAK);
			if (gpakfile.empty()) { return; }
			// unpack the gpak contents
			GPAK::Unpack(gpakfile);
		}

		void Pack ()
		{
			// get the gpak filename path for unpacking
			std::string gpakdir = Platform::OpenFolderDialog();
			if (gpakdir.empty()) { return; }
			// unpack the gpak contents
			GPAK::Pack(gpakdir);
		}

		// inits the prompt for resizing a level - Resize_Underlying does actual resizing
		void Resize ()
		{
			// sets the state for the level resize control/prompt
			state = STATE_PROMPT;

			// initialise a control prompt for a level resize
			Control::Create(Control::TYPE_RESIZE, levelwidth, levelheight);
			// set the cursor to arrow whilst handling a control/prompt
			Cursor::SetCursor(Cursor::CURSOR_ARROW);

			// unset any current tooltips
			Tooltip::Unset();
		}

		// the actual implementation of resizing a level
		void Resize_Underlying (int _nwidth, int _nheight, Control::Resize::Dir _dir, bool _addhistory = false)
		{
			// cache the new level size, size difference, and the chosen alignment to resize
			int nwidth = _nwidth, nheight = _nheight;
			int horzdiff = nwidth - levelwidth, vertdiff = nheight - levelheight;
			Control::Resize::Dir dir = _dir;

			// no point even continuing if there is no difference
			if (horzdiff == 0 && vertdiff == 0) { return; }

			// start a new history state to restore the resize
			if (_addhistory) {
				NewHistoryState(ACTION_RESIZE);
				history.back().newlevelsize = { nwidth, nheight };
				history.back().resizedir = dir;
			}

			// store the old tile data for the level
			LVL::MapArray oldtiledata = tiledata;
			// clear and allocate the space for the new level
			for (auto& layer: tiledata) {
				layer.clear();
				layer.resize(nwidth*nheight, 0);
			}

			// the default values for the resizing of the level data
			int lvlw = levelwidth, lvlh = levelheight;
			int lvlx = (nwidth - lvlw) / 2, lvly = (nheight - lvlh) / 2;
			int offx = 0, offy = 0;

			// determine the content offset needed if shrinking the level down
			if (horzdiff < 0) {
				if (Control::IsWest(dir)) { lvlw -= abs(horzdiff); }
				else if (Control::IsEast(dir)) { offx += abs(horzdiff); lvlw -= abs(horzdiff); }
				else { offx += (abs(horzdiff) / 2), lvlw -= abs(horzdiff); } }
			if (vertdiff < 0) {
				if (Control::IsNorth(dir)) { lvlh -= abs(vertdiff); }
				else if (Control::IsSouth(dir)) { offy += abs(vertdiff); lvlh -= abs(vertdiff); }
				else { offy += (abs(vertdiff) / 2), lvlh -= abs(vertdiff); } }

			// determine the horizontal position of the level content
			if (Control::IsWest(dir)) { lvlx = 0; }
			else if (Control::IsEast(dir)) { lvlx = nwidth - lvlw; }
			// determine the vertical alignment of the level content
			if (Control::IsNorth(dir)) { lvly = 0; }
			else if (Control::IsSouth(dir)) { lvly = nheight - lvlh; }

			// make sure they aren't out of bounds
			if (lvlx < 0) { lvlx = 0; }
			if (lvly < 0) { lvly = 0; }

			// copy the actual level data over - layer by layer
			int oldlayer = 0;
			for (auto& layer: tiledata) {
				for (int iy = 0; iy < lvlh; ++iy) {
					for (int ix = 0; ix < lvlw; ++ix) {
						layer.at((iy+lvly) * nwidth + (ix+lvlx)) = oldtiledata.at(oldlayer).at((iy+offy) * levelwidth + (ix+offx));
					}
				}
				++oldlayer;
			}

			// if shrinking then cache the neccessary tile state in history
			if ((_addhistory) && (horzdiff < 0 || vertdiff < 0)) {
				for (int i = 0; i < LVL::LAYER_TOTAL; ++i) {
					auto& layer = oldtiledata.at(i);
					for (int iy = 0; iy < levelheight; ++iy) {
						for (int ix = 0; ix < levelwidth; ++ix) {
							HistoryAddToState(i, iy * levelwidth + ix, layer.at(iy * levelwidth + ix), 0);
						}
					}
				}
			}

			// save the new level width and height and recalculate editor area
			levelwidth = nwidth, levelheight = nheight;
			CalculateArea();
		}

		// @HACKY: these following two functions exist so that I can invoke a flip
		// from undo and redo without then adding another flip to the history

		void FlipHorz_Underlying (std::array<bool, LVL::LAYER_TOTAL> _layerstate = islayeractive)
		{
			// flips each layer one at a time
			std::size_t layerindex = 0;
			for (auto& layer: tiledata) {
				// we do not flip locked/invisible layers only visible/unlocked ones
				if (_layerstate.at(layerindex)) {
					// can't be asked to deal with c++ iterator bullshit, etc, just doing it in a c-style
					std::uint32_t* tiles = &layer[0];
					std::uint32_t temp;
					// caching shit so it might be faster maybe :D
					std::size_t middle = (levelwidth / 2); // swapping after middle would undo flip
					// go row-by-row swapping individual tiles as memory is not contiguous so no memcpy :(
					for (std::size_t iy = 0; iy < levelheight; ++iy) {
						for (std::size_t ix = 0; ix < middle; ++ix) {
							// @TOOD: OPTIMISE BUT CAN'T BE ASKED TO DEAL WITH C++ SHIT RIGHT NOW (IT IS INSTANT THOUGH... SO MAYBE NOT)
							std::size_t lpos = iy * levelwidth + ix;
							std::size_t rpos = iy * levelwidth + ((levelwidth-1)-ix);
							temp = tiles[lpos];
							tiles[lpos] = GetTileHorzFlipID(layerindex, tiles[rpos]);
							tiles[rpos] = GetTileHorzFlipID(layerindex, temp);
						}
					}
				}
				++layerindex;
			}
			// update the graphics to represent changes
			// RefreshLayerGraphics();
			// unsaved changes are present
			UnsavedChanges();
		}

		void FlipVert_Underlying (std::array<bool, LVL::LAYER_TOTAL> _layerstate = islayeractive)
		{
			// flips each layer one at a time
			std::size_t layerindex = 0;
			for (auto& layer: tiledata) {
				// we do not flip locked/invisible layers only visible/unlocked ones
				if (_layerstate.at(layerindex)) {
					// can't be asked to deal with c++ iterator bullshit, etc, just doing it in a c-style
					std::uint32_t* tiles = &layer[0];
					std::vector<std::uint32_t> temp;
					temp.resize(levelwidth);
					// caching shit so it might be faster maybe :D
					std::size_t middle = (levelheight / 2) * levelwidth; // swapping after middle would undo flip
					std::size_t rowsize = levelwidth * sizeof(std::uint32_t);
					std::size_t lvlsize = levelwidth * levelheight;
					// swaps out each row of tiles (top-to-bottom)
					for (std::size_t i = 0; i < middle; i += levelwidth) {
						// @TOOD: OPTIMISE BUT CAN'T BE ASKED TO DEAL WITH C++ RIGHT NOW (IT IS INSTANT THOUGH... SO MAYBE NOT)
						std::memcpy((void*)&temp[0], (void*)&tiles[i], rowsize);
						std::memcpy((void*)&tiles[i], (void*)&tiles[((lvlsize-1)-i)-(levelwidth-1)], rowsize);
						std::memcpy((void*)&tiles[((lvlsize-1)-i)-(levelwidth-1)], (void*)&temp[0], rowsize);
						// performs an extra actin to correctly flip multi-directional tiles on the row
						for (std::size_t j = 0; j < levelwidth; ++j) {
							std::size_t upos = i + j;
							std::size_t bpos = ((lvlsize-levelwidth) - i) + j;
							tiles[upos] = GetTileVertFlipID(layerindex, tiles[upos]);
							tiles[bpos] = GetTileVertFlipID(layerindex, tiles[bpos]);
						}
					}
				}
				++layerindex;
			}
			// update the graphics to represent changes
			// RefreshLayerGraphics();
			// unsaved changes are present
			UnsavedChanges();
		}

		void Undo_Underlying (bool _singleupdate)
		{
			// we don't do anything if this is the case
			if (historypos <= -1) { return; }

			// if the action was a flip, just revert it
			if (history.at(historypos).action == ACTION_FLIP_HORZ) { FlipHorz_Underlying(history.at(historypos).layerstate); }
			else if (history.at(historypos).action == ACTION_FLIP_VERT) { FlipVert_Underlying(history.at(historypos).layerstate); }
			// do other shit if it was a more "complex" action
			else {
				// if it was a resize make sure to resize the level before reverting state
				if (history.at(historypos).action == ACTION_RESIZE) {
					Resize_Underlying(history.at(historypos).oldlevelsize.x, history.at(historypos).oldlevelsize.y, history.at(historypos).resizedir);
				}

				// revert the state to remove the last action
				auto& state = history.at(historypos).state;
				for (auto& tile: state) {
					tiledata.at(tile.layer).at(tile.pos) = tile.oldid;
				}
			}

			// we don't want to do this stuff if we're doing more than one update (slow)
			if (_singleupdate) {
				// update graphics to represent changes
				// RefreshLayerGraphics();
				// unsaved changes are present
				UnsavedChanges();
			}

			// go back to the previous action
			if (historypos > -1) { --historypos; }
		}

		void Undo ()
		{
			Undo_Underlying(true);
		}

		void Redo_Underlying (bool _singleupdate)
		{
			// go forward to the next action
			if (historypos < (int)history.size()-1) { ++historypos; }
			else { return; }

			// if the action was a flip, just revert it
			if (history.at(historypos).action == ACTION_FLIP_HORZ) { FlipHorz_Underlying(history.at(historypos).layerstate); }
			else if (history.at(historypos).action == ACTION_FLIP_VERT) { FlipVert_Underlying(history.at(historypos).layerstate); }
			// if it was a resize make sure to resize the level and there is no need to restore any state
			else if (history.at(historypos).action == ACTION_RESIZE) {
				Resize_Underlying(history.at(historypos).newlevelsize.x, history.at(historypos).newlevelsize.y, history.at(historypos).resizedir); }
			// do other shit if it was a more "complex" action
			else {
				// revert the state to re-add the last action
				auto& state = history.at(historypos).state;
				for (auto& tile: state) {
					tiledata.at(tile.layer).at(tile.pos) = tile.newid;
				}
			}

			// we don't want to do this stuff if we're doing more than one update (slow)
			if (_singleupdate) {
				// update graphics to represent changes
				// RefreshLayerGraphics();
				// unsaved changes are present
				UnsavedChanges();
			}
		}

		void Redo ()
		{
			Redo_Underlying(true);
		}

		void HistoryBegin ()
		{
			// cycles through history until the beginning
			while (historypos > -1) { Undo_Underlying(false); }

			// update graphics to represent changes
			// RefreshLayerGraphics();
			// unsaved changes are present
			UnsavedChanges();
		}

		void HistoryEnd ()
		{
			// cycles through history until the end
			while (historypos < (int)history.size()-1) { Redo_Underlying(false); }

			// update graphics to represent changes
			// RefreshLayerGraphics();
			// unsaved changes are present
			UnsavedChanges();
		}

		void FlipHorz ()
		{
			// add the flip to the history
			NewHistoryState(ACTION_FLIP_HORZ);
			// perform the actual horizontal flip
			FlipHorz_Underlying();
		}

		void FlipVert ()
		{
			// add the flip to the history
			NewHistoryState(ACTION_FLIP_VERT);
			// perform the actual vertical flip
			FlipVert_Underlying();
		}

		void ToggleMirrorHorz ()
		{
			Hotbar::ToggleGraphic(Hotbar::BUTTON_MIRRORTOGGLE_HORZ);
			mirrorhorz = !mirrorhorz;
		}

		void ToggleMirrorVert ()
		{
			Hotbar::ToggleGraphic(Hotbar::BUTTON_MIRRORTOGGLE_VERT);
			mirrorvert = !mirrorvert;
		}

		void SetToolPencil ()
		{
			// don't bother if already pencil
			if (tool == TOOL_PENCIL) { return; }

			// carry out neccessary changes
			Hotbar::ToggleGraphic((Hotbar::ButtonId)tool);
			tool = TOOL_PENCIL;
			Hotbar::ToggleGraphic((Hotbar::ButtonId)tool);

			cursor = Cursor::CURSOR_PENCIL;
			cursorunset = true;
		}

		void SetToolFill ()
		{
			// don't bother if already fill
			if (tool == TOOL_FILL) { return; }

			// carry out neccessary changes
			Hotbar::ToggleGraphic((Hotbar::ButtonId)tool);
			tool = TOOL_FILL;
			Hotbar::ToggleGraphic((Hotbar::ButtonId)tool);

			cursor = Cursor::CURSOR_FILL;
			cursorunset = true;
		}

		void SetToolSelect ()
		{
			// don't bother if already select
			if (tool == TOOL_SELECT) { return; }

			// carry out neccessary changes
			Hotbar::ToggleGraphic((Hotbar::ButtonId)tool);
			tool = TOOL_SELECT;
			Hotbar::ToggleGraphic((Hotbar::ButtonId)tool);

			cursor = Cursor::CURSOR_SELECT;
			cursorunset = true;
		}

		void ToggleGrid ()
		{
			Hotbar::ToggleGraphic(Hotbar::BUTTON_GRIDTOGGLE);
			gridvisible = !gridvisible;
		}

		void ToggleCamera ()
		{
			Hotbar::ToggleGraphic(Hotbar::BUTTON_CAMERATOGGLE);
			cameravisible = !cameravisible;
		}

		void UpdateGameSettings (bool _value)
		{
			// do not attempt to modify the settings if the user doesn't want them modified
			if (!allowsettingsmods) { return; }

			// will store the filedata of the settings file
			std::string filedata;
			std::size_t start, end;

			// get the entire contents of the settings file loaded into memory
			std::ifstream input(gamepath + GAME_SETTINGS_NAME);
			if (!input.is_open()) {
				ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to read game settings!");
				return;
			}
			std::stringstream stream;
			stream << input.rdbuf();
			filedata = stream.str();
			input.close();

			// conver the desired boolean value into a string
			std::string value = ((_value) ? "true" : "false");

			// change the fast start setting
			start = filedata.find(SETTINGS_MODDING_FASTSTART) + std::strlen(SETTINGS_MODDING_FASTSTART) + 1; // +1 for the space
			end = filedata.find('\n', start);
			filedata.replace(start, end-start, value);

			// write the data back to the settings file
			std::ofstream output(gamepath + GAME_SETTINGS_NAME, std::ios::trunc);
			if (!output.is_open()) {
				ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to modify game settings!");
				return;
			}
			output << filedata;
			output.close();
		}

		void LaunchGame ()
		{
			// can't launch the game if there is no path
			if (gamepath.empty()) {
				ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "A valid path must be set to run the game!");
				return;
			}

			// launch the TEIN executable
			std::string executable = gamepath + GAME_EXECUTABLE_NAME;
			Platform::RunExecutable(executable, gamepath);
		}

		void Run ()
		{
			// set appropriate modding settings to false then launch
			UpdateGameSettings(false);
			LaunchGame();
		}

		void RunDebug ()
		{
			// set appropriate modding settings to true then launch
			UpdateGameSettings(true);
			LaunchGame();
		}

		void SettingsMenu ()
		{
			// @TODO: ...
		}

		void Help ()
		{
			// opens the modder's manual in default browser
			Platform::LoadWebpage(HELP_PAGE_URL);
		}

		void Pencil ()
		{
			// don't handle pencil if there is no active layers as content should be locked (kind of hacky)
			if (TilePanel::AllLayersAreInactive()) { return; }

			// get the current tile position that has been hit
			int tilex, tiley;
			GetTilePos(tilex, tiley);
			// creates the neww tile
			CreateTile(tilex, tiley, TilePanel::GetActiveLayer(), ((state == STATE_DRAW) ? TilePanel::GetActiveID() : 0));
			// unsaved changes are present
			UnsavedChanges();
		}

		bool FloodFill_InSelect (int _tilex, int _tiley, int _sx, int _sy, int _sw, int _sh)
		{
			return (_tilex >= _sx && _tilex < _sw && _tiley >= _sy && _tiley < _sh);
		}

		void FloodFill (int _tilelayer, int _tilex, int _tiley, std::uint32_t _replaceid, std::uint32_t _activeid, bool _outsideselect)
		{
			// @OPTIMISE: SEE THE SECOND ALTERNATIVE IMPLEMENTATION - https://en.wikipedia.org/wiki/Flood_fill

			// do not fill in these particular cases
			if (_replaceid == _activeid) { return; }
			if (tiledata.at(_tilelayer).at(_tiley * levelwidth + _tilex) != _replaceid) { return; }

			// sort out the selection area properly
			int sx, sy, sw, sh;
			if (selectbox.x != 0 || selectbox.w != 0 || selectbox.y != 0 || selectbox.h != 0) { GetSelectRect(sx, sy, sw, sh); }
			else { sx = -1, sy = -1, sw = -1, sh = -1; }

			// cache this for quick access
			auto& layer = tiledata.at(_tilelayer);

			// create the queue for storing the elements to be filled
			std::queue<SDL_Point> tofill;
			CreateTile(_tilex, _tiley, _tilelayer, _activeid, true, true);
			tofill.push(SDL_Point { _tilex, _tiley });

			// go through until the queue is depleted and everything has been searched
			while (!tofill.empty()) {
				// fills in the tile at the end of the queue
				SDL_Point curtile = tofill.front();
				tofill.pop();

				// checks all the neighbouring elements
				if ((curtile.x < levelwidth-1) && (layer.at(curtile.y * levelwidth + (curtile.x+1)) == _replaceid)) {
					if (((!FloodFill_InSelect(curtile.x+1,curtile.y, sx,sy,sw,sh)) && (_outsideselect)) ||
						((FloodFill_InSelect(curtile.x+1,curtile.y, sx,sy,sw,sh)) && (!_outsideselect))) {
						CreateTile(curtile.x+1, curtile.y, _tilelayer, _activeid, true, true);
						tofill.push(SDL_Point { curtile.x+1, curtile.y });
					}
				}
				if ((curtile.x > 0) && (layer.at(curtile.y * levelwidth + (curtile.x-1)) == _replaceid)) {
					if (((!FloodFill_InSelect(curtile.x-1,curtile.y, sx,sy,sw,sh)) && (_outsideselect)) ||
						((FloodFill_InSelect(curtile.x-1,curtile.y, sx,sy,sw,sh)) && (!_outsideselect))) {
						CreateTile(curtile.x-1, curtile.y, _tilelayer, _activeid, true, true);
						tofill.push(SDL_Point { curtile.x-1, curtile.y });
					}
				}
				if ((curtile.y > 0) && (layer.at((curtile.y-1) * levelwidth + curtile.x) == _replaceid)) {
					if (((!FloodFill_InSelect(curtile.x,curtile.y-1, sx,sy,sw,sh)) && (_outsideselect)) ||
						((FloodFill_InSelect(curtile.x,curtile.y-1, sx,sy,sw,sh)) && (!_outsideselect))) {
						CreateTile(curtile.x, curtile.y-1, _tilelayer, _activeid, true, true);
						tofill.push(SDL_Point { curtile.x, curtile.y-1 });
					}
				}
				if ((curtile.y < levelheight-1) && (layer.at((curtile.y+1) * levelwidth + curtile.x) == _replaceid)) {
					if (((!FloodFill_InSelect(curtile.x,curtile.y+1, sx,sy,sw,sh)) && (_outsideselect)) ||
						((FloodFill_InSelect(curtile.x,curtile.y+1, sx,sy,sw,sh)) && (!_outsideselect))) {
						CreateTile(curtile.x, curtile.y+1, _tilelayer, _activeid, true, true);
						tofill.push(SDL_Point { curtile.x, curtile.y+1 });
					}
				}
			}

			/*
			// do not fill in these particular cases
			if (_replaceid == _activeid) { return; }
			if (tiledata.at(_tilelayer).at(_tiley * levelwidth + _tilex) != _replaceid) { return; }
			if (selectbox.x != 0 || selectbox.w != 0 || selectbox.y != 0 || selectbox.h != 0) {
				// sort out the selection area properly
				int sx, sy, sw, sh;
				GetSelectRect(sx, sy, sw, sh);
				// handle whether we're inside or outside the selection area
				// inside select box but the origin was outside, quit
				if (_tilex < sx || _tilex >= sw || _tiley < sy || _tiley >= sh) { if (_outsideselect) { return; } }
				else { if (!_outsideselect) { return; } } // outside the select box but the origin was inside, quit
			}

			// creates the new tile
			// do mirror but don't update graphics (slow)
			if (!CreateTile(_tilex, _tiley, _tilelayer, _activeid, true, true)) { return; }

			// continue on with the flood-fill in other directions
			if ((_tiley-1) >= 0)               { FloodFill(_tilelayer, _tilex,   _tiley-1, _replaceid, _activeid, _outsideselect); } // north
			if ((_tilex+1) < (int)levelwidth)  { FloodFill(_tilelayer, _tilex+1, _tiley,   _replaceid, _activeid, _outsideselect); } // east
			if ((_tiley+1) < (int)levelheight) { FloodFill(_tilelayer, _tilex,   _tiley+1, _replaceid, _activeid, _outsideselect); } // south
			if ((_tilex-1) >= 0)               { FloodFill(_tilelayer, _tilex-1, _tiley,   _replaceid, _activeid, _outsideselect); } // west
			*/
		}

		void Replace (int _tilelayer, std::uint32_t _replaceid, std::uint32_t _activeid, bool _outsideselect)
		{
			// get the layer we'll be dealing with
			auto& layer = tiledata.at(_tilelayer);
			// go through every tile and replace all instance of _replaceid with _activeid
			int index = -1;
			for (auto& tileid: layer) {
				// update at the start so I don't have to in the continue branches
				++index;
				// do not replace in these instances
				if (tileid != _replaceid) { continue; }
				if (selectbox.x != 0 || selectbox.w != 0 || selectbox.y != 0 || selectbox.h != 0) {
					// calculate x and y pos
					int tilex = index % levelwidth;
					int tiley = index / levelwidth;
					// sort out the selection area properly
					int sx, sy, sw, sh;
					GetSelectRect(sx, sy, sw, sh);
					// handle whether we're inside or outside the selection area
					// inside select box but the origin was outside, move on
					if (tilex >= sx && tilex < sw && tiley >= sy && tiley < sh) { if (_outsideselect) { continue; } }
					else { if (!_outsideselect) { continue; } } // outside the select box but the origin was inside, move on
				}
				// add the information to the history state
				if (!HistoryAddToState(_tilelayer, index, _replaceid, _activeid)) { continue; }
				// update the tile itself
				tileid = _activeid;
			}
		}

		void Fill ()
		{
			// don't handle fill/replace if there is no active layers as content should be locked (kind of hacky)
			if (TilePanel::AllLayersAreInactive()) { return; }

			// get the current tile position that has been hit
			int tilex, tiley;
			GetTilePos(tilex, tiley);
			// determine information regarding the tile (location, new tile ID, etc.)
			int activelayer = TilePanel::GetActiveLayer();
			int activeid;
			// if we're doing an erase fill, then the active ID must be 0
			if (state == STATE_DRAW) { activeid = TilePanel::GetActiveID(); }
			else { activeid = 0; }

			// determine what the tile that will be filled over is
			int replaceid = tiledata.at(activelayer).at(tiley * levelwidth + tilex);

			// determine if the origin point is inside or outside a selection (if one is present)
			bool outsideselect = true;
			if (selectbox.x != 0 || selectbox.w != 0 || selectbox.y != 0 || selectbox.h != 0) {
				// sort out the selection area properly
				int sx, sy, sw, sh;
				GetSelectRect(sx, sy, sw, sh);
				// if out of selection area, do not fill
				if (tilex >= sx && tilex < sw && tiley >= sy && tiley < sh) { outsideselect = false; }
			}

			// determine whether we are doing a replacement fill or a normal fill
			if (replacefill) { Replace(activelayer, replaceid, activeid, outsideselect); }
			else {
				// perform a flood fill algorithm to fill all connected instances of replaceid with activeid
				// uses the stack-based method seen here: https://en.wikipedia.org/wiki/Flood_fill#The_algorithm
				FloodFill(activelayer, tilex, tiley, replaceid, activeid, outsideselect);
			}

			// updates the layer graphics to reflect changess
			// RefreshLayerGraphics();
			// unsaved changes are present
			UnsavedChanges();
		}

		void Deselect ()
		{
			// deselects the current selection
			selectbox = {};
			selectvisible = false;
		}

		void SelectAll ()
		{
			// selects everything in the editor
			selectbox = { 0, 0, (float)levelwidth, (float)levelheight };
			selectvisible = true;
		}

		void Select ()
		{
			// cache these values for speed
			int xoffpos = GetXOffsetPos();
			int yoffpos = GetYOffsetPos();

			// starts handling a brand new selection
			if (selectstart) {
				selectbox = {};
				int curx, cury;
				SDL_GetMouseState(&curx, &cury);
				selectbox.x = std::floor((curx - xoffpos) / tilesize);
				selectbox.y = std::floor((cury - yoffpos) / tilesize);
				selectvisible = true;
				selectstart = false;
				select = true;
			}

			// sets the location of the other corner of the select tool
			if (select) {
				// determines the tile to clip the select box tool
				int curx, cury;
				SDL_GetMouseState(&curx, &cury);
				selectbox.w = std::floor((curx - xoffpos) / tilesize) + 1;
				selectbox.h = std::floor((cury - yoffpos) / tilesize) + 1;

				// the fringe will scale based on zoom
				int fringe = SELECT_DRAG_FRINGE * currentzoom;

				// determine the area that is not the select fringe
				int edgex = TilePanel::WIDTH+fringe, edgey = Hotbar::HEIGHT+fringe;
				int edgew = (App::GetWidth()-fringe)-edgex, edgeh = (App::GetHeight()-fringe)-edgey;

				// will pan the camera if on the edge of the screen - determine which direction
				if (!Cursor::Within(edgex, 0, edgew, App::GetHeight())) { xoffset += ((curx < (App::GetWidth() / 2)) ? CAMERA_PAN_SPEED : -CAMERA_PAN_SPEED); }
				if (!Cursor::Within(0, edgey, App::GetWidth(), edgeh)) { yoffset += ((cury < (App::GetHeight() / 2)) ? CAMERA_PAN_SPEED : -CAMERA_PAN_SPEED); }
			}
		}

		// general function because they share a lot of the code
		void CopyAndCut (bool _cut)
		{
			// don't do anything if there's no selection
			if (selectbox.x == 0 && selectbox.w == 0 && selectbox.y == 0 && selectbox.h == 0) { return; }

			// add a new history state if we're cutting
			if (_cut) { NewHistoryState(ACTION_STANDARD); }

			// sort out the selection area properly
			int sx, sy, sw, sh;
			GetSelectRect(sx, sy, sw, sh);

			// clear and resize the selection buffer (less memory allocation)
			selectwidth = (sw - sx);
			selectheight = (sh - sy);
			for (auto& layer: selectbuffer) {
				layer.clear();
				layer.resize(selectwidth * selectheight);
			}

			// @OPTIMISE: USE MEMCPY FOR THE CONTIGUOUS STRIPS OF MEMORY
			// copy all selected tiles from each layer into the buffer
			std::size_t layerindex = 0;
			for (auto& layer: selectbuffer) {
				// we only want to handle layers that have content unlocked
				if (islayeractive.at(layerindex)) {
					for (int iy = sy; iy < sh; ++iy) {
						for (int ix = sx; ix < sw; ++ix) {
							int tilepos = iy * levelwidth + ix;
							layer.at((iy-sy) * selectwidth + (ix-sx)) = tiledata.at(layerindex).at(tilepos);
						}
					}
				}
				++layerindex;
			}

			if (_cut) {
				// if cutting then we also remove the tiles from the map
				// we do this here so that the clipboard is not filled with semi-deleted data
				for (std::size_t layer = 0; layer < LVL::LAYER_TOTAL; ++layer) {
					// we only want to handle layers that have content unlocked
					if (islayeractive.at(layer)) {
						for (int iy = sy; iy < sh; ++iy) {
							for (int ix = sx; ix < sw; ++ix) {
								if (!CreateTile(ix, iy, layer, 0, true, true)) { continue; }
							}
						}
					}
				}

				// we also want to deselect the selection and update if cutting
				// RefreshLayerGraphics();
				Deselect();
				// unsaved changes are present
				UnsavedChanges();
			}
		}

		void Copy ()
		{
			// performs a copy
			CopyAndCut(false);
		}

		void Cut ()
		{
			// performs a cut
			CopyAndCut(true);
		}

		void Paste ()
		{
			// don't bother pasting if empty clipboard/selectbuffer or out of bounds
			bool empty = true;
			for (auto& layer: selectbuffer) { if (!layer.empty()) { empty = false; break; } }
			if (empty || !Cursor::Within(GetXOffsetPos(), GetYOffsetPos(), area.w, area.h)) { return; }

			// we want to add the paste action to the history
			NewHistoryState(ACTION_STANDARD);

			// the location to start pasting at
			int startx, starty;
			GetTilePos(startx, starty);

			// begin pasting, making sure to not go out of bounds
			std::size_t layerindex = 0;
			for (auto& layer: selectbuffer) {
				// we only want to handle layers that have content unlocked
				if (islayeractive.at(layerindex)) {
					for (int iy = starty; iy < (starty + selectheight); ++iy) {
						if (iy >= (int)levelheight) { break; } // out-of-bounds
						for (int ix = startx; ix < (startx + selectwidth); ++ix) {
							if (ix >= (int)levelwidth) { break; } // out-of-bounds
							// does the actual adding and also stores state in history
							int bufferpos = (iy-starty) * selectwidth + (ix-startx);
							if (!CreateTile(ix, iy, layerindex, layer.at(bufferpos), true, true)) { continue; }
						}
					}
				}
				++layerindex;
			}

			// refresh the graphics to reflect changes and deselect
			// RefreshLayerGraphics();
			Deselect();
			// unsaved changes are present
			UnsavedChanges();
		}

		void Clear ()
		{
			// don't do anything if there's no selection
			if (selectbox.x == 0 && selectbox.w == 0 && selectbox.y == 0 && selectbox.h == 0) { return; }

			// we want to add the clear action to the history
			NewHistoryState(ACTION_STANDARD);

			// sort out the selection area properly
			int sx, sy, sw, sh;
			GetSelectRect(sx, sy, sw, sh);

			// clears all tiles within the current selection
			for (std::size_t layer = 0; layer < LVL::LAYER_TOTAL; ++layer) {
				// we only want to handle layers that have content unlocked
				if (islayeractive.at(layer)) {
					for (int iy = sy; iy < sh; ++iy) {
						for (int ix = sx; ix < sw; ++ix) {
							if (!CreateTile(ix, iy, layer, 0, true, true)) { continue; }
						}
					}
				}
			}

			// refresh the graphics to reflect changes and deselect
			// RefreshLayerGraphics();
			Deselect();
			// unsaved changes are present
			UnsavedChanges();
		}

		void ToggleLayerActive ()
		{
			islayeractive.at(LVL::LAYER_ACTIVE) = !islayeractive.at(LVL::LAYER_ACTIVE);
			TilePanel::ToggleLayer(LVL::LAYER_ACTIVE);
		}

		void ToggleLayerTag ()
		{
			islayeractive.at(LVL::LAYER_TAG) = !islayeractive.at(LVL::LAYER_TAG);
			TilePanel::ToggleLayer(LVL::LAYER_TAG);
		}

		void ToggleLayerOverlay ()
		{
			islayeractive.at(LVL::LAYER_OVERLAY) = !islayeractive.at(LVL::LAYER_OVERLAY);
			TilePanel::ToggleLayer(LVL::LAYER_OVERLAY);
		}

		void ToggleLayerBack1 ()
		{
			islayeractive.at(LVL::LAYER_BACK1) = !islayeractive.at(LVL::LAYER_BACK1);
			TilePanel::ToggleLayer(LVL::LAYER_BACK1);
		}

		void ToggleLayerBack2 ()
		{
			islayeractive.at(LVL::LAYER_BACK2) = !islayeractive.at(LVL::LAYER_BACK2);
			TilePanel::ToggleLayer(LVL::LAYER_BACK2);
		}

		void RealignCamera ()
		{
			// reset the current camera zoom and recalculate the area
			currentzoom = 1.0f;
			CalculateArea();

			// get available area for editor to be placed in
			float validareaw = (App::GetWidth() - TilePanel::WIDTH) - PADDING;
			float validareah = (App::GetHeight() - Hotbar::HEIGHT) - PADDING;

			// @TODO: MAKE A LOT OF THESE VALUES FLOATS BY DEFAULT TO AVOID CASTING? (NEED TO BE FLOATS OR ELSE CALCULATION BREAKS ON ZOOM)
			// calculate the default offset for the camera (to add some padding)
			xoffset = (float)((validareaw - (((float)tilesize * (float)DEFAULT_LEVELWIDTH) - (float)PADDING)) / 2.0f) / (float)tilesize;
			yoffset = (float)((validareah - (((float)tilesize * (float)DEFAULT_LEVELHEIGHT) - (float)PADDING)) / 2.0f) / (float)tilesize;

			// cache the default camera offset - used when resizing the window
			xoffset_def = xoffset, yoffset_def = yoffset;
		}

		void ZoomDecrement ()
		{
			// zooms out by half the current zoom
			currentzoom /= 2.0f;
			if (currentzoom < ZOOM_MIN) { currentzoom = ZOOM_MIN; return; } // already at min zoom

			// recalculates important values (e.g. tilesize)
			CalculateArea();

			// @IMPROVE: DUPLICATE CODE CAN BE PLACED IN INLINE FUNCTION?
			// performs the neccessary offset to make the zoom centered (must be done with the new tilesize)
			xoffset += (((float)(App::GetWidth() - TilePanel::WIDTH) / (float)tilesize) / 4.f);
			yoffset += (((float)(App::GetHeight() - Hotbar::HEIGHT) / (float)tilesize) / 4.f);
		}

		void ZoomIncrement ()
		{
			// zooms in by half the current zoom
			currentzoom *= 2.0f;
			if (currentzoom > ZOOM_MAX) { currentzoom = ZOOM_MAX; return; } // already at max zoom

			// @IMPROVE: DUPLICATE CODE CAN BE PLACED IN INLINE FUNCTION?
			// performs the neccessary offset to make the zoom centered (must be done with the old tilesize)
			xoffset -= (((float)(App::GetWidth() - TilePanel::WIDTH) / (float)tilesize) / 4.f);
			yoffset -= (((float)(App::GetHeight() - Hotbar::HEIGHT) / (float)tilesize) / 4.f);

			// recalculates important values (e.g. tilesize)
			CalculateArea();
		}
	}

	void Init ()
	{
		// initialise the other sub-systems of the editor
		Hotbar::Init();
		TilePanel::Init();

		// if a level was dragged onto the executable load it immediately
		if (App::GetCmdArgCount() > 1) {
			currentfile = App::GetCmdArg(1); // first arg after exe name
			LVL::Load(currentfile, tiledata, levelwidth, levelheight);
			App::AppendCaption(" (" + currentfile + ")");
		}
		// otherwise, just set a caption for the window
		else { App::AppendCaption(" (Untitled)"); }

		// calculate the level editor area + grid and offset/pan
		CalculateArea();
		RealignCamera();

		// dynamically allocate the tiledata to fit the level size
		for (auto& layer: tiledata) { layer.resize(levelwidth * levelheight); }

		// load the path where the game (and game settings file) is located
		gamepath = Settings::GetString("tein_game_path");
		// if we are allowed to make settings modifications, make a backup we can restore to after use
		allowsettingsmods = Settings::GetBoolean("allow_settings_modifications");
		if (allowsettingsmods) {
			bool failed = false;
			// if the gamepath is invalid inform the user ask them if they still want to allow settings modifications
			if (gamepath.empty()) { failed = true; }
			else {
				// otherwise we carry on and make the backup for later
				std::string filename = gamepath + GAME_SETTINGS_NAME;
				std::ifstream input(filename);
				if (!input.is_open()) { failed = true; }
				else {
					std::ofstream output(filename + ".bak", std::ios::trunc);
					if (!output.is_open()) { failed = true; }
					else { output << input.rdbuf(); }
				}
			}
			// if we failed to modify the settings file inform the user
			if (failed) {
				ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to create backup of TEIN settings!");
				nosettingsbackup = true;
			}
		}

		// reads in info on how different tiles map to each other when being flipped
		SDL_RWops* stream = DataManager::Load("tileflip.dat");
		std::string tileflipdata;
		tileflipdata.resize(SDL_RWsize(stream));
		if (SDL_RWread(stream, (void*)&tileflipdata[0], SDL_RWsize(stream), 1) == 0) {
			ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_SDL, "Failed to read tile flip map file!");
		}
		// cleans up the stream after use as it's no longer needed
		SDL_RWclose(stream);
		stream = nullptr;

		// converts the raw tile flip data into a usable map of tile ids
		auto lines = Utility::StrToken(tileflipdata, "\n");
		for (auto& line: lines) {
			auto tokens = Utility::StrToken(line, "|");
			std::uint8_t category = std::stoi(tokens.at(1));
			std::uint32_t id1 = std::stoi(tokens.at(2)) + LVL::OFFSET[category];
			std::uint32_t id2 = std::stoi(tokens.at(3)) + LVL::OFFSET[category];
			std::uint8_t layer = TilePanel::CategoryToLayer(category, id1); // it is assumed id2 is from the same category+layer
			if (tokens.at(0) == "h") { flipmaphorz.at(layer).push_back(std::make_pair(id1, id2)); }
			else if (tokens.at(0) == "v") { flipmapvert.at(layer).push_back(std::make_pair(id1, id2)); }
		}

		// loads in user-defined color values for different visual elements of the editor
		camerabounds_valid_color = Settings::GetColor("camerabounds_valid_color");
		camerabounds_invalid_color = Settings::GetColor("camerabounds_invalid_color");
		selectbox_color = Settings::GetColor("selectbox_color");
		cursor_tile_color = Settings::GetColor("cursor_tile_color");
		leveleditor_background_color = Settings::GetColor("leveleditor_background_color");
		leveleditor_grid_color = Settings::GetColor("leveleditor_grid_color");
		symmetry_lines_color = Settings::GetColor("symmetry_lines_color");

		// loads in user-defined alpha values for different layer states
		layer_active_alpha[LVL::LAYER_ACTIVE] = Settings::GetInteger("layer_active_basic_alpha");
		layer_active_alpha[LVL::LAYER_TAG] = Settings::GetInteger("layer_active_tag_alpha");
		layer_active_alpha[LVL::LAYER_OVERLAY] = Settings::GetInteger("layer_active_overlay_alpha");
		layer_active_alpha[LVL::LAYER_BACK1] = Settings::GetInteger("layer_active_back1_alpha");
		layer_active_alpha[LVL::LAYER_BACK2] = Settings::GetInteger("layer_active_back2_alpha");
		layer_inactive_alpha[LVL::LAYER_ACTIVE] = Settings::GetInteger("layer_inactive_basic_alpha");
		layer_inactive_alpha[LVL::LAYER_TAG] = Settings::GetInteger("layer_inactive_tag_alpha");
		layer_inactive_alpha[LVL::LAYER_OVERLAY] = Settings::GetInteger("layer_inactive_overlay_alpha");
		layer_inactive_alpha[LVL::LAYER_BACK1] = Settings::GetInteger("layer_inactive_back1_alpha");
		layer_inactive_alpha[LVL::LAYER_BACK2] = Settings::GetInteger("layer_inactive_back2_alpha");

		// set actions for the hotbar buttons
		Hotbar::SetAction(Hotbar::BUTTON_NEW, New);
		Hotbar::SetAction(Hotbar::BUTTON_OPEN, Open);
		Hotbar::SetAction(Hotbar::BUTTON_SAVE, Save);
		Hotbar::SetAction(Hotbar::BUTTON_SAVEAS, SaveAs);
		Hotbar::SetAction(Hotbar::BUTTON_GPAK_UNPACK, Unpack);
		Hotbar::SetAction(Hotbar::BUTTON_GPAK_PACK, Pack);
		Hotbar::SetAction(Hotbar::BUTTON_UNDO, Undo);
		Hotbar::SetAction(Hotbar::BUTTON_REDO, Redo);
		Hotbar::SetAction(Hotbar::BUTTON_RESIZE, Resize);
		Hotbar::SetAction(Hotbar::BUTTON_FLIP_HORZ, FlipHorz);
		Hotbar::SetAction(Hotbar::BUTTON_FLIP_VERT, FlipVert);
		Hotbar::SetAction(Hotbar::BUTTON_MIRRORTOGGLE_HORZ, ToggleMirrorHorz);
		Hotbar::SetAction(Hotbar::BUTTON_MIRRORTOGGLE_VERT, ToggleMirrorVert);
		Hotbar::SetAction(Hotbar::BUTTON_PENCIL, SetToolPencil);
		Hotbar::SetAction(Hotbar::BUTTON_FILL, SetToolFill);
		Hotbar::SetAction(Hotbar::BUTTON_SELECT, SetToolSelect);
		Hotbar::SetAction(Hotbar::BUTTON_GRIDTOGGLE, ToggleGrid);
		Hotbar::SetAction(Hotbar::BUTTON_CAMERATOGGLE, ToggleCamera);
		Hotbar::SetAction(Hotbar::BUTTON_RUN, Run);
		Hotbar::SetAction(Hotbar::BUTTON_RUNDEBUG, RunDebug);
		Hotbar::SetAction(Hotbar::BUTTON_SETTINGS, SettingsMenu);
		Hotbar::SetAction(Hotbar::BUTTON_HELP, Help);

		// @TOOD: MODIFY THROUGH SETTINGS MENU
		// set hotkeys for the hotbar buttons
		Hotkey::Register(KMOD_CTRL, SDLK_n, New);
		Hotkey::Register(KMOD_CTRL, SDLK_o, Open);
		Hotkey::Register(KMOD_CTRL, SDLK_s, Save);
		Hotkey::Register(KMOD_CTRL|KMOD_SHIFT, SDLK_s, SaveAs);
		Hotkey::Register(KMOD_CTRL, SDLK_u, Unpack);
		Hotkey::Register(KMOD_CTRL, SDLK_p, Pack);
		Hotkey::Register(KMOD_CTRL, SDLK_z, Undo);
		Hotkey::Register(KMOD_CTRL, SDLK_y, Redo);
		Hotkey::Register(KMOD_CTRL, SDLK_r, Resize);
		Hotkey::Register(KMOD_CTRL|KMOD_SHIFT, SDLK_z, HistoryBegin);
		Hotkey::Register(KMOD_CTRL|KMOD_SHIFT, SDLK_y, HistoryEnd);
		Hotkey::Register(KMOD_NONE, SDLK_j, FlipHorz);
		Hotkey::Register(KMOD_NONE, SDLK_k, FlipVert);
		Hotkey::Register(KMOD_NONE, SDLK_n, ToggleMirrorHorz);
		Hotkey::Register(KMOD_NONE, SDLK_m, ToggleMirrorVert);
		Hotkey::Register(KMOD_NONE, SDLK_b, SetToolPencil);
		Hotkey::Register(KMOD_NONE, SDLK_f, SetToolFill);
		Hotkey::Register(KMOD_NONE, SDLK_s, SetToolSelect);
		Hotkey::Register(KMOD_NONE, SDLK_g, ToggleGrid);
		Hotkey::Register(KMOD_NONE, SDLK_c, ToggleCamera);
		Hotkey::Register(KMOD_NONE, SDLK_F6, Run);
		Hotkey::Register(KMOD_NONE, SDLK_F7, RunDebug);
		Hotkey::Register(KMOD_NONE, SDLK_F2, SettingsMenu);
		Hotkey::Register(KMOD_NONE, SDLK_F1, Help);
		Hotkey::Register(KMOD_CTRL, SDLK_d, Deselect);
		Hotkey::Register(KMOD_CTRL, SDLK_a, SelectAll);
		Hotkey::Register(KMOD_CTRL, SDLK_c, Copy);
		Hotkey::Register(KMOD_CTRL, SDLK_x, Cut);
		Hotkey::Register(KMOD_CTRL, SDLK_v, Paste);
		Hotkey::Register(KMOD_NONE, SDLK_DELETE, Clear);
		Hotkey::Register(KMOD_CTRL, SDLK_1, ToggleLayerActive);
		Hotkey::Register(KMOD_CTRL, SDLK_2, ToggleLayerTag);
		Hotkey::Register(KMOD_CTRL, SDLK_3, ToggleLayerOverlay);
		Hotkey::Register(KMOD_CTRL, SDLK_4, ToggleLayerBack1);
		Hotkey::Register(KMOD_CTRL, SDLK_5, ToggleLayerBack2);
		Hotkey::Register(KMOD_CTRL, SDLK_0, RealignCamera);
		Hotkey::Register(KMOD_CTRL, SDLK_MINUS, ZoomDecrement);
		Hotkey::Register(KMOD_CTRL, SDLK_EQUALS, ZoomIncrement);

		// pencil is the starting tool
		Hotbar::ToggleGraphic(Hotbar::BUTTON_PENCIL);
		// set the starting cursor graphic to be pencil (matches the starting tool)
		cursor = Cursor::CURSOR_PENCIL;
		cursorunset = true;

		// initialise tooltips for the tilepanel
		Tooltip::Init();

		// used in timing calculations
		#ifndef RELEASE
		LARGE_INTEGER perfcountfreqresult;
		QueryPerformanceFrequency(&perfcountfreqresult);
		perfcountfreq = perfcountfreqresult.QuadPart;
		#endif
	}

	void Quit ()
	{
		// if we allowed settings modifications we want to restore the backup
		if (allowsettingsmods && !nosettingsbackup) {
			bool failed = false;
			if (gamepath.empty()) { failed = true; }
			else {
				std::string filename = gamepath + GAME_SETTINGS_NAME;
				std::ifstream input(filename + ".bak");
				if (!input.is_open()) { failed = true; }
				else {
					std::ofstream output(filename, std::ios::trunc);
					if (!output.is_open()) { failed = true; }
					else { output << input.rdbuf(); }
				}
				// remove the backup
				input.close();
				std::remove(std::string(filename + ".bak").c_str());
			}
			// if we failed to restore the backup inform the user
			if (failed) { ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to restore backup of TEIN settings!"); }
		}
	}

	void Handle ()
	{
		if (state != STATE_PROMPT) {
			// handles the other sub-system events
			Hotbar::Handle();
			TilePanel::Handle();

			// handles any potential hotkeys that may have been pressed
			Hotkey::Handle();

			// cache for easy access and speed
			int areax = GetXOffsetPos(), areay = GetYOffsetPos();
			// calculate a custom boundary because the area can be behind the tilepanel or hotbar
			int boundsx = areax, boundsy = areay, boundsw = area.w, boundsh = area.h;
			if (boundsx < TilePanel::WIDTH) { boundsx = TilePanel::WIDTH, boundsw = area.w - (TilePanel::WIDTH - areax); }
			if (boundsy < Hotbar::HEIGHT) { boundsy = Hotbar::HEIGHT, boundsh = area.h - (Hotbar::HEIGHT - areay); }

			switch (App::event.type) {
				// quit the applicatiion, if requested
				case (SDL_QUIT): {
					// prompts the user to save before quitting
					if (SaveChanges() == Platform::MSGBOX_RESULT_CANCEL) { return; }
					App::Stop();
				} break;
				// dynamically scale editor area on window resize
				case (SDL_WINDOWEVENT): {
					switch (App::event.window.event) {
						case (SDL_WINDOWEVENT_RESIZED):
						case (SDL_WINDOWEVENT_SIZE_CHANGED): {
							// resizes these two gui elements
							Hotbar::CalculateArea();
							TilePanel::CalculateArea();
							// determines new tile, area, and grid size
							CalculateArea();
							// if the camera is at the default position then recalculate the padding
							// this is done so that the surrounding padding still looks good
							if (xoffset == xoffset_def && yoffset == yoffset_def) { RealignCamera(); }
						} break;
					}
				} break;
				// handles level files being dropped onto the application window, loading them
				case (SDL_DROPFILE): {
					// prompts the user to save before opening
					if (SaveChanges() == Platform::MSGBOX_RESULT_CANCEL) { return; }
					// load the chosen file
					currentfile = App::event.drop.file;
					LVL::Load(currentfile, tiledata, levelwidth, levelheight);
					// calculates level editor area + grid
					CalculateArea();
					// reset the camera pan/offset for the new level
					RealignCamera();
					// reset the history for the loaded file
					history.clear();
					historypos = -1;
					// sets a new caption for the window (using the new filename)
					App::AppendCaption(" (" + currentfile + ")");
					unsavedchanges = false;
				} break;
				// handle caching the relative mouse position when moved (used for panning)
				case (SDL_MOUSEMOTION): {
					relativecurx = App::event.motion.xrel;
					relativecury = App::event.motion.yrel;
				} break;
				// handle toggling drawing and erasing tiles in the level editor
				case (SDL_MOUSEBUTTONDOWN): {
					switch (App::event.button.button) {
						// handles drawing, erasing, tools, etc.
						case (SDL_BUTTON_LEFT):
						case (SDL_BUTTON_RIGHT): {
							if (Cursor::Within(boundsx, boundsy, boundsw, boundsh)) {
								// determine drawing or erasing based on button pressed
								if (App::event.button.button == SDL_BUTTON_LEFT) { state = STATE_DRAW; }
								else if (App::event.button.button == SDL_BUTTON_RIGHT) { state = STATE_ERASE; }

								// sets up the start of a new selection
								if (tool == TOOL_SELECT && App::event.button.button == SDL_BUTTON_LEFT) { selectstart = true; }
								// select handles history differently
								if (tool != TOOL_SELECT && !TilePanel::AllLayersAreInactive()) { NewHistoryState(ACTION_STANDARD); }
							}
						} break;
						// handles mouse-based panning
						case (SDL_BUTTON_MIDDLE): { panning = true; } break;
					}
				} break;
				case (SDL_MOUSEBUTTONUP): {
					switch (App::event.button.button) {
						// handles unsetting state and select stuff
						case (SDL_BUTTON_LEFT):
						case (SDL_BUTTON_RIGHT): {
							// this is here to prevent the state being reset when the user presses the 'New' button
							if (state != STATE_PROMPT) {
								// resets the current state
								state = STATE_NONE;

								// handles select stuff
								if (tool == TOOL_SELECT) {
									// if LMB is released then end a selection
									if (App::event.button.button == SDL_BUTTON_LEFT) { selectstart = false, select = false; }
									// if RMB is pressed (then released) whist using select tool, it clears the current selection
									if (Cursor::Within(boundsx, boundsy, boundsw, boundsh) && App::event.button.button == SDL_BUTTON_RIGHT) { Deselect(); }
								}
							}
						} break;
						// handles mouse-based panning
						case (SDL_BUTTON_MIDDLE): { panning = false; } break;
					}
				} break;
				// handles scrolling using the scrollwheel on the mouse (whilst holding either CTRL key)
				case (SDL_MOUSEWHEEL): {
					if (Hotkey::CheckKeyMods(KMOD_CTRL)) {
						// depending on which direction scrolled, perform the appropriate zoome
						if (App::event.wheel.y > 0) { ZoomIncrement(); }
						else if (App::event.wheel.y < 0) { ZoomDecrement(); }
					}
				} break;
				// handles keys being pressed/released for certain special case actions @TODO: MAKE THIS REBINDABLE
				case (SDL_KEYDOWN): {
					if (App::event.key.repeat == 0) {
						switch (App::event.key.keysym.sym) {
							// shift for rplace-fill
							case (SDLK_LSHIFT):
							case (SDLK_RSHIFT): { replacefill = true; } break;
							// ctrl for ghosted clipboard
							case (SDLK_LCTRL):
							case (SDLK_RCTRL):  { selectcursor = true; } break;
							// arrows for camera panning
							case (SDLK_UP):     { panningdir[PANNING_UP]    = true; } break;
							case (SDLK_RIGHT):  { panningdir[PANNING_RIGHT] = true; } break;
							case (SDLK_DOWN):   { panningdir[PANNING_DOWN]  = true; } break;
							case (SDLK_LEFT):   { panningdir[PANNING_LEFT]  = true; } break;
						}
					}
				} break;
				case (SDL_KEYUP): {
					switch (App::event.key.keysym.sym) {
						// shift for rplace-fill
						case (SDLK_LSHIFT):
						case (SDLK_RSHIFT): { replacefill = false; } break;
						// ctrl for ghosted clipboard
						case (SDLK_LCTRL):
						case (SDLK_RCTRL):  { selectcursor = false; } break;
						// arrows for camera panning
						case (SDLK_UP):     { panningdir[PANNING_UP]    = false; } break;
						case (SDLK_RIGHT):  { panningdir[PANNING_RIGHT] = false; } break;
						case (SDLK_DOWN):   { panningdir[PANNING_DOWN]  = false; } break;
						case (SDLK_LEFT):   { panningdir[PANNING_LEFT]  = false; } break;
					}
				} break;
			}
		}
		// new level control/prompt has focus
		else {
			// @IMPROVE: LOTS OF DUPLICATE CODE HERE, NOT NECCESSARY
			// quit the applicatiion, if requested
			if (App::event.type == SDL_QUIT) {
				// prompts the user to save before quitting
				if (SaveChanges() == Platform::MSGBOX_RESULT_CANCEL) { return; }
				App::Stop();
			}
			// dynamically scale editor area on window resize
			else if (App::event.type == SDL_WINDOWEVENT) {
				switch (App::event.window.event) {
					case (SDL_WINDOWEVENT_RESIZED):
					case (SDL_WINDOWEVENT_SIZE_CHANGED): {
						// resizes these two gui elements
						Hotbar::CalculateArea();
						TilePanel::CalculateArea();
						// determines new tile, area, and grid size
						CalculateArea();
						// repositions the control prompt
						Control::CalculateArea();
					} break;
				}
			}
			// handle the control box events
			else {
				Control::Handle();
				if (Control::GetState() != Control::STATE_NONE) {
					// return control if OK or CLOSED
					Cursor::SetCursor(cursor); // return the cursor state
					SDL_StopTextInput(); // turn off text input handling
					state = STATE_NONE;

					// handle the control prompt being submitted by the user
					if (Control::GetState() == Control::STATE_OK) {
						if (Control::GetType() == Control::TYPE_NEW) { New_Underlying(); } // create the new level
						else if (Control::GetType() == Control::TYPE_RESIZE) {
							Resize_Underlying(Control::GetWidth(), Control::GetHeight(), Control::GetDir(), true); // resize the existing level
						}
					}
				}
			}
		}
	}

	void Update ()
	{
		// only happens whilst not in a prompt/control box
		if (state != STATE_PROMPT) {
			// cache for easy access and speed
			int areax = GetXOffsetPos(), areay = GetYOffsetPos();
			// @IMPROVE: CAN BE CACHED - IT IS USED IN BOTH RENDER AND UPDATE
			// calculate a custom boundary because the area can be behind the tilepanel or hotbar
			int boundsx = areax, boundsy = areay, boundsw = area.w, boundsh = area.h;
			if (boundsx < TilePanel::WIDTH) { boundsx = TilePanel::WIDTH, boundsw = area.w - (TilePanel::WIDTH - areax); }
			if (boundsy < Hotbar::HEIGHT) { boundsy = Hotbar::HEIGHT, boundsh = area.h - (Hotbar::HEIGHT - areay); }

			// handles unsetting and resetting the cursor on entance/exit
			if (!Cursor::Within(boundsx, boundsy, boundsw, boundsh) && !cursorunset) { Cursor::SetCursor(Cursor::CURSOR_ARROW); cursorunset = true; }
			else if (Cursor::Within(boundsx, boundsy, boundsw, boundsh) && cursorunset) { Cursor::SetCursor(cursor); cursorunset = false; }

			// handles panning via middle-mouse button
			if (panning) { xoffset += ((float)relativecurx / (float)tilesize), yoffset += ((float)relativecury / (float)tilesize); }
			// handles panning via the arrow keys
			if (panningdir[PANNING_UP])    { yoffset += CAMERA_PAN_SPEED; }
			if (panningdir[PANNING_RIGHT]) { xoffset -= CAMERA_PAN_SPEED; }
			if (panningdir[PANNING_DOWN])  { yoffset -= CAMERA_PAN_SPEED; }
			if (panningdir[PANNING_LEFT])  { xoffset += CAMERA_PAN_SPEED; }

			// handles the actions of the various tools
			if (state != STATE_NONE) {
				// no point handling state and tools if we're not in the level editor area
				if (Cursor::Within(boundsx, boundsy, boundsw, boundsh)) {
					// handle the different states and tools...
					switch (tool) {
						case (TOOL_PENCIL): { Pencil(); } break;
						case (TOOL_FILL):   { Fill(); } break;
						case (TOOL_SELECT): { if (state == STATE_DRAW) { Select(); } } break;
					}
				}
			}

			// resets these cached values to ensure they don't carry through
			relativecurx = 0, relativecury = 0;
		}
	}

	void Render ()
	{
		// stores the starting time of the render
		#ifndef RELEASE
		std::uint64_t begincyclecount = __rdtsc();
		LARGE_INTEGER begincounter;
		QueryPerformanceCounter(&begincounter);
		#endif

		// cache for easy access and speed
		int areax = GetXOffsetPos(), areay = GetYOffsetPos();

		// @IMPROVE: CAN BE CACHED - IT IS USED IN BOTH RENDER AND UPDATE
		// calculate a custom boundary because the area can be behind the tilepanel or hotbar
		int boundsx = areax, boundsy = areay, boundsw = area.w, boundsh = area.h;
		if (boundsx < TilePanel::WIDTH) { boundsx = TilePanel::WIDTH, boundsw = area.w - (TilePanel::WIDTH - areax); }
		if (boundsy < Hotbar::HEIGHT) { boundsy = Hotbar::HEIGHT, boundsh = area.h - (Hotbar::HEIGHT - areay); }

		// renders the level editor area
		GPU_RectangleFilled(App::screen, boundsx, boundsy, boundsx + boundsw, boundsy + boundsh, leveleditor_background_color);

		// data for rendering the actual tile data
		int activelayer = TilePanel::GetActiveLayer();
		SDL_Color layercolor = Render::Color::WHITE;
		GPU_Rect tilebounds = { 0, 0, TilePanel::TILE_SPR_SIZE, TilePanel::TILE_SPR_SIZE };
		float tilescale = (float)tilesize / TilePanel::TILE_SPR_SIZE;
		float tileareaw = std::ceil(((float)(App::GetWidth() - TilePanel::WIDTH) / (float)tilesize) - xoffset);
		float tileareah = std::ceil(((float)(App::GetHeight() - Hotbar::HEIGHT) / (float)tilesize) - yoffset);

		// determine the culling bounds for what tiles to render (more performant)
		SDL_Rect cull = {
			((xoffset > 0) ? 0 : (int)std::abs(xoffset)),
			((yoffset > 0) ? 0 : (int)std::abs(yoffset)),
			((tileareaw < levelwidth) ? (int)tileareaw : (int)levelwidth),
			((tileareah < levelheight) ? (int)tileareah : (int)levelheight)
		};

		// @IMPROVE: THIS COULD BE CONVERTED INTO A FUNCTION THAT TAKES IN A TILEDATA SET AND A RANGE
		// OF VALUES (X,Y,W,H) AND DRAWS ALL THE TILES IN THAT SET WITHIN RANGE
		// goes through and renders each of the tile layers in order
		for (int i = LVL::LAYER_TOTAL-1; i >= 0; --i) {
			// if a layer is inactive then there is no reason to bother rendering
			if (islayeractive.at(i)) {
				// determines the alpha for the layer based on current active layer
				layercolor.a = ((i >= activelayer) ? layer_active_alpha[i] : layer_inactive_alpha[i]);
				// draws the actual tiledata tile-by-tile
				auto& layer = tiledata.at(i);
				for (int iy = cull.y; iy < cull.h; ++iy) { // note: treating cull.h as a cull.y2
					for (int ix = cull.x; ix < cull.w; ++ix) { // note: treating cull.w as a cull.x2
						std::uint32_t tile = layer.at(iy * levelwidth + ix);
						// do not bother rendering empty tiles (waste of time)
						if (tile != 0) {
							tilebounds.x = TILE_LOOKUP[tile].x;
							tilebounds.y = TILE_LOOKUP[tile].y;
							Render::DrawTexture(
								(ix*tilesize)+areax, (iy*tilesize)+areay,
								0.0f, 0.0f,
								tilescale, tilescale,
								Asset::tex_tiles, &tilebounds,
								layercolor
							);
						}
					}
				}
			}
		}

		// @IMPROVE: THIS COULD BE CONVERTED INTO A FUNCTION THAT TAKES IN A TILEDATA SET AND A RANGE
		// OF VALUES (X,Y,W,H) AND DRAWS ALL THE TILES IN THAT SET WITHIN RANGE
		// there is no point in attempting to render the ghosted tile/clipboard content if not in bounds
		if (Cursor::Within(boundsx, boundsy, boundsw, boundsh)) {
			// if the CTRL key is being held draw the ghosted clipboard content
			if (selectcursor) {
				// detemrine if the select buffer is completely empty
				bool selectempty = true;
				for (auto& layer: selectbuffer) { if (!layer.empty()) { selectempty = false; break; } }
				// if the select buffer is empty then there is no need to continue (nothing to render)
				if (!selectempty) {
					// locate the tile position on the level editor where the cursor is
					int posx, posy;
					GetTilePos(posx, posy);
					(posx *= tilesize) += GetXOffsetPos(), (posy *= tilesize) += GetYOffsetPos();
					// render the ghosted clipboard tile textures + the tinted rect to make it stand out
					GPU_RectangleFilled(App::screen, posx, posy, posx+(selectwidth*tilesize), posy+(selectheight*tilesize), cursor_tile_color);
					for (int i = LVL::LAYER_TOTAL-1; i >= 0; --i) {
						// don't bother rendering inactive layers
						if (islayeractive.at(i)) {
							auto& layer = selectbuffer.at(i);
							for (int iy = 0; iy < selectheight; ++iy) {
								for (int ix = 0; ix < selectwidth; ++ix) {
									std::uint32_t tile = layer.at(iy * selectwidth + ix);
									// do not bother rendering empty tiles (waste of time)
									if (tile != 0) {
										tilebounds.x = TILE_LOOKUP[tile].x;
										tilebounds.y = TILE_LOOKUP[tile].y;
										Render::DrawTexture(
											(ix*tilesize)+posx, (iy*tilesize)+posy,
											0.0f, 0.0f,
											tilescale, tilescale,
											Asset::tex_tiles, &tilebounds,
											SDL_Color { 255, 255, 255, cursor_tile_color.a }
										);
									}
								}
							}
						}
					}
				}
			}
			// handle rendering the standard singular ghosted tile
			else {
				// there is no need to render if the select tool is in use
				if (tool != TOOL_SELECT) {
					// locate the tile position on the level editor where the cursor is
					int posx, posy;
					GetTilePos(posx, posy);
					(posx *= tilesize) += GetXOffsetPos(), (posy *= tilesize) += GetYOffsetPos();
					// figure out what tile/sprite should be drawn
					int activeid = TilePanel::GetActiveID();
					GPU_Rect bounds { TILE_LOOKUP[activeid].x, TILE_LOOKUP[activeid].y, TilePanel::TILE_SPR_SIZE, TilePanel::TILE_SPR_SIZE };
					// render the ghosted tile texture + the tinted rect to make it stand out
					GPU_RectangleFilled(App::screen, posx, posy, posx+tilesize, posy+tilesize, cursor_tile_color);
					if (!TilePanel::AllLayersAreInactive()) {
						Render::DrawTexture(posx, posy, 0.f, 0.f, tilescale, tilescale, Asset::tex_tiles, &bounds, SDL_Color { 255, 255, 255, cursor_tile_color.a });
					}
				}
			}
		}

		// fixes visual problems with the selection box based on certain mouse positions
		if (selectvisible) {
			GPU_Rect selectrender = { selectbox.x, selectbox.y, selectbox.w, selectbox.h };
			if (selectrender.x >= selectrender.w) { selectrender.x++; selectrender.w--; }
			if (selectrender.y >= selectrender.h) { selectrender.y++; selectrender.h--; }
			// renders the current selection box to the screen
			GPU_RectangleFilled(
				App::screen,
				areax + (selectrender.x * tilesize), areay + (selectrender.y * tilesize),
				areax + (selectrender.w * tilesize), areay + (selectrender.h * tilesize),
				selectbox_color
			);
		}

		// renders the camera bounds
		if (cameravisible) {
			// setup for camera rendering
			int cameracount = 0;
			SDL_Color color = camerabounds_valid_color;
			GPU_Rect camera = {
				(float)areax,
				(float)areay,
				areax + ((float)levelwidth * tilesize),
				areay + ((float)levelheight * tilesize)
			};

			// gets the first and last camera bounds tiles, to form the rectangle bounds
			auto& layer = tiledata.at(LVL::LAYER_TAG);
			std::size_t ix = 0, iy = 0;
			for (auto& tile: layer) {
				if (tile == CAMERA_TILE_ID) {
					if (cameracount == 0) { camera.x = areax + std::floor(ix * tilesize), camera.y = areay + std::floor(iy * tilesize); }
					else { camera.w = (areax + std::floor(ix * tilesize)) + tilesize, camera.h = (areay + std::floor(iy * tilesize)) + tilesize; }
					++cameracount;
				}
				// move down each row
				if (++ix >= levelwidth) { ix = 0, ++iy; }
			}

			// if there is one camera tile then it will be broken, red rect to warn user
			if (cameracount == 1) { camera.x = areax, camera.y = areay, color = camerabounds_invalid_color; }
			// ensures that the rectangle always renders correctly
			if (camera.x >= camera.w) { camera.x += tilesize, camera.w -= tilesize; }

			// renders the camera bounds area
			GPU_RectangleFilled(App::screen, camera.x, camera.y, camera.w, camera.h, color);
		}

		// renders the tile grid on top of the level editor - if it's active
		if (gridvisible) {
			// calculate the min and max bounds for line drawing
			GPU_Rect grid;
			grid.x = boundsx - ((boundsx == TilePanel::WIDTH) ? (tilesize - (boundsw%tilesize)) : 0);
			grid.y = boundsy - ((boundsy == Hotbar::HEIGHT) ? (tilesize - (boundsh%tilesize)) : 0);
			grid.w = boundsx + boundsw;
			grid.h = boundsy + boundsh;
			// draw all the vertical and horizontal grid lines across the map
			for (float ix = grid.x; ix <= grid.w; ix += tilesize) { GPU_Line(App::screen, ix, grid.y, ix, grid.h, leveleditor_grid_color); }
			for (float iy = grid.y; iy <= grid.h; iy += tilesize) { GPU_Line(App::screen, grid.x, iy, grid.w, iy, leveleditor_grid_color); }
		}

		// renders appropriate lines of symmetry/mirroring
		if (mirrorhorz) {
			int xpos = areax + ((levelwidth * tilesize) / 2);
			GPU_RectangleFilled(App::screen, xpos-1, areay, xpos+1, areay+(levelheight * tilesize), symmetry_lines_color); }
		if (mirrorvert) {
			int ypos = areay + ((levelheight * tilesize) / 2);
			GPU_RectangleFilled(App::screen, areax, ypos-1, areax+(levelwidth * tilesize), ypos+1, symmetry_lines_color); }

		// calculates how long the render frame took
		#if 0
		#ifndef RELEASE
		LARGE_INTEGER endcounter;
		QueryPerformanceCounter(&endcounter);
		std::uint64_t elapsedcycles = __rdtsc() - begincyclecount;
		std::int64_t elapsedcount = endcounter.QuadPart - begincounter.QuadPart;
		double mspf = (double)((1000.0 * (double)elapsedcount) / (double)perfcountfreq);
		double fps = (double)((double)perfcountfreq / (double)elapsedcount);
		double mcpf = (double)((double)elapsedcycles / (1000.0 * 1000.0));
		printf("RENDERING: %.02fMSPF | %.02fFPS | %.02fMCPF\n", mspf, fps, mcpf);
		#endif
		#endif

		// renders the other editor sub-systems
		TilePanel::Render();
		Hotbar::Render();
		Tooltip::Render();

		// this one is different from Handle() and Update() because we still want
		// to render the level editor content, we just want to render the new level
		// control/prompt on top of that

		if (state == STATE_PROMPT) { Control::Render(); }
	}
}
