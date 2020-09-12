namespace Editor
{

namespace internal
{
    // Default parameters used when creating a blank level file.
    GLOBAL constexpr u16         DEFAULT_AREA_WIDTH         = 32;
    GLOBAL constexpr u16         DEFAULT_AREA_HEIGHT        = 32;
    GLOBAL constexpr const char* DEFAULT_AREA_NAME          = "UNTITLED";
    GLOBAL constexpr const char* DEFAULT_AREA_EFFECT        = "BASIC";
    GLOBAL constexpr const char* DEFAULT_AREA_SKYBOX        = "DBGSKY";
    GLOBAL constexpr const char* DEFAULT_AREA_SKYBOX_EFFECT = "BASIC";

    GLOBAL constexpr float DEFAULT_TILE_SIZE = 20.0f; // The size to draw tiles in the editor when at zoom 100%.
    GLOBAL constexpr float TILE_IMAGE_SIZE = 128.0f;

    GLOBAL constexpr float MIN_EDITOR_ZOOM        = 0.25f;
    GLOBAL constexpr float MAX_EDITOR_ZOOM        = 4.00f;
    GLOBAL constexpr float EDITOR_ZOOM_INCREMENT  = 0.10f;

    GLOBAL const char* EDITOR_REGULAR_FONT = "LIBMONOR";
    GLOBAL const char* EDITOR_BOLD_FONT    = "LIBMONOB";

    GLOBAL const Vec4 UI_COLOR_BLACK     = { 0.00f, 0.00f, 0.00f, 1.0f };
    GLOBAL const Vec4 UI_COLOR_EX_DARK   = { 0.20f, 0.20f, 0.20f, 1.0f };
    GLOBAL const Vec4 UI_COLOR_DARK      = { 0.20f, 0.20f, 0.20f, 1.0f };
    GLOBAL const Vec4 UI_COLOR_MED_DARK  = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLOBAL const Vec4 UI_COLOR_MEDIUM    = { 0.33f, 0.33f, 0.33f, 1.0f };
    GLOBAL const Vec4 UI_COLOR_MED_LIGHT = { 0.43f, 0.43f, 0.43f, 1.0f };
    GLOBAL const Vec4 UI_COLOR_LIGHT     = { 0.60f, 0.60f, 0.60f, 1.0f };
    GLOBAL const Vec4 UI_COLOR_EX_LIGHT  = { 0.90f, 0.90f, 0.90f, 1.0f };
    GLOBAL const Vec4 UI_COLOR_WHITE     = { 1.00f, 1.00f, 1.00f, 1.0f };

    GLOBAL bool editor_initialized;
    GLOBAL Catalog editor_catalog;

    GLOBAL constexpr float CONTROL_PANEL_WIDTH = 274.0f;
    GLOBAL constexpr float CONTROL_PANEL_SCROLLBAR_WIDTH = 12.0f;
    GLOBAL constexpr float CONTROL_PANEL_INNER_PAD = 3.0f;

    GLOBAL constexpr float TOOL_PANEL_WIDTH = 30.0f;
    GLOBAL constexpr float TOOL_PANEL_INNER_PAD = 1.0f;

    GLOBAL constexpr float PALETTE_PANEL_INNER_PAD = 6.0f;
    GLOBAL constexpr float PALETTE_PANEL_ITEM_SIZE = 48.0f;
    GLOBAL constexpr float PALETTE_PANEL_LABEL_W = 58.0f;
    GLOBAL constexpr float PALETTE_PANEL_LABEL_H = 24.0f;

    GLOBAL constexpr float LAYER_PANEL_INNER_PAD = 3.0f;
    GLOBAL constexpr float LAYER_PANEL_BUTTON_H = 24.0f;

    GLOBAL constexpr float INFORMATION_PANEL_LABEL_WIDTH = 0.125f; // NDC!

    GLOBAL constexpr float INFORMATION_PANEL_HEIGHT = 24.0f;
    GLOBAL constexpr float INFORMATION_PANEL_INNER_PAD = 6.0f;

    GLOBAL constexpr float PROPERTIES_PANEL_INNER_PAD = 6.0f;
    GLOBAL constexpr float PROPERTIES_PANEL_ITEM_HEIGHT = 24.0f;

    GLOBAL constexpr size_t ITEMS_PER_ROW = 4;

    GLOBAL float control_panel_width;

    GLOBAL float layer_panel_content_height;
    GLOBAL float layer_panel_panel_height;
    GLOBAL float layer_panel_scroll_offset;

    GLOBAL float tile_palette_panel_content_height;
    GLOBAL float tile_palette_panel_scroll_offset;
    GLOBAL float tile_palette_panel_panel_height;

    GLOBAL float thing_palette_panel_content_height;
    GLOBAL float thing_palette_panel_scroll_offset;
    GLOBAL float thing_palette_panel_panel_height;

    typedef u32 UI_ID;

    GLOBAL constexpr UI_ID UI_INVALID_ID = UINT32_MAX;

    // The function type that is called by various UI widgets when triggered.
    typedef void (*UI_Action)(void);

    typedef u32 UI_Flag;

    GLOBAL constexpr UI_Flag UI_NONE       = 0x00000000;
    GLOBAL constexpr UI_Flag UI_LOCKED     = 0x00000001;
    GLOBAL constexpr UI_Flag UI_INACTIVE   = 0x00000002;
    GLOBAL constexpr UI_Flag UI_HIGHLIGHT  = 0x00000004;
    // Flags for what type of characters can be written into a text box.
    GLOBAL constexpr UI_Flag UI_ALPHABETIC = 0x00000010;
    GLOBAL constexpr UI_Flag UI_NUMERIC    = 0x00000020;
    GLOBAL constexpr UI_Flag UI_ALPHANUM   = 0x00000040;

    enum UI_Align
    {
        UI_ALIGN_CENTER,
        UI_ALIGN_LEFT,
        UI_ALIGN_RIGHT,
        UI_ALIGN_TOP,
        UI_ALIGN_BOTTOM
    };

    enum UI_Dir
    {
        UI_DIR_UP,
        UI_DIR_RIGHT,
        UI_DIR_DOWN,
        UI_DIR_LEFT
    };

    struct Panel
    {
        Quad absolute_bounds; // Panel position and size on the window.
        Quad viewport;        // Viewport clipped inside parent.
        Vec2 relative_offset; // Panel position relative to its viewport.

        UI_Flag flags;        // Flags that get applied to a panel's content.

        Vec2*  cursor;
        UI_Dir cursor_dir;

        bool cursor_advance_enabled;
    };

    // Highly unlikely we will ever need more embedded scissors than this but if we
    // do we will know about it through various code ASSERTS and can just increase.
    GLOBAL Stack<Panel, 256> ui_panels;

    GLOBAL UI_ID ui_cur_id;
    GLOBAL UI_ID ui_hot_id;
    GLOBAL UI_ID ui_hit_id;

    GLOBAL Vec2 ui_mouse_relative;

    GLOBAL constexpr float UI_CURSOR_BLINK_INTERVAL = 0.5f;
    GLOBAL float ui_cursor_blink_timer;

    GLOBAL UI_ID  ui_active_text_box = UI_INVALID_ID;
    GLOBAL UI_ID  ui_hot_text_box = UI_INVALID_ID;
    GLOBAL size_t ui_text_box_cursor;

    GLOBAL bool ui_cursor_visible;

    // We also store the active window ID at the time of the tab press so that the
    // system does not move from window-to-window when the user presseses tab.
    GLOBAL Window_ID ui_text_box_tab_window_id;
    GLOBAL bool      ui_make_next_text_box_active;
    GLOBAL bool      ui_tab_handled;
    GLOBAL bool      ui_text_event_happened;

    enum UI_Text_Event_Type: u32
    {
        UI_TEXT_EVENT_TEXT,
        UI_TEXT_EVENT_KEY,
    };

    struct UI_Text_Event
    {
        UI_Text_Event_Type type;

        char       text;
        SDL_Keycode key;
    };

    GLOBAL std::vector<UI_Text_Event> ui_text_events;

    // The term item will be used throughout the editor to refer to both tiles+things.
    // Note: We could have used either Tile or Thing ID/Flags for the typedefing here.
    typedef Tile_ID    Item_ID;
    typedef Tile_Flags Item_Flags;

    // Throughout the editor's code we use a -1 value to refer to the thing layer.
    GLOBAL constexpr int THING_LAYER = -1;

    enum Tool_State { TOOL_STATE_IDLE, TOOL_STATE_PLACE, TOOL_STATE_ERASE };
    enum Tool_Type  { TOOL_TYPE_BRUSH, TOOL_TYPE_FILL, TOOL_TYPE_SELECT };

    struct Tool_Fill
    {
        std::vector<Vec2> frontier;
        std::vector<bool> searched;

        int layer;

        Item_ID find_id;
        Item_ID replace_id;

        Vec2 start;

        bool inside_select;
    };

    struct Select_Bounds
    {
        int top;
        int right;
        int bottom;
        int left;

        bool visible;
    };

    struct Tool_Select
    {
        std::vector<Select_Bounds> bounds;

        bool start;
        bool add;

        size_t cached_size;
    };

    struct Tool_Info
    {
        Tool_Fill fill;
        Tool_Select select;
    };

    struct Clipboard
    {
        std::array<std::vector<Tile>, Area::TILE_LAYER_TOTAL> tiles;

        int x;
        int y;
        int w;
        int h;
    };

    enum History_Action
    {
        HISTORY_ACTION_TILE,
    };

    struct History_Tile_Info
    {
        int x;
        int y;

        Tile_ID old_id;
        Tile_ID new_id;

        int layer;
    };

    struct History_State
    {
        History_Action action;

        std::vector<History_Tile_Info> tile_info;
    };

    struct History
    {
        int current_position;
        std::vector<History_State> state;
    };

    GLOBAL History history;

    GLOBAL std::vector<Clipboard> clipboard;

    GLOBAL Tool_State tool_state = TOOL_STATE_IDLE;
    GLOBAL Tool_Type  tool_type  = TOOL_TYPE_BRUSH;
    GLOBAL Tool_Info  tool_info;

    GLOBAL Vec2 level_editor_mouse_world;
    GLOBAL Vec2 level_editor_mouse;
    GLOBAL Vec2 level_editor_mouse_tile;

    GLOBAL Quad level_editor_bounds;
    GLOBAL Quad level_editor_viewport;

    GLOBAL Vec2 camera_pos;
    GLOBAL float camera_zoom;

    GLOBAL Area current_area;

    constexpr GLOBAL Tile_ID NO_TILE_SELECTED = CAST(Tile_ID, -1);
    constexpr GLOBAL Thing_ID NO_THING_SELECTED = CAST(Thing_ID, -1);

    GLOBAL Tile_ID selected_tile;
    GLOBAL Thing_ID selected_thing;

    GLOBAL bool tile_layer_active[Area::TILE_LAYER_TOTAL];
    GLOBAL bool thing_layer_active;

    GLOBAL bool unsaved_changes;
    GLOBAL bool grid_visible;

    enum Editor_Panel
    {
        EDITOR_PANEL_LEVEL,
        EDITOR_PANEL_TILES,
        EDITOR_PANEL_THINGS,
    };

    enum Editor_View
    {
        EDITOR_VIEW_2D,
        EDITOR_VIEW_3D,
    };

    GLOBAL Editor_Panel current_editor_panel;
    GLOBAL Editor_View current_editor_view;

    #if defined(PLATFORM_WINNT) ////////////////////////////////////////////////
    FILDEF HWND get_window_handle (SDL_Window* _window)
    {
        SDL_SysWMinfo win_info;
        SDL_zero(win_info);

        SDL_VERSION(&win_info.version);

        if (!SDL_GetWindowWMInfo(_window, &win_info)) { return NULL; }
        else { return win_info.info.win.window; }
    }

    STDDEF std::string open_dialog ()
    {
        const char* filter = "All Files (*.*)\0*.*\0LVL Files (*.lvl)\0*.lvl\0";

        constexpr size_t BUFFER_SIZE = UINT16_MAX+1;
        char file_buffer[BUFFER_SIZE] = {};

        OPENFILENAMEA open_file_name = {};

        open_file_name.hwndOwner       = get_window_handle(get_window("WINMAIN").window);
        open_file_name.lStructSize     = sizeof(OPENFILENAMEA);
        open_file_name.nFilterIndex    = 2;
        open_file_name.lpstrFilter     = filter;
        open_file_name.lpstrFile       = file_buffer;
        open_file_name.nMaxFile        = BUFFER_SIZE;
        open_file_name.lpstrInitialDir = area_catalog.build_path("").c_str();
        open_file_name.lpstrDefExt     = "lvl";
        open_file_name.Flags           = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_EXPLORER;

        // Attempt to load the open dialog and handle any potential errors.
        std::string file;
        if (!GetOpenFileNameA(&open_file_name)) {
            DWORD error = CommDlgExtendedError();
            if (error) { // Zero means the user cancelled -- not an actual error!
                LOG_ERROR(ERR_MED, "Failed to open file! (Error: 0x%X)", error);
            }
        } else {
            file = fix_path_slashes(file_buffer);
        }

        return file;
    }

    STDDEF std::string save_dialog ()
    {
        const char* filter = "All Files (*.*)\0*.*\0LVL Files (*.lvl)\0*.lvl\0";

        constexpr size_t BUFFER_SIZE = MAX_PATH+1;
        char file_buffer[BUFFER_SIZE] = {};

        OPENFILENAMEA open_file_name = {};

        open_file_name.hwndOwner       = get_window_handle(get_window("WINMAIN").window);
        open_file_name.lStructSize     = sizeof(OPENFILENAMEA);
        open_file_name.nFilterIndex    = 2;
        open_file_name.lpstrFilter     = filter;
        open_file_name.lpstrFile       = file_buffer;
        open_file_name.nMaxFile        = BUFFER_SIZE;
        open_file_name.lpstrInitialDir = area_catalog.build_path("").c_str();
        open_file_name.lpstrDefExt     = "lvl";
        open_file_name.Flags           = OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT;

        // Attempt to load the save dialog and handle any potential errors.
        std::string file;
        if (!GetSaveFileNameA(&open_file_name)) {
            DWORD error = CommDlgExtendedError();
            if (error) { // Zero means the user cancelled -- not an actual error!
                LOG_ERROR(ERR_MED, "Failed to save file! (Error: 0x%X)", error);
            }
        } else {
            file = fix_path_slashes(file_buffer);
        }

        return file;
    }
    #endif // PLATFORM_WINNT ///////////////////////////////////////////////////

    FILDEF void reset_ui_state ()
    {
        // We do this during every event otherwise we can end up with some weird
        // values provided by SDL_GetRelativeMouseState, so we just cache here.
        int imx, imy; SDL_GetRelativeMouseState(&imx, &imy);
        ui_mouse_relative = { CAST(float, imx), CAST(float, imy) };

        ui_cur_id = 0;

        ui_text_events.clear();
        ui_text_event_happened = false;
        ui_tab_handled = false;
    }

    FILDEF void handle_ui_events ()
    {
        // Handle events specifically for UI text boxes.
        if (ui_active_text_box != UI_INVALID_ID) {
            UI_Text_Event text_event;
            switch (main_event.type) {
            case (SDL_TEXTINPUT): {
                text_event.type = UI_TEXT_EVENT_TEXT;
                text_event.text = main_event.text.text[0];
                ui_text_events.push_back(text_event);
                // printf("Added text input event!\n");
            } break;
            case (SDL_KEYDOWN): {
                text_event.type = UI_TEXT_EVENT_KEY;
                text_event.key  = main_event.key.keysym.sym;
                ui_text_events.push_back(text_event);
                // printf("Added key down event!\n");
            } break;
            }
        } else {
            if (main_event.type == SDL_KEYDOWN) {
                if (main_event.key.keysym.sym == SDLK_TAB) { // We do this so we can focus on the first text box in the window!
                    ui_make_next_text_box_active = true;
                    ui_text_box_tab_window_id = get_focused_window().id;

                    ui_cursor_blink_timer = UI_CURSOR_BLINK_INTERVAL;
                    ui_cursor_visible = true;
                }
            }
        }
    }

    FILDEF void advance_ui_cursor_start (Panel& _panel, float _w, float _h)
    {
        if (!_panel.cursor_advance_enabled) { return; }

        ASSERT(_panel.cursor);
        Vec2& cur = *_panel.cursor;

        switch (_panel.cursor_dir) {
        case(UI_DIR_UP):    { cur.y -= _h; } break;
        case(UI_DIR_LEFT):  { cur.x -= _w; } break;
        }
    }
    FILDEF void advance_ui_cursor_end (Panel& _panel, float _w, float _h)
    {
        if (!_panel.cursor_advance_enabled) { return; }

        ASSERT(_panel.cursor);
        Vec2& cur = *_panel.cursor;

        switch (_panel.cursor_dir) {
        case(UI_DIR_RIGHT): { cur.x += _w; } break;
        case(UI_DIR_DOWN):  { cur.y += _h; } break;
        }
    }

    FILDEF Quad get_clipped_bounds (float _x, float _y, float _w, float _h)
    {
        // Clip the widget's bounds to be within the panel's visible area.
        // This stops the user being able to click on invisible portions.
        //
        // We also make the bounds relative to the window, so that they can
        // properly be compared with the mouse cursor without any issues.
        Quad v = get_viewport();
        Quad clipped_bounds;

        clipped_bounds.x1 = MAX(_x,            0.0f) + v.x;
        clipped_bounds.y1 = MAX(_y,            0.0f) + v.y;
        clipped_bounds.x2 = MIN(_x + (_w-1.0f), v.w) + v.x;
        clipped_bounds.y2 = MIN(_y + (_h-1.0f), v.h) + v.y;

        return clipped_bounds;
    }

    FILDEF bool handle_widget (float _x, float _y, float _w, float _h, bool _locked)
    {
        bool result = false;

        Quad clipped_bounds = get_clipped_bounds(_x, _y, _w, _h);

        Vec2 mouse = get_mouse_pos();
        bool mouse_down = (SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK);

        // Determine the hot and active states for the global UI context.
        bool inside = point_in_bounds_xyxy(mouse, clipped_bounds);
        if (!_locked && ui_hit_id == ui_cur_id) {
            if (!mouse_down) {
                if (ui_hot_id == ui_cur_id) {
                    result = true;
                }
                ui_hit_id = UI_INVALID_ID;
            }
        }
        if (ui_hot_id == ui_cur_id) {
            if (!_locked && inside) {
                if (mouse_down) {
                    ui_hit_id = ui_cur_id;
                }
            } else {
                ui_hot_id = UI_INVALID_ID;
            }
        }
        if (inside) {
            ui_hot_id = ui_cur_id;
        }

        // If true then the widget being checked was activated this frame.
        return result;
    }

    FILDEF bool is_hot () { return (ui_cur_id == ui_hot_id); }
    FILDEF bool is_hit () { return (ui_cur_id == ui_hit_id); }

    FILDEF bool is_there_a_hot_ui_element () { return (ui_hot_id != UI_INVALID_ID); }
    FILDEF bool is_there_a_hit_ui_element () { return (ui_hit_id != UI_INVALID_ID); }

    FILDEF void draw_separator (Vec2 _cursor, UI_Dir _dir, float _w, float _h, Vec4 _color)
    {
        float x1 = _cursor.x;
        float y1 = _cursor.y;
        float x2 = _cursor.x;
        float y2 = _cursor.y;

        switch (_dir) {
        case (UI_DIR_UP):    { x1+=1;  x2+=(_w-1); y1+=1;  y2+=1;      } break;
        case (UI_DIR_RIGHT): { x1+=_w; x2+=_w;     y1+=1;  y2+=(_h-1); } break;
        case (UI_DIR_DOWN):  { x1+=1;  x2+=(_w-1); y1+=_h; y2+=_h;     } break;
        case (UI_DIR_LEFT):  { x1+=1;  x2+=1;      y1+=1;  y2+=(_h-1); } break;
        }

        set_draw_color(_color);
        draw_line(x1, y1, x2, y2);
    }

    FILDEF Vec2 get_relative_panel_cursor (Panel& _panel)
    {
        ASSERT(_panel.cursor);
        Vec2 cur = *_panel.cursor;

        cur.x += _panel.relative_offset.x;
        cur.y += _panel.relative_offset.y;

        return cur;
    }
    FILDEF Vec2 get_panel_cursor (Panel& _panel)
    {
        ASSERT(_panel.cursor);
        return *_panel.cursor;
    }

    STDDEF void begin_panel (float _x, float _y, float _w, float _h, UI_Flag _flags, Vec4 _c = Vec4())
    {
        Panel panel;

        // The method of adding a new panel varies depending on whether the panel
        // is a child to an existing panel or if it is a lone panel in the window.
        panel.absolute_bounds = { _x, _y, _w, _h };
        if (ui_panels.count > 0) {
            const Quad& p_ab = ui_panels.peek().absolute_bounds;
            const Quad& p_v  = ui_panels.peek().viewport;
            const Vec2& p_ro = ui_panels.peek().relative_offset;

            Quad& c_ab = panel.absolute_bounds;
            Quad& c_v  = panel.viewport;
            Vec2& c_ro = panel.relative_offset;

            c_ab.x += p_ab.x + p_ro.x;
            c_ab.y += p_ab.y + p_ro.y;

            c_v = c_ab;

            // We also clip the panel's viewport to be inside of the
            // parent panel to avoid issues with overlapping/spill.
            float dx = c_v.x - p_v.x;
            float dy = c_v.y - p_v.y;

            if (c_v.x < p_v.x) { c_v.x = p_v.x; c_v.w -= roundf(abs(dx)); dx = c_v.x - p_v.x; }
            if (c_v.y < p_v.y) { c_v.y = p_v.y; c_v.h -= roundf(abs(dy)); dy = c_v.y - p_v.y; }

            if (c_v.x+c_v.w > p_v.x+p_v.w) { c_v.w = p_v.w - roundf(abs(dx)); }
            if (c_v.y+c_v.h > p_v.y+p_v.h) { c_v.h = p_v.h - roundf(abs(dy)); }

            if (c_v.w < 0.0f) { c_v.w = 0.0f; }
            if (c_v.h < 0.0f) { c_v.h = 0.0f; }

            // And determine the panel's offset to its viewport.
            c_ro.x = c_ab.x - c_v.x;
            c_ro.y = c_ab.y - c_v.y;

            // Inherit the parent panel's flags.
            panel.flags = _flags | ui_panels.peek().flags;
        } else {
            panel.viewport = panel.absolute_bounds;
            panel.relative_offset = { 0.0f, 0.0f };
            panel.flags = _flags;
        }

        panel.cursor = NULL;
        panel.cursor_dir = UI_DIR_RIGHT;
        panel.cursor_advance_enabled = true;

        set_viewport(panel.viewport);
        ui_panels.push(panel);

        set_draw_color(_c);
        fill_quad(0.0f, 0.0f, panel.viewport.w, panel.viewport.h);
    }
    STDDEF bool begin_click_panel (UI_Action _action, float _w, float _h, UI_Flag _flags)
    {
        Panel& parent = ui_panels.peek();

        Vec2 rcur = get_relative_panel_cursor(parent);
        Vec2 cur = get_panel_cursor(parent);

        // Cache the panel's flags so they are easily accessible.
        bool locked    = (_flags & UI_LOCKED);
        bool highlight = (_flags & UI_HIGHLIGHT);

        bool result = handle_widget(rcur.x, rcur.y, _w, _h, locked);
        if (result && _action) { _action(); } // Make sure action is valid!

        Vec4 back = UI_COLOR_MEDIUM;

        if      (locked)   { back = UI_COLOR_MED_DARK;  }
        else if (is_hit()) { back = UI_COLOR_DARK;      }
        else if (is_hot()) { back = UI_COLOR_MED_LIGHT; }

        begin_panel(cur.x, cur.y, _w, _h, _flags, back);
        advance_ui_cursor_start(parent, _w, _h);

        if (highlight && !is_hit()) {
            Vec4 color = UI_COLOR_MED_LIGHT;
            color.a = 0.66f;
            set_draw_color(color);
            fill_quad(0.0f, 0.0f, get_viewport().w, get_viewport().h);
        }

        Vec4 separator_color = (locked) ? UI_COLOR_DARK : UI_COLOR_MED_DARK;
        Vec2 cursor = ui_panels.peek().relative_offset;

        draw_separator(cursor, parent.cursor_dir, _w, _h, separator_color);
        advance_ui_cursor_end(parent, _w, _h);

        ++ui_cur_id;

        return result;
    }
    STDDEF void end_panel ()
    {
        ui_panels.pop();

        // We either go back to a previous nested panel or this is the last panel
        // and we go back to placing things relative to the entire program window.
        if (ui_panels.count > 0) { set_viewport(ui_panels.peek().viewport); }
        else { set_viewport(0, 0, get_render_target_w(), get_render_target_h()); }
    }

    FILDEF float get_panel_w () { return ui_panels.peek().absolute_bounds.w; }
    FILDEF float get_panel_h () { return ui_panels.peek().absolute_bounds.h; }

    FILDEF void set_panel_cursor (Vec2* _cursor) { ui_panels.peek().cursor = _cursor; }
    FILDEF void set_panel_cursor_dir (UI_Dir _dir) { ui_panels.peek().cursor_dir = _dir; }

    FILDEF void advance_panel_cursor (float _advance)
    {
        advance_ui_cursor_start(ui_panels.peek(), _advance, _advance);
        advance_ui_cursor_end(ui_panels.peek(), _advance, _advance);
    }

    FILDEF Vec2 get_panel_offset ()
    {
        return ui_panels.peek().relative_offset;
    }

    STDDEF bool do_button_img (UI_Action _action, float _w, float _h, UI_Flag _flags, std::string _img)
    {
        _flags |= ui_panels.peek().flags;

        // Cache the button's flags so they are easily accessible.
        bool inactive  = (_flags & UI_INACTIVE);
        bool locked    = (_flags & UI_LOCKED);
        bool highlight = (_flags & UI_HIGHLIGHT);

        advance_ui_cursor_start(ui_panels.peek(), _w, _h);

        Vec2 cur = get_relative_panel_cursor(ui_panels.peek());

        // We scissor the contents to avoid image overspill.
        begin_scissor(cur.x, cur.y, _w, _h);
        defer { end_scissor(); };

        // Locked buttons cannot be interacted with.
        bool result = handle_widget(cur.x, cur.y, _w, _h, locked);
        if (result && _action) { _action(); } // Make sure action is valid!

        Vec4 front  = Vec4(0.73f,0.73f,0.73f,1.0f);
        Vec4 back   = UI_COLOR_MEDIUM;
        Vec4 shadow = Vec4(0.16f,0.16f,0.16f,1.0f);

        if      (locked)   { back = UI_COLOR_MED_DARK;  }
        else if (is_hit()) { back = UI_COLOR_DARK;      }
        else if (is_hot()) { back = UI_COLOR_MED_LIGHT; }

        if (locked || inactive) {
            shadow.a = 0.5f;
            front.a = 0.5f;
        }

        set_draw_color(back); // Draw the button's background quad.
        fill_quad(cur.x, cur.y, cur.x + _w, cur.y + _h);

        if (highlight && !is_hit()) {
            Vec4 color = UI_COLOR_MED_LIGHT;
            color.a = 0.66f;
            set_draw_color(color);
            fill_quad(0.0f, 0.0f, get_viewport().w, get_viewport().h);
        }

        // The ((_w)-1.0f) and ((_h)-1.0f) are used to ensure the separator
        // does not mess with the centering of the image based on direction.

        UI_Dir dir = ui_panels.peek().cursor_dir;

        Texture& tex = editor_catalog.GET_TEXTURE_RESOURCE(_img);

        // Scale the image to fit inside the button.
        float scale = ((_h)-1.0f) / tex.h;
        set_texture_draw_scale(scale, scale);

        float w = (dir == UI_DIR_RIGHT || dir == UI_DIR_LEFT) ? ((_w)-1.0f) : (_w);
        float h = (dir == UI_DIR_UP    || dir == UI_DIR_DOWN) ? ((_h)-1.0f) : (_h);

        // Center the image within the button.
        float x = roundf(cur.x + (w / 2.0f) + ((dir == UI_DIR_LEFT) ? 1.0f : 0.0f));
        float y = roundf(cur.y + (h / 2.0f) + ((dir == UI_DIR_UP)   ? 1.0f : 0.0f));

        float offset = 1.0f;

        tex.color = shadow;
        draw_texture(tex, x, y-offset);
        tex.color = front;
        draw_texture(tex, x, y);

        draw_separator(get_relative_panel_cursor(ui_panels.peek()), ui_panels.peek().cursor_dir, _w, _h, UI_COLOR_MED_DARK);
        advance_ui_cursor_end(ui_panels.peek(), _w, _h);

        ++ui_cur_id;

        return result;
    }

    STDDEF bool do_button_txt (UI_Action _action, float _w, float _h, UI_Flag _flags, const char* _text, std::string _fnt, bool _use_large = false)
    {
        _flags |= ui_panels.peek().flags;

        // Cache the button's flags so they are easily accessible.
        bool inactive  = (_flags & UI_INACTIVE);
        bool locked    = (_flags & UI_LOCKED);
        bool highlight = (_flags & UI_HIGHLIGHT);

        advance_ui_cursor_start(ui_panels.peek(), _w, _h);

        Vec2 cur = get_relative_panel_cursor(ui_panels.peek());

        // We scissor the contents to avoid text overspill.
        begin_scissor(cur.x, cur.y, _w, _h);
        defer { end_scissor(); };

        // Locked buttons cannot be interacted with.
        bool result = handle_widget(cur.x, cur.y, _w, _h, locked);
        if (result && _action) { _action(); } // Make sure action is valid!

        Vec4 front  = UI_COLOR_EX_LIGHT;
        Vec4 back   = UI_COLOR_MEDIUM;
        Vec4 shadow = UI_COLOR_BLACK;

        if      (locked)   { back = UI_COLOR_MED_DARK;  }
        else if (is_hit()) { back = UI_COLOR_DARK;      }
        else if (is_hot()) { back = UI_COLOR_MED_LIGHT; }

        if (locked || inactive) {
            shadow.a = 0.5f;
            front.a = 0.5f;
        }

        set_draw_color(back); // Draw the button's background quad.
        fill_quad(cur.x, cur.y, cur.x + _w, cur.y + _h);

        if (highlight && !is_hit()) {
            Vec4 color = UI_COLOR_MED_LIGHT;
            color.a = 0.66f;
            set_draw_color(color);
            fill_quad(0.0f, 0.0f, get_viewport().w, get_viewport().h);
        }

        UI_Dir dir = ui_panels.peek().cursor_dir;

        Truetype& fnt = editor_catalog.GET_TRUETYPE_RESOURCE(_fnt);

        if (_use_large) { fnt.mode = TRUETYPE_MODE_LARGE; }
        else            { fnt.mode = TRUETYPE_MODE_SMALL; }

        float w = get_text_width(fnt, _text) * get_font_draw_scale();
        float h = get_text_height(fnt, _text) * get_font_draw_scale();
        // Center the text within the button.
        float x = roundf(cur.x + ((_w - w) / 2.0f));
        float y = roundf(cur.y + ((_h / 2.0f) + (h / 4.0f)));

        float offset = 1.0f;

        set_truetype_color(fnt, shadow);
        draw_text(fnt, x, y-offset, _text);
        set_truetype_color(fnt, front);
        draw_text(fnt, x, y, _text);

        draw_separator(get_relative_panel_cursor(ui_panels.peek()), ui_panels.peek().cursor_dir, _w, _h, UI_COLOR_MED_DARK);
        advance_ui_cursor_end(ui_panels.peek(), _w, _h);

        ++ui_cur_id;

        return result;
    }

    FILDEF void do_quad (float _w, float _h, Vec4 _color)
    {
        UI_ID flags = ui_panels.peek().flags;

        advance_ui_cursor_start(ui_panels.peek(), _w, _h);

        Vec2 cur = get_relative_panel_cursor(ui_panels.peek());

        // Cache the quad's flags so they are easily accessible.
        bool inactive = (flags & UI_INACTIVE);
        bool locked = (flags & UI_LOCKED);

        if (locked || inactive) { _color.a = 0.5f; }

        set_draw_color(_color);
        fill_quad(cur.x, cur.y, cur.x+_w, cur.y+_h);

        advance_ui_cursor_end(ui_panels.peek(), _w, _h);
    }

    STDDEF void do_icon (float _w, float _h, std::string _img)
    {
        UI_ID flags = ui_panels.peek().flags;

        // Cache the image's flags so they are easily accessible.
        bool inactive = (flags & UI_INACTIVE);
        bool locked = (flags & UI_LOCKED);

        advance_ui_cursor_start(ui_panels.peek(), _w, _h);

        Vec2 cur = get_relative_panel_cursor(ui_panels.peek());

        // We scissor the contents to avoid image overspill.
        begin_scissor(cur.x, cur.y, _w, _h);
        defer { end_scissor(); };

        Vec4 front  = Vec4(0.73f, 0.73f, 0.73f, 1.0f);
        Vec4 shadow = Vec4(0.16f, 0.16f, 0.16f, 1.0f);

        if (locked || inactive) {
            shadow.a = 0.5f;
            front.a = 0.5f;
        }

        UI_Dir dir = ui_panels.peek().cursor_dir;

        Texture& tex = editor_catalog.GET_TEXTURE_RESOURCE(_img);

        // Scale the image to fit inside the button.
        Vec2 old_scale = get_texture_draw_scale();
        float scale = ((_h)-1.0f) / tex.h;
        set_texture_draw_scale(scale*old_scale.x, scale*old_scale.y);

        // Center the image within the space.
        float x = roundf(cur.x + (_w / 2.0f) + ((dir == UI_DIR_LEFT) ? 1.0f : 0.0f));
        float y = roundf(cur.y + (_h / 2.0f) + ((dir == UI_DIR_UP)   ? 1.0f : 0.0f));

        float offset = 1.0f;

        tex.color = shadow;
        draw_texture(tex, x, y-offset);
        tex.color = front;
        draw_texture(tex, x, y);

        advance_ui_cursor_end(ui_panels.peek(), _w, _h);
    }

    FILDEF void align_text (UI_Align _horz, UI_Align _vert, float& _x, float& _y, float _tw, float _th, float _w, float _h)
    {
        // Determine how to place the text based on alignment.
        switch (_horz) {
        case (UI_ALIGN_LEFT  ): { /* No need to do anything. */         } break;
        case (UI_ALIGN_RIGHT ): { _x += roundf( (_w-_tw));              } break;
        case (UI_ALIGN_CENTER): { _x += roundf(((_w-_tw)/2.0f));        } break;
        }
        switch (_vert) {
        case (UI_ALIGN_TOP   ): { _y += roundf(_th);                    } break;
        case (UI_ALIGN_BOTTOM): { _y += roundf(((_h)-(_th/4.0f)));      } break;
        case (UI_ALIGN_CENTER): { _y += roundf(((_h/2.0f)+(_th/4.0f))); } break;
        }
    }

    FILDEF bool mouse_in_ui_bounds_xywh (Quad _b)
    {
        Quad clipped_bounds = get_clipped_bounds(_b.x, _b.y, _b.w, _b.h);
        Vec2 mouse = get_mouse_pos();

        return point_in_bounds_xyxy(mouse, clipped_bounds);
    }

    STDDEF void do_label (UI_Align _horz, UI_Align _vert, float _w, float _h, const char* _text, std::string _fnt, Vec4 _bg = Vec4())
    {
        UI_Flag flags = ui_panels.peek().flags;

        // Cache the button's flags so they are easily accessible.
        bool inactive = (flags & UI_INACTIVE);
        bool locked = (flags & UI_LOCKED);

        advance_ui_cursor_start(ui_panels.peek(), _w, _h);

        Vec2 cur = get_relative_panel_cursor(ui_panels.peek());

        // We scissor the contents to avoid text overspill.
        begin_scissor(cur.x, cur.y, _w, _h);
        defer { end_scissor(); };

        // Draw the background color, by default this is transparent.
        set_draw_color(_bg);
        fill_quad(cur.x, cur.y, cur.x+_w, cur.y+_h);

        Truetype& fnt = editor_catalog.GET_TRUETYPE_RESOURCE(_fnt);

        float tw = get_text_width(fnt, _text) * get_font_draw_scale();
        float th = get_text_height(fnt, _text) * get_font_draw_scale();

        // If text is a single line we calculate how much we can fit in the width
        // and if necessary trim any off and replace the end with and ellipsis.
        std::string text(_text);
        if (get_line_count(text.c_str()) == 1) {
            if (tw > _w) { // Our text goes out of the label bounds.
                if (text.length() <= 3) {
                    text = "...";
                    tw = get_text_width(fnt, text.c_str()) * get_font_draw_scale();
                } else {
                    text.replace(text.length()-3, 3, "...");
                    while (tw > _w && text.length() > 3) {
                        text.erase(text.length()-4, 1);
                        tw = get_text_width(fnt, text.c_str()) * get_font_draw_scale();
                    }
                }
            }
        }

        float x = cur.x;
        float y = cur.y;

        align_text(_horz, _vert, x, y, tw, th, _w, _h);

        float offset = 1.0f;

        Vec4 shadow = UI_COLOR_BLACK;
        Vec4 front = UI_COLOR_EX_LIGHT;

        if (locked || inactive) {
            shadow.a = 0.5f;
            front.a = 0.5f;
        }

        set_truetype_color(fnt, shadow);
        draw_text(fnt, x, y-offset, text.c_str());
        set_truetype_color(fnt, front);
        draw_text(fnt, x, y, text.c_str());

        advance_ui_cursor_end(ui_panels.peek(), _w, _h);
    }
    FILDEF void do_label (UI_Align _horz, UI_Align _vert, float _h, const char* _text, std::string _fnt, Vec4 _bg = Vec4())
    {
        Truetype& fnt = editor_catalog.GET_TRUETYPE_RESOURCE(_fnt);

        // Important to return ceiled value otherwise the next label using the
        // cursor to position itself might overlap the previous label by 1px.
        float w = ceilf(get_text_width(fnt, _text) * get_font_draw_scale());

        // Perform a normal text label now we know the width.
        return do_label(_horz, _vert, w, _h, _text, _fnt, _bg);
    }

    FILDEF void deselect_active_text_box ()
    {
        ui_text_box_cursor = std::string::npos;
        ui_active_text_box = UI_INVALID_ID;

        SDL_StopTextInput();
    }
    FILDEF void deselect_active_text_box (std::string& _text, const char* _default)
    {
        // If specified and the text box is empty on exit then we assign
        // the content of the text box to be the passed in default value.
        if (_default && !_text.length()) { _text = _default; }
        deselect_active_text_box();
    }

    FILDEF bool text_box_is_active ()
    {
        return (ui_active_text_box != UI_INVALID_ID);
    }

    FILDEF bool mouse_inside_level_editor_viewport(); // Predefine.

    STDDEF void do_text_box (float _w, float _h, UI_Flag _flags, std::string& _text, const char* _default = NULL)
    {
        // Cache the text box's flags so they are easily accessible.
        _flags |= ui_panels.peek().flags;
        bool locked = (_flags & UI_LOCKED);

        advance_ui_cursor_start(ui_panels.peek(), _w, _h);

        Truetype& fnt = editor_catalog.GET_TRUETYPE_RESOURCE(EDITOR_REGULAR_FONT);
        Vec2 cur = get_relative_panel_cursor(ui_panels.peek());

        // Locked text boxes cannot be interacted with.
        if (!locked) {
            if (handle_widget(cur.x, cur.y, _w, _h, locked)) {
                ui_cursor_visible = true;
                ui_cursor_blink_timer = UI_CURSOR_BLINK_INTERVAL;

                ui_text_box_cursor = _text.length();
                ui_active_text_box = ui_cur_id;

                SDL_StartTextInput();
            }
        } else {
            if (ui_active_text_box == ui_cur_id) {
                deselect_active_text_box(_text, _default);
            }
        }

        bool mouse_down = (SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK);
        bool mouse_r_down = (SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_RMASK);

        // If we are the active text box and the mouse was pressed this update
        // and we're not hit then that means the press was outside of us and
        // therefore we need to become deselected and can no longer be active.
        if (ui_active_text_box == ui_cur_id && mouse_down && !is_hit()) {
            deselect_active_text_box(_text, _default);
        }
        // If the right mouse button is pressed then we just always deselect.
        if (ui_active_text_box == ui_cur_id && mouse_r_down) {
            deselect_active_text_box(_text, _default);
        }

        // Handle setting the application's cursor to the correct graphic.
        if (is_hot()) {
            ui_hot_text_box = ui_cur_id;
            set_cursor("CURBEAM");
        } else {
            if (ui_hot_text_box == ui_cur_id) { ui_hot_text_box = UI_INVALID_ID; }

            // We have this check so that we can know it's okay to set
            // the cursor back to arrow as no text box elements are hot.
            if (ui_hot_text_box == UI_INVALID_ID) {
                // NOTE: Kind of hacky to put this here, but it prevents issues with
                // the flickering of the cursor due to text boxes. Could be cleaned.
                if (mouse_inside_level_editor_viewport() && get_window("WINMAIN").focus) {
                    if (current_editor_panel != EDITOR_PANEL_LEVEL) {
                        switch (tool_type) {
                        case (TOOL_TYPE_BRUSH):  { set_cursor("CURBRUSH"); } break;
                        case (TOOL_TYPE_FILL):   { set_cursor("CURPAINT"); } break;
                        case (TOOL_TYPE_SELECT): { set_cursor("CURCROSS"); } break;
                        }
                    } else {
                        set_cursor("CURARROW");
                    }
                } else {
                    set_cursor("CURARROW");
                }
            }
        }

        if (!locked && ui_make_next_text_box_active && get_render_target()->id == ui_text_box_tab_window_id) {
            ui_text_box_cursor = std::string::npos;
            ui_active_text_box = ui_cur_id;

            ui_make_next_text_box_active = false;
            ui_text_box_tab_window_id = 0;
        }

        Vec4 front   = UI_COLOR_EX_LIGHT;
        Vec4 shadow  = UI_COLOR_BLACK;
        Vec4 outline = UI_COLOR_DARK;
        Vec4 back    = UI_COLOR_MED_DARK;

        if (locked) {
            outline = UI_COLOR_MED_DARK;
            back = UI_COLOR_MEDIUM;

            shadow.a = 0.5f;
            front.a = 0.5f;
        }

        set_draw_color(outline); // Draw the text box's outline quad.
        fill_quad(cur.x, cur.y, cur.x+_w, cur.y+_h);
        set_draw_color(back); // Draw the text box's background quad.
        fill_quad(cur.x+1.0f, cur.y+1.0f, cur.x+_w-1.0f, cur.y+_h-1.0f);

        constexpr float X_PAD = 5.0f;
        constexpr float Y_PAD = 2.0f;

        float x = cur.x+(X_PAD     );
        float y = cur.y+(Y_PAD     );
        float w = _w   -(X_PAD*2.0f);
        float h = _h   -(Y_PAD*2.0f);

        // Handle text input events if we are the active text box.
        if (ui_active_text_box == ui_cur_id) {
            // Make sure that the cursor is in the bounds of the string.
            if (ui_text_box_cursor > _text.length()) {
                ui_text_box_cursor = _text.length();
            }

            if (get_render_target()->focus) {
                for (auto& text_event: ui_text_events) {
                    switch (text_event.type) {
                    case (UI_TEXT_EVENT_TEXT): {
                        char c = text_event.text;

                        if ((_flags&UI_ALPHANUM)   && !isalnum(c)) { break; }
                        if ((_flags&UI_ALPHABETIC) && !isalpha(c)) { break; }
                        if ((_flags&UI_NUMERIC)    && !isdigit(c)) { break; }

                        // Clear out the default text and enter what the user actually wants.
                        if ((_default) && _text == _default) {
                            ui_text_box_cursor = 0;
                            _text.clear();
                        }

                        auto pos = _text.begin()+(ui_text_box_cursor++);
                        _text.insert(pos, c);

                        ui_text_event_happened = true;
                    } break;
                    case (UI_TEXT_EVENT_KEY): {
                        switch (text_event.key)
                        {
                        case (SDLK_TAB): {
                            if (!ui_tab_handled) {
                                ui_make_next_text_box_active = true;
                                ui_text_box_tab_window_id = get_render_target()->id;
                                ui_tab_handled = true;
                                ui_text_event_happened = true;
                            }
                        } break;
                        case (SDLK_LEFT): {
                            if (ui_text_box_cursor > 0) {
                                --ui_text_box_cursor;
                                ui_text_event_happened = true;
                            }
                        } break;
                        case (SDLK_RIGHT): {
                            if (ui_text_box_cursor < _text.length()) {
                                ++ui_text_box_cursor;
                                ui_text_event_happened = true;
                            }
                        } break;
                        case (SDLK_UP): {
                            if (_flags&UI_NUMERIC) {
                                if (atoi(_text.c_str()) < INT_MAX) {
                                    _text = std::to_string(atoi(_text.c_str())+1);
                                    ui_text_box_cursor = _text.length();
                                    ui_text_event_happened = true;
                                }
                            }
                        } break;
                        case (SDLK_DOWN): {
                            if (_flags&UI_NUMERIC) {
                                if (atoi(_text.c_str()) > 0) {
                                    _text = std::to_string(atoi(_text.c_str())-1);
                                    ui_text_box_cursor = _text.length();
                                    ui_text_event_happened = true;
                                }
                            }
                        } break;
                        case (SDLK_HOME): {
                            ui_text_box_cursor = 0;
                            ui_text_event_happened = true;
                        } break;
                        case (SDLK_END): {
                            ui_text_box_cursor = _text.length();
                            ui_text_event_happened = true;
                        } break;
                        case (SDLK_BACKSPACE): {
                            if (ui_text_box_cursor != 0) {
                                _text.erase(--ui_text_box_cursor, 1);
                                ui_text_event_happened = true;
                            }
                        } break;
                        case (SDLK_DELETE): {
                            if (ui_text_box_cursor < _text.length()) {
                                _text.erase(ui_text_box_cursor, 1);
                                ui_text_event_happened = true;
                            }
                        } break;
                        case (SDLK_ESCAPE):
                        case (SDLK_RETURN): {
                            deselect_active_text_box(_text, _default);
                        } break;
                        }
                    } break;
                    }
                }

                ui_text_events.clear();

                // Reset the cursor blink interval.
                if (ui_text_event_happened) {
                    // Start the blinking of the cursor.
                    ui_cursor_visible = true;
                    ui_cursor_blink_timer = UI_CURSOR_BLINK_INTERVAL;
                }
            }

            // Cursor should always be at the end of the default text.
            if ((_default) && _text == _default) {
                ui_text_box_cursor = _text.length();
            }
        }

        // We scissor the contents to avoid text overspill.
        begin_scissor(x, y, w, h);

        // Calculate the position of the text and draw it.
        float tx = x;
        float ty = y;
        float tw = get_text_width(fnt, _text.c_str()) * get_font_draw_scale();
        float th = get_text_height(fnt, _text.c_str()) * get_font_draw_scale();

        align_text(UI_ALIGN_RIGHT, UI_ALIGN_CENTER, tx, ty, tw, th, w, h);

        float x_off = 0.0f;
        float y_off = 1.0f;

        // Adjust text position/offsetrun based on the current cursor.
        if (ui_active_text_box == ui_cur_id) {
            std::string sub(_text.substr(0, ui_text_box_cursor));
            float cursor_x = tx+(get_text_width(fnt, sub.c_str()) * get_font_draw_scale());
            if (cursor_x < x) { x_off = (x - cursor_x); }
        }

        set_truetype_color(fnt, shadow);
        draw_text(fnt, tx+x_off, ty-y_off, _text.c_str());
        set_truetype_color(fnt, front);
        draw_text(fnt, tx+x_off, ty, _text.c_str());

        end_scissor();

        // If we're active then draw the text box cursor as well.
        if (ui_active_text_box == ui_cur_id && ui_cursor_visible) {
            begin_scissor(x-1.0f, y-1.0f, w+2.0f, h+2.0f);

            std::string sub(_text.substr(0, ui_text_box_cursor));

            float xo = (get_text_width(fnt, sub.c_str()) * get_font_draw_scale());
            float yo = (h-th)/2.0f; // Center the cursor vertically.

            // Just looks nicer...
            if ((ui_text_box_cursor != 0 && _text.length()) || (!_text.length())) {
                xo += 1.0f;
            }

            set_draw_color(UI_COLOR_EX_LIGHT);
            draw_line(tx+xo+x_off, y+yo, tx+xo+x_off, y+yo+th);

            end_scissor();
        }

        advance_ui_cursor_end(ui_panels.peek(), _w, _h);

        ++ui_cur_id;
    }
    STDDEF void do_text_box_labeled (float _w, float _h, UI_Flag _flags, std::string& _text, const char* _label, const char* _default = NULL)
    {
        Truetype& fnt = editor_catalog.GET_TRUETYPE_RESOURCE(EDITOR_REGULAR_FONT);

        // Important to return ceiled value otherwise the next label using the
        // cursor to position itself might overlap the previous label by 1px.
        float lw = ceilf(get_text_width(fnt, _label) * get_font_draw_scale());
        float tw = _w - lw;

        if (tw < 0.0f) { return; } // Won't draw anything!

        // Cache this stuff because we are going to temporarily change it.
        Vec2 cursor = *ui_panels.peek().cursor;
        UI_Dir dir = ui_panels.peek().cursor_dir;

        set_panel_cursor_dir(UI_DIR_RIGHT);
        do_label(UI_ALIGN_LEFT, UI_ALIGN_CENTER, lw, _h, _label, EDITOR_REGULAR_FONT);

        set_panel_cursor_dir(dir);
        do_text_box(tw, _h, _flags, _text, _default);

        // Reset the X location of the cursor for the caller.
        if (dir == UI_DIR_UP || dir == UI_DIR_DOWN) {
            ui_panels.peek().cursor->x = cursor.x;
        }
    }
    STDDEF void do_text_box_labeled (float _w, float _h, UI_Flag _flags, std::string& _text, float _label_w, const char* _label, const char* _default = NULL)
    {
        Truetype& fnt = editor_catalog.GET_TRUETYPE_RESOURCE(EDITOR_REGULAR_FONT);

        float lw = _label_w;
        float tw = _w - lw;

        if (tw < 0.0f) { return; } // Won't draw anything!

        // Cache this stuff because we are going to temporarily change it.
        Vec2 cursor = *ui_panels.peek().cursor;
        UI_Dir dir = ui_panels.peek().cursor_dir;

        set_panel_cursor_dir(UI_DIR_RIGHT);
        do_label(UI_ALIGN_LEFT, UI_ALIGN_CENTER, lw, _h, _label, EDITOR_REGULAR_FONT);

        set_panel_cursor_dir(dir);
        do_text_box(tw, _h, _flags, _text, _default);

        // Reset the X location of the cursor for the caller.
        if (dir == UI_DIR_UP || dir == UI_DIR_DOWN) {
            ui_panels.peek().cursor->x = cursor.x;
        }
    }

    STDDEF void do_scrollbar (float _x, float _y, float _w, float _h, float _content_height, float& _scroll_offset)
    {
        // Allows scrollbars to be outside the panel they are scrolling.
        set_viewport(0.0f, 0.0f, get_render_target_w(), get_render_target_h());
        defer { set_viewport(ui_panels.peek().viewport); };

        _x += ui_panels.peek().viewport.x;
        _y += ui_panels.peek().viewport.y;

        // We scissor the contents to avoid any overspill.
        begin_scissor(_x, _y, _w, _h);
        defer { end_scissor(); };

        constexpr float PAD = 1.0f;

        float x = _x +  PAD;
        float y = _y +  PAD;
        float w = _w - (PAD*2);
        float h = _h;

        // Determine the percentage of content visible.
        float percent_visible = get_panel_h() / _content_height;
        if (percent_visible > 1.0f) { percent_visible = 1.0f; }

        // Represent this amount in the scrollbar.
        h = (h*percent_visible) - (PAD*2);

        // Ensure the normalized scroll offset is in bounds.
        float ndc_h = (_h-h-(PAD*2)) / (_h-(PAD*2));
        _scroll_offset = std::clamp(_scroll_offset, 0.0f, ndc_h);

        // Convert the normalized scroll offset into pixel offset.
        y += _scroll_offset * (_h-(PAD*2));

        handle_widget(x, y, w, h, false);

        // Adjust the offset by however much the mouse has moved.
        if (is_hit()) {
            _scroll_offset += (ui_mouse_relative.y / (_h-(PAD*2)));
            _scroll_offset = std::clamp(_scroll_offset, 0.0f, ndc_h);
        }

        set_draw_color(UI_COLOR_EX_DARK);
        fill_quad(_x, _y, _x+_w, _y+_h);

        Vec4 color = UI_COLOR_MED_DARK;

        if      (is_hit()) { color = UI_COLOR_MED_LIGHT; }
        else if (is_hot()) { color = UI_COLOR_MEDIUM;    }

        set_draw_color(color);
        fill_quad(x, y, x+w, y+h);

        // Draw the three lines on the scrollbar (avoid overspill).
        //
        // We do not bother though if it's really small because then
        // adding these lines just looks sort of gross and cluttered.
        if (h > 10) {
            constexpr float LINE_GAP = 2.0f;

            begin_scissor(x, y, w, h);
            set_draw_color(UI_COLOR_EX_DARK);

            float y1 = y+(h/2.0f) - LINE_GAP;
            float y2 = y+(h/2.0f);
            float y3 = y+(h/2.0f) + LINE_GAP;

            draw_line(x+1.0f, y1, x+w-1.0f, y1);
            draw_line(x+1.0f, y2, x+w-1.0f, y2);
            draw_line(x+1.0f, y3, x+w-1.0f, y3);

            end_scissor();
        }

        // Set the panels relative offset to apply the actual scroll.
        //
        // We do this extra check stuff because towards the end of
        // scrolling there can be offset issues which result in
        // moving past the content bounds so we just perform manual
        // adjustment of the scroll in order to correct this issue.
        float final_offset = roundf(_content_height * _scroll_offset);
        float resulting_height = _content_height - final_offset;
        if (_scroll_offset != 0.0f && resulting_height < get_panel_h()) {
            float difference = get_panel_h() - resulting_height;
            ui_panels.peek().relative_offset.y -= (final_offset - difference);
        } else {
            ui_panels.peek().relative_offset.y -= roundf(_content_height * _scroll_offset);
        }

        ++ui_cur_id;
    }
    FILDEF void do_scrollbar (Quad _bounds, float _content_height, float& _scroll_offset)
    {
        do_scrollbar(_bounds.x, _bounds.y, _bounds.w, _bounds.h, _content_height, _scroll_offset);
    }

    FILDEF History_State& get_current_history_state ()
    {
        return history.state[history.current_position];
    }
    STDDEF void new_history_state (History_Action _action)
    {
        if (_action == HISTORY_ACTION_TILE && !mouse_inside_level_editor_viewport()) { return; }

        // Don't bother creating a new state if the current erase/place action is
        // empty otherwise we will end up with a bunch of empty states in the list.
        if (history.current_position > -1) {
            History_State& current = get_current_history_state();
            if (current.tile_info.empty() && current.action == _action && _action == HISTORY_ACTION_TILE) {
                return;
            }
        }

        // Clear all the history after the current position, if there is any, as it
        // will no longer apply to the timeline of level editor actions anymore.
        int delete_position = history.current_position+1;
        if (delete_position < CAST(int, history.state.size())) {
            auto begin = history.state.begin();
            auto end = history.state.end();

            history.state.erase(begin+delete_position, end);
        }

        /*
        // If it's a selection action then we don't need to modify this.
        if (_action != HISTORY_ACTION_SELECT_STATE) {
            get_current_level_tab().unsaved_changes = true;
        }
        */

        history.state.push_back(History_State());
        history.state.back().action = _action;

        /*
        // Also deal with the select bounds for selection actions.
        if (_action == HISTORY_ACTION_SELECT_STATE) {
            tab.history.state.back().old_select_state = tab.old_select_state;
            tab.history.state.back().new_select_state = tab.tool_info.select.bounds;
        }
        */

        /*
        // Also deal with width and height for resizing.
        if (_action == HISTORY_ACTION_RESIZE) {
            tab.history.state.back().resize_dir = get_resize_dir();
            tab.history.state.back().old_width  = tab.level.header.width;
            tab.history.state.back().old_height = tab.level.header.height;
            tab.history.state.back().new_width  = get_resize_w();
            tab.history.state.back().new_height = get_resize_h();
        }
        */

        ++history.current_position;
    }
    STDDEF void add_to_history_tile_state (History_Tile_Info _info)
    {
        if (!mouse_inside_level_editor_viewport()) { return; }

        // If there is no current action then we create one. This resolved some
        // potential bugs that can occur when undoing/redoing mid stroke, etc.
        if (history.current_position <= -1) {
            new_history_state(HISTORY_ACTION_TILE);
        }

        if (get_current_history_state().action != HISTORY_ACTION_TILE) {
            new_history_state(HISTORY_ACTION_TILE);
        }

        // Don't add the same tiles repeatedly, otherwise add the tile.
        History_State& state = get_current_history_state();
        if (!state.tile_info.empty()) {
            const History_Tile_Info& n = _info;
            for (auto& o: state.tile_info) {
                if ((o.x == n.x) && (o.y == n.y) && (o.new_id != n.new_id) && (o.layer == n.layer)) {
                    o.new_id = n.new_id;
                }
                if ((o.x == n.x) && (o.y == n.y) && (o.new_id == n.new_id) && (o.layer == n.layer)) {
                    return;
                }
            }
        }

        state.tile_info.push_back(_info);
    }

    STDDEF bool le_save_area ()
    {
        if (current_area.file_name.empty()) {
            std::string file_name(save_dialog());
            if (file_name.empty()) { return false; }
            current_area.file_name = file_name;
        }

        save_area(current_area, current_area.file_name.c_str());

        unsaved_changes = false;
        return true;
    }
    STDDEF bool le_save_as_area ()
    {
        std::string file_name(save_dialog());
        if (file_name.empty()) { return false; }

        save_area(current_area, file_name.c_str());
        current_area.file_name = file_name;

        unsaved_changes = false;
        return true;
    }

    STDDEF int save_changes_prompt ()
    {
        if (!unsaved_changes) { return ALERT_RESULT_NULL; }
        int result = show_alert("Unsaved Changes", "Unsaved Changes!\nWould you like to save?", ALERT_TYPE_WARNING, ALERT_BUTTON_YES_NO_CANCEL, &get_window("WINMAIN"));
        if (result == ALERT_RESULT_YES) {
            if (!le_save_area()) {
                return ALERT_RESULT_CANCEL;
            }
        }
        return result;
    }

    FILDEF void create_blank_area ()
    {
        current_area.file_name.clear();

        current_area.header.version       = CURRENT_AREA_VERSION;
        current_area.header.width         = DEFAULT_AREA_WIDTH;
        current_area.header.height        = DEFAULT_AREA_HEIGHT;
        current_area.header.thing_count   = 0;
        current_area.header.name          = DEFAULT_AREA_NAME;
        current_area.header.effect        = DEFAULT_AREA_EFFECT;
        current_area.header.skybox        = DEFAULT_AREA_SKYBOX;
        current_area.header.skybox_effect = DEFAULT_AREA_SKYBOX_EFFECT;

        u32 lw = current_area.header.width;
        u32 lh = current_area.header.height;

        for (int i=0; i<Area::TILE_LAYER_TOTAL; ++i) {
            current_area.tiles.at(i).clear();
            current_area.tiles.at(i).resize(lw*lh, Tile());
        }

        current_area.things.clear();

        // RESET TO DEFAULTS

        camera_pos = Vec2(0.0f, 0.0f);
        camera_zoom = 1.0f;

        history.current_position = -1;
    }

    FILDEF void do_tile_palette (Vec2& _cur)
    {
        Vec2 cursor(PALETTE_PANEL_INNER_PAD, PALETTE_PANEL_INNER_PAD+get_panel_offset().y);
        size_t index = 0;

        for (const auto& it: ::internal::tile_info_map) {
            const auto& info = it.second;

            Vec2 pos = cursor;

            Quad bounds = { cursor.x, cursor.y-(PALETTE_PANEL_INNER_PAD/2.0f), PALETTE_PANEL_LABEL_W, (PALETTE_PANEL_ITEM_SIZE + PALETTE_PANEL_LABEL_H) };
            if (mouse_in_ui_bounds_xywh(bounds) || selected_tile == it.first) { // Highlight if moused over or selected.
                if (SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK) {
                    selected_tile = it.first;
                }

                set_draw_color(UI_COLOR_MEDIUM);
                fill_quad(bounds.x,bounds.y,bounds.x+bounds.w,bounds.y+bounds.h);
            }

            float item_offset = ((PALETTE_PANEL_LABEL_W - PALETTE_PANEL_ITEM_SIZE) / 2);
            pos.x += item_offset;

            // Center anchor.
            pos.x += (PALETTE_PANEL_ITEM_SIZE / 2.0f);
            pos.y += (PALETTE_PANEL_ITEM_SIZE / 2.0f);

            Texture& tex = texture_catalog.GET_TEXTURE_RESOURCE(info.texture);
            float scale = PALETTE_PANEL_ITEM_SIZE / tex.h;

            // Draw a border around the tile so that is pops a bit more.
            // set_draw_color(UI_COLOR_EX_DARK);
            // fill_quad(cursor.x+item_offset-1.0f, cursor.y-1.0f, cursor.x+item_offset+PALETTE_PANEL_ITEM_SIZE+1.0f, cursor.y+PALETTE_PANEL_ITEM_SIZE+1.0f);

            begin_scissor(cursor.x+item_offset, cursor.y, PALETTE_PANEL_ITEM_SIZE, PALETTE_PANEL_ITEM_SIZE);
            set_texture_draw_scale(scale, scale);
            draw_texture(tex, pos.x, pos.y);
            end_scissor();

            _cur.x = cursor.x;
            _cur.y = (cursor.y - get_panel_offset().y) + PALETTE_PANEL_ITEM_SIZE;

            do_label(UI_ALIGN_CENTER, UI_ALIGN_CENTER, PALETTE_PANEL_LABEL_W, PALETTE_PANEL_LABEL_H, info.name.c_str(), EDITOR_REGULAR_FONT);

            ++index;
            if ((index % ITEMS_PER_ROW) == 0) {
                cursor.x = PALETTE_PANEL_INNER_PAD;
                cursor.y += PALETTE_PANEL_ITEM_SIZE + PALETTE_PANEL_LABEL_H + PALETTE_PANEL_INNER_PAD;
            } else {
                cursor.x += PALETTE_PANEL_LABEL_W + PALETTE_PANEL_INNER_PAD;
            }
        }
    }
    FILDEF void do_thing_palette (Vec2& _cur)
    {
        Vec2 cursor(PALETTE_PANEL_INNER_PAD, PALETTE_PANEL_INNER_PAD+get_panel_offset().y);
        size_t index = 0;

        for (const auto& it: ::internal::thing_info_map) {
            const auto& info = it.second;

            Vec2 pos = cursor;

            Quad bounds = { cursor.x, cursor.y-(PALETTE_PANEL_INNER_PAD/2.0f), PALETTE_PANEL_LABEL_W, (PALETTE_PANEL_ITEM_SIZE + PALETTE_PANEL_LABEL_H) };
            if (mouse_in_ui_bounds_xywh(bounds) || selected_thing == it.first) { // Highlight if moused over or selected.
                if (SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK) {
                    selected_thing = it.first;
                }

                set_draw_color(UI_COLOR_MEDIUM);
                fill_quad(bounds.x,bounds.y,bounds.x+bounds.w,bounds.y+bounds.h);
            }

            float item_offset = ((PALETTE_PANEL_LABEL_W - PALETTE_PANEL_ITEM_SIZE) / 2);
            pos.x += item_offset;

            // Center anchor.
            pos.x += (PALETTE_PANEL_ITEM_SIZE / 2.0f);
            pos.y += (PALETTE_PANEL_ITEM_SIZE / 2.0f);

            Texture& tex = sprite_catalog.GET_TEXTURE_RESOURCE(info.sprite);
            float scale = std::min(PALETTE_PANEL_ITEM_SIZE / tex.h, 1.0f); // If the thing is smaller than item size don't scale up.

            begin_scissor(cursor.x+item_offset, cursor.y, PALETTE_PANEL_ITEM_SIZE, PALETTE_PANEL_ITEM_SIZE);
            set_texture_draw_scale(scale, scale);
            draw_texture(tex, pos.x, pos.y);
            end_scissor();

            _cur.x = cursor.x;
            _cur.y = (cursor.y - get_panel_offset().y) + PALETTE_PANEL_ITEM_SIZE;

            do_label(UI_ALIGN_CENTER, UI_ALIGN_CENTER, PALETTE_PANEL_LABEL_W, PALETTE_PANEL_LABEL_H, info.name.c_str(), EDITOR_REGULAR_FONT);

            ++index;
            if ((index % ITEMS_PER_ROW) == 0) {
                cursor.x = PALETTE_PANEL_INNER_PAD;
                cursor.y += PALETTE_PANEL_ITEM_SIZE + PALETTE_PANEL_LABEL_H + PALETTE_PANEL_INNER_PAD;
            } else {
                cursor.x += PALETTE_PANEL_LABEL_W + PALETTE_PANEL_INNER_PAD;
            }
        }
    }

    FILDEF void do_palette_panel (float _y, float _h, bool _scrollbar)
    {
        if (current_editor_panel == EDITOR_PANEL_TILES) {
            tile_palette_panel_content_height = ((PALETTE_PANEL_ITEM_SIZE + PALETTE_PANEL_LABEL_H + PALETTE_PANEL_INNER_PAD) * ceilf((float)::internal::tile_info_map.size() / ITEMS_PER_ROW));
        }
        if (current_editor_panel == EDITOR_PANEL_THINGS) {
            thing_palette_panel_content_height = ((PALETTE_PANEL_ITEM_SIZE + PALETTE_PANEL_LABEL_H + PALETTE_PANEL_INNER_PAD) * ceilf((float)::internal::thing_info_map.size() / ITEMS_PER_ROW));
        }

        begin_panel(0.0f, _y, get_panel_w(), _h, UI_NONE, UI_COLOR_MEDIUM);

        float x = PALETTE_PANEL_INNER_PAD;
        float y = PALETTE_PANEL_INNER_PAD;
        float w = CONTROL_PANEL_WIDTH - (PALETTE_PANEL_INNER_PAD * 2.0f);
        float h = get_panel_h()       - (PALETTE_PANEL_INNER_PAD * 2.0f);

        begin_panel(x, y, w, h, UI_NONE, UI_COLOR_MED_DARK);

        tile_palette_panel_panel_height = get_panel_h();
        thing_palette_panel_panel_height = get_panel_h();

        if (_scrollbar) {
            float sx = 3.0f + get_viewport().w + CONTROL_PANEL_INNER_PAD;
            float sy = 0.0f;
            float sw = CONTROL_PANEL_SCROLLBAR_WIDTH - CONTROL_PANEL_INNER_PAD;
            float sh = get_viewport().h;

            if (current_editor_panel == EDITOR_PANEL_TILES) {
                do_scrollbar(sx,sy,sw,sh, tile_palette_panel_content_height, tile_palette_panel_scroll_offset);
            }
            if (current_editor_panel == EDITOR_PANEL_THINGS) {
                do_scrollbar(sx,sy,sw,sh, thing_palette_panel_content_height, thing_palette_panel_scroll_offset);
            }
        }

        Vec2 cursor(0.0f, 0.0f);

        set_panel_cursor(&cursor);
        set_panel_cursor_dir(UI_DIR_RIGHT);

        if (current_editor_panel == EDITOR_PANEL_TILES) { do_tile_palette(cursor); }
        if (current_editor_panel == EDITOR_PANEL_THINGS) { do_thing_palette(cursor); }

        end_panel();
        end_panel();
    }
    FILDEF bool palette_panel_needs_scrollbar ()
    {
        if (current_editor_panel == EDITOR_PANEL_TILES) { return (tile_palette_panel_content_height > tile_palette_panel_panel_height); }
        if (current_editor_panel == EDITOR_PANEL_THINGS) { return (thing_palette_panel_content_height > thing_palette_panel_panel_height); }

        return false;
    }

    FILDEF void do_property__string (float _pad, float _lw, std::string _label, UI_Flag _flags, std::string& _str)
    {
        float w = get_panel_w() - (PROPERTIES_PANEL_INNER_PAD*2.0f);
        float h = PROPERTIES_PANEL_ITEM_HEIGHT - 2.0f;

        do_text_box_labeled(w,h, _flags, _str, _lw, _label.c_str());
        advance_panel_cursor(_pad);
    }

    FILDEF void do_tile_properties ()
    {
        // @Incomplete: ...
    }
    FILDEF void do_thing_properties ()
    {
        // @Incomplete: ...
    }

    FILDEF void do_properties_panel (float _y, float _h, bool _scrollbar)
    {
        // @Incomplete: Handle scrolling...
        PERSISTENT float content_height = 0.0f;
        PERSISTENT float scroll_offset = 0.0f;

        begin_panel(0.0f, _y, get_panel_w(), _h, UI_NONE, UI_COLOR_MEDIUM);

        if (_scrollbar) {
            Quad sb = { CONTROL_PANEL_WIDTH, CONTROL_PANEL_INNER_PAD, (CONTROL_PANEL_SCROLLBAR_WIDTH - CONTROL_PANEL_INNER_PAD), (get_viewport().h - (CONTROL_PANEL_INNER_PAD * 2.0f)) };
            do_scrollbar(sb, content_height, scroll_offset);
        }

        // @Incomplete: ...

        end_panel();
    }
    FILDEF bool properties_panel_needs_scrollbar ()
    {
        return false;
    }

    GLOBAL float level_properties_panel_content_height;
    GLOBAL float level_properties_panel_scroll_offset;
    GLOBAL float level_properties_panel_panel_height;

    FILDEF void do_level_properties_panel (float _y, float _h, bool _scrollbar)
    {
        const char* LABELS[] = { "WIDTH", "HEIGHT", "THINGS", "NAME", "EFFECT", "SKYBOX", "SKYBOX FX" };
        const float PADS[] = { 2.0f, 2.0f, 8.0f, 2.0f, 8.0f, 2.0f, 0.0f };

        level_properties_panel_content_height = PROPERTIES_PANEL_INNER_PAD * 2.0f;
        level_properties_panel_content_height += (PROPERTIES_PANEL_ITEM_HEIGHT-2.0f) * STATIC_ARRAY_COUNT(LABELS);
        for (int i=0; i<STATIC_ARRAY_COUNT(PADS); ++i) {
            level_properties_panel_content_height += PADS[i];
        }

        begin_panel(0.0f, _y, get_panel_w(), _h, UI_NONE, UI_COLOR_MEDIUM);
        level_properties_panel_panel_height = get_viewport().h;

        if (_scrollbar) {
            Quad sb = { CONTROL_PANEL_WIDTH, CONTROL_PANEL_INNER_PAD, (CONTROL_PANEL_SCROLLBAR_WIDTH - CONTROL_PANEL_INNER_PAD), (get_viewport().h - (CONTROL_PANEL_INNER_PAD * 2.0f)) };
            do_scrollbar(sb, level_properties_panel_content_height, level_properties_panel_scroll_offset);
        }

        Vec2 cursor(PROPERTIES_PANEL_INNER_PAD, PROPERTIES_PANEL_INNER_PAD);

        set_panel_cursor(&cursor);
        set_panel_cursor_dir(UI_DIR_DOWN);

        float lw = 0.0f;
        for (int i=0; i<STATIC_ARRAY_COUNT(LABELS); ++i) {
            float tempw = get_text_width(editor_catalog.GET_TRUETYPE_RESOURCE(EDITOR_REGULAR_FONT), (std::string(LABELS[i]) + " ").c_str()) * get_font_draw_scale();
            if (tempw > lw) {
                lw = tempw;
            }
        }

        std::string w_str(std::to_string(current_area.header.width));
        std::string h_str(std::to_string(current_area.header.height));
        std::string t_str(std::to_string(current_area.things.size())); // Use this instead of THING_COUNT cos that only gets updated on save.

        do_property__string(PADS[0], lw, LABELS[0], UI_LOCKED, w_str);
        do_property__string(PADS[1], lw, LABELS[1], UI_LOCKED, h_str);
        do_property__string(PADS[2], lw, LABELS[2], UI_LOCKED, t_str);
        do_property__string(PADS[3], lw, LABELS[3], UI_NONE,   current_area.header.name);
        do_property__string(PADS[4], lw, LABELS[4], UI_NONE,   current_area.header.effect);
        do_property__string(PADS[5], lw, LABELS[5], UI_NONE,   current_area.header.skybox);
        do_property__string(PADS[6], lw, LABELS[6], UI_NONE,   current_area.header.skybox_effect);

        end_panel();
    }
    FILDEF bool level_properties_panel_needs_scrollbar ()
    {
        return (level_properties_panel_content_height > level_properties_panel_panel_height);
    }

    FILDEF bool do_layer_button (UI_Flag _flags, std::string _name, bool _active, bool _selected, Vec4 _color)
    {
        if (!_active) { _flags |= UI_INACTIVE; }
        std::string img = (_flags & UI_INACTIVE) ? "ICONHIDE" : "ICONSHOW";

        float bw = get_panel_w();
        float bh = LAYER_PANEL_BUTTON_H;

        if (_selected) { _flags |= UI_HIGHLIGHT; }
        bool result = begin_click_panel(NULL, bw,bh, _flags);

        float pad = 5.0f;
        Vec2 cursor(pad, 0.0f);

        set_panel_cursor(&cursor);
        set_panel_cursor_dir(UI_DIR_RIGHT);

        float w = 10.0f;
        float h = (LAYER_PANEL_BUTTON_H-4.0f)-1.0f; // -1.0f due to separator!

        if constexpr (LAYER_PANEL_BUTTON_H < 38.0f) {
            float scale = 32.0f / LAYER_PANEL_BUTTON_H;
            set_texture_draw_scale(scale, scale);
        }

        cursor.y = (bh-h)/2.0f;
        do_quad(w, h, _color);
        cursor.y = 0.0f;
        advance_panel_cursor(pad);
        do_icon(LAYER_PANEL_BUTTON_H, get_panel_h(), img);
        advance_panel_cursor(pad);
        do_label(UI_ALIGN_LEFT, UI_ALIGN_CENTER, get_panel_h(), _name.c_str(), EDITOR_REGULAR_FONT);

        end_panel();
        return result;
    }
    FILDEF void do_layer_panel (float _y, float _h, bool _scrollbar)
    {
        float pad = LAYER_PANEL_INNER_PAD;

        begin_panel(0.0f, _y, get_panel_w(), _h, UI_NONE, UI_COLOR_MEDIUM);
        begin_panel(pad, pad, CONTROL_PANEL_WIDTH-(pad*2), _h-(pad*2), UI_NONE, UI_COLOR_MED_DARK);

        // NOTE: We do this to add a 1px border around the actual layer buttons in
        // the case that the panel is too small and needs a scrollbar it looks
        // nicer. Its a bit hacky and at some point we should probs have a feature
        // to add a padding border around a panel but for now we just do this...
        begin_panel(1.0f, 1.0f, get_panel_w()-2.0f, get_panel_h()-2.0f, UI_NONE, UI_COLOR_MED_DARK);
        layer_panel_panel_height = get_panel_h();

        Vec2 cursor(0.0f, 0.0f);

        set_panel_cursor(&cursor);
        set_panel_cursor_dir(UI_DIR_DOWN);

        if (_scrollbar) {
            float sx =  1.0f + get_viewport().w + CONTROL_PANEL_INNER_PAD;
            float sy = -1.0f;
            float sw = CONTROL_PANEL_SCROLLBAR_WIDTH - CONTROL_PANEL_INNER_PAD;
            float sh =  2.0f + get_viewport().h;

            do_scrollbar(sx, sy, sw, sh, layer_panel_content_height, layer_panel_scroll_offset);
        }

        bool thing_layer_selected = (current_editor_panel == EDITOR_PANEL_THINGS);
        bool ceils_layer_selected = false;
        bool walls_layer_selected = false;
        bool flats_layer_selected = false;

        if (current_editor_panel == EDITOR_PANEL_TILES) {
            if (selected_tile != NO_TILE_SELECTED) {
                ceils_layer_selected = (get_tile_info(selected_tile).type == TILE_TYPE_CEIL);
                walls_layer_selected = (get_tile_info(selected_tile).type == TILE_TYPE_WALL);
                flats_layer_selected = (get_tile_info(selected_tile).type == TILE_TYPE_FLAT);
            }
        }

        if (do_layer_button(UI_NONE, "[4] THING", thing_layer_active,                       thing_layer_selected, Vec4(0.75f, 0.38f, 0.38f, 1.0f))) { thing_layer_active = !thing_layer_active; }
        if (do_layer_button(UI_NONE, "[3] CEILS", tile_layer_active[Area::TILE_LAYER_CEIL], ceils_layer_selected, Vec4(0.44f, 0.51f, 0.72f, 1.0f))) { tile_layer_active[Area::TILE_LAYER_CEIL] = !tile_layer_active[Area::TILE_LAYER_CEIL]; }
        if (do_layer_button(UI_NONE, "[2] WALLS", tile_layer_active[Area::TILE_LAYER_WALL], walls_layer_selected, Vec4(0.79f, 0.69f, 0.20f, 1.0f))) { tile_layer_active[Area::TILE_LAYER_WALL] = !tile_layer_active[Area::TILE_LAYER_WALL]; }
        if (do_layer_button(UI_NONE, "[1] FLATS", tile_layer_active[Area::TILE_LAYER_FLAT], flats_layer_selected, Vec4(0.45f, 0.59f, 0.41f, 1.0f))) { tile_layer_active[Area::TILE_LAYER_FLAT] = !tile_layer_active[Area::TILE_LAYER_FLAT]; }

        end_panel();
        end_panel();
        end_panel();
    }
    FILDEF bool layer_panel_needs_scrollbar ()
    {
        return (layer_panel_content_height > layer_panel_panel_height);
    }

    FILDEF void deselect(); // Predefine.

    FILDEF void do_control_panel ()
    {
        control_panel_width = CONTROL_PANEL_WIDTH;
        bool scrollbar;
        if (current_editor_panel == EDITOR_PANEL_LEVEL) {
            scrollbar = level_properties_panel_needs_scrollbar() || layer_panel_needs_scrollbar();
        } else {
            scrollbar = palette_panel_needs_scrollbar() || properties_panel_needs_scrollbar() || layer_panel_needs_scrollbar();
        }
        if (scrollbar) {
            control_panel_width += CONTROL_PANEL_SCROLLBAR_WIDTH;
        }

        float y = 0.0f;

        begin_panel(get_viewport().w - control_panel_width, 0.0f, control_panel_width, get_viewport().h, UI_NONE);

        ////////////////////////////////////////////////////////////////////////
        constexpr float TABS_HEIGHT = TOOL_PANEL_WIDTH;
        begin_panel(0.0f, 0.0f, get_panel_w(), TABS_HEIGHT, UI_NONE, UI_COLOR_MEDIUM);

        Vec2 cursor(0.0f, 0.0f);

        set_panel_cursor(&cursor);
        set_panel_cursor_dir(UI_DIR_RIGHT);

        float bw1 = roundf(get_panel_w() / 3.0f);
        float bw2 = get_panel_w() - (bw1 * 2.0f); // Make sure we fill the space.
        float bh  = get_panel_h();

        UI_Flag level_flags  = (current_editor_panel == EDITOR_PANEL_LEVEL)  ? UI_NONE : UI_INACTIVE;
        UI_Flag tiles_flags  = (current_editor_panel == EDITOR_PANEL_TILES)  ? UI_NONE : UI_INACTIVE;
        UI_Flag things_flags = (current_editor_panel == EDITOR_PANEL_THINGS) ? UI_NONE : UI_INACTIVE;

        if (do_button_txt(NULL, bw1,bh, level_flags,  "LEVEL", EDITOR_BOLD_FONT, true)) {             current_editor_panel = EDITOR_PANEL_LEVEL;  }
        if (do_button_txt(NULL, bw1,bh, tiles_flags,  "TILES", EDITOR_BOLD_FONT, true)) {             current_editor_panel = EDITOR_PANEL_TILES;  }
        if (do_button_txt(NULL, bw2,bh, things_flags, "THING", EDITOR_BOLD_FONT, true)) { deselect(); current_editor_panel = EDITOR_PANEL_THINGS; }

        end_panel();
        ////////////////////////////////////////////////////////////////////////

        constexpr float MAIN_SPACE_H = 0.75f; // NDC

        y += TABS_HEIGHT + 1.0f;

        if (current_editor_panel != EDITOR_PANEL_LEVEL)
        {
            float left_over_space = get_panel_h() - y;
            float layer_panel_h = roundf((layer_panel_content_height + 2.0f + (LAYER_PANEL_INNER_PAD * 2.0f)));
            float palette_and_properties_space = left_over_space - layer_panel_h;
            float palette_panel_h = roundf(palette_and_properties_space / 2.0f);
            float properties_panel_h = roundf(palette_and_properties_space - palette_panel_h);

            do_palette_panel(y, palette_panel_h-1.0f, scrollbar);
            y += palette_panel_h;
            do_properties_panel(y, properties_panel_h-1.0f, scrollbar);
            y += properties_panel_h;
            do_layer_panel(y, layer_panel_h, scrollbar);
        }
        else
        {
            float left_over_space = get_panel_h() - y;
            float layer_panel_h = roundf((layer_panel_content_height + 2.0f + (LAYER_PANEL_INNER_PAD * 2.0f)));
            float level_panel_h = roundf(left_over_space - layer_panel_h);

            do_level_properties_panel(y, level_panel_h-1.0f, scrollbar);
            y += level_panel_h;
            do_layer_panel(y, layer_panel_h, scrollbar);
        }

        end_panel();
    }

    FILDEF void action_new_area ()
    {
        if (save_changes_prompt() != ALERT_RESULT_CANCEL) {
            create_blank_area();
        }
    }
    FILDEF void action_open_area ()
    {
        std::string file_name(open_dialog());
        if(file_name.empty()) { return; }

        if (save_changes_prompt() != ALERT_RESULT_CANCEL) {
            load_area(current_area, file_name.c_str());
            current_area.file_name = file_name;
            unsaved_changes = false;

            history.current_position = -1;
        }
    }
    FILDEF void action_save_area ()
    {
        le_save_area();
    }
    FILDEF void action_save_as_area ()
    {
        le_save_as_area();
    }
    FILDEF void action_undo ()
    {
        // There is no history or we are already at the beginning.
        if (history.current_position <= -1) { return; }

        bool tile_state_empty = false;

        History_State& state = get_current_history_state();
        switch (state.action) {
        // case (HISTORY_ACTION_FLIP_LEVEL_H): { internal__flip_level_h(state.tile_layer_active);                       } break;
        // case (HISTORY_ACTION_FLIP_LEVEL_V): { internal__flip_level_v(state.tile_layer_active);                       } break;
        // case (HISTORY_ACTION_SELECT_STATE): { internal__restore_select_state(state.old_select_state);                } break;
        // case (HISTORY_ACTION_RESIZE):       { internal__resize(state.resize_dir, state.old_width, state.old_height); } break;
        case (HISTORY_ACTION_TILE): {
            // We check if the normal state we're undoing is empty or not. If it is
            // then we mark it as such and then if there is another state before it
            // we undo that one as well. This just feels a nicer than not doing it.
            if (state.tile_info.empty()) { tile_state_empty = true; }

            for (auto& i: state.tile_info) {
                int pos = i.y * current_area.header.width + i.x;
                current_area.tiles[i.layer][pos].id = i.old_id;
            }

            // if (state.action == HISTORY_ACTION_CLEAR) {
            //     internal__restore_select_state(state.old_select_state);
            // }
        } break;
        }

        if (history.current_position > -1) {
            --history.current_position;

            // We only want to do this part if there is another state to undo.
            if (state.action == HISTORY_ACTION_TILE && tile_state_empty) { action_undo(); }
        }

        // if (state.action != HISTORY_ACTION_SELECT_STATE) {
             unsaved_changes = true;
        // }
    }
    FILDEF void action_redo ()
    {
        // There is no history or we are already at the end.
        if (history.current_position >= CAST(int, history.state.size()-1)) { return; }

        ++history.current_position;

        History_State& state = get_current_history_state();
        switch (state.action) {
        // case (HISTORY_ACTION_FLIP_LEVEL_H): { internal__flip_level_h(state.tile_layer_active);                       } break;
        // case (HISTORY_ACTION_FLIP_LEVEL_V): { internal__flip_level_v(state.tile_layer_active);                       } break;
        // case (HISTORY_ACTION_SELECT_STATE): { internal__restore_select_state(state.new_select_state);                } break;
        // case (HISTORY_ACTION_RESIZE):       { internal__resize(state.resize_dir, state.new_width, state.new_height); } break;
        case (HISTORY_ACTION_TILE): {
            for (auto& i: state.tile_info) {
                int pos = i.y * current_area.header.width + i.x;
                current_area.tiles[i.layer][pos].id = i.new_id;
            }

            // if (state.action == HISTORY_ACTION_CLEAR) {
            //     internal__restore_select_state(state.new_select_state);
            // }
        } break;
        }

        // If we end on an empty normal state and we are not already at the end of
        // the redo history then we redo again as it feels nicer. This action is
        // the inverse of what we do when we do an undo with blank normal actions.
        if (history.current_position+1 < CAST(int, history.state.size())) {
            // Jump forward to see if it is empty, if it's not then revert back.
            ++history.current_position;
            History_State& next_state = get_current_history_state();

            if (next_state.action != HISTORY_ACTION_TILE || !next_state.tile_info.empty()) {
                --history.current_position;
            }
        }

        // if (state.action != HISTORY_ACTION_SELECT_STATE) {
             unsaved_changes = true;
        // }
    }
    FILDEF void action_history_begin ()
    {
        while (history.current_position > -1) { action_undo(); }
        unsaved_changes = true;
    }
    FILDEF void action_history_end ()
    {
        int maximum = CAST(int, history.state.size()-1);
        while (history.current_position < maximum) { action_redo(); }
        unsaved_changes = true;
    }
    FILDEF void action_tool_brush ()
    {
        /*
        if (tool_type == TOOL_TYPE_SELECT && tool_state == TOOL_STATE_PLACE) {
            if (!tool_info.select.bounds.empty()) {
                new_history_state(HISTORY_ACTION_SELECT_STATE);
            }
        }
        */

        if (tool_type != TOOL_TYPE_BRUSH && tool_state != TOOL_STATE_IDLE) {
            new_history_state(HISTORY_ACTION_TILE);
        }

        tool_type = TOOL_TYPE_BRUSH;
    }
    FILDEF void action_tool_fill ()
    {
        /*
        if (tool_type == TOOL_TYPE_SELECT && tool_state == TOOL_STATE_PLACE) {
            if (!tool_info.select.bounds.empty()) {
                new_history_state(HISTORY_ACTION_SELECT_STATE);
            }
        }
        */

        if (tool_type != TOOL_TYPE_FILL && tool_state != TOOL_STATE_IDLE) {
            new_history_state(HISTORY_ACTION_TILE);
        }

        tool_type = TOOL_TYPE_FILL;
    }
    FILDEF void action_tool_select ()
    {
        if (tool_type != TOOL_TYPE_SELECT && tool_state != TOOL_STATE_IDLE) {
            new_history_state(HISTORY_ACTION_TILE);
        }
        if (tool_state == TOOL_STATE_PLACE) {
            tool_info.select.start = true;
        }

        tool_type = TOOL_TYPE_SELECT;
    }
    FILDEF void action_resize ()
    {
        // @Incomplete: ...
    }
    FILDEF void action_grid ()
    {
        grid_visible = !grid_visible;
    }
    FILDEF void action_pack ()
    {
        // @Incomplete: ...
    }
    FILDEF void action_unpack ()
    {
        // @Incomplete: ...
    }
    FILDEF void action_reset_camera ()
    {
        camera_pos = Vec2(0.0f, 0.0f);
        camera_zoom = 1.0f;
    }
    FILDEF void action_zoom_in ()
    {
        if ((camera_zoom *= 2.0f) > MAX_EDITOR_ZOOM) {
            camera_zoom = MAX_EDITOR_ZOOM;
        }
    }
    FILDEF void action_zoom_out ()
    {
        if ((camera_zoom /= 2.0f) < MIN_EDITOR_ZOOM) {
            camera_zoom = MIN_EDITOR_ZOOM;
        }
    }
    FILDEF void action_set_view ()
    {
        if      (current_editor_view == EDITOR_VIEW_3D) { current_editor_view = EDITOR_VIEW_2D; }
        else if (current_editor_view == EDITOR_VIEW_2D) { current_editor_view = EDITOR_VIEW_3D; }
    }

    FILDEF void do_tool_panel ()
    {
        Vec2 cursor(TOOL_PANEL_INNER_PAD, TOOL_PANEL_INNER_PAD);

        begin_panel(get_viewport().w - control_panel_width - TOOL_PANEL_WIDTH - 1.0f, 0.0f, TOOL_PANEL_WIDTH, get_viewport().h, UI_NONE, UI_COLOR_MEDIUM);

        set_panel_cursor(&cursor);
        set_panel_cursor_dir(UI_DIR_DOWN);

        float bw = TOOL_PANEL_WIDTH - (TOOL_PANEL_INNER_PAD * 2.0f);
        float bh = TOOL_PANEL_WIDTH;

        UI_Flag blnk_flags = UI_NONE;
        UI_Flag open_flags = UI_NONE;
        UI_Flag save_flags = UI_NONE;
        UI_Flag svas_flags = UI_NONE;
        UI_Flag undo_flags = UI_NONE;
        UI_Flag redo_flags = UI_NONE;
        UI_Flag brsh_flags = (tool_type == TOOL_TYPE_BRUSH)  ? UI_NONE : UI_INACTIVE;
        UI_Flag fill_flags = (tool_type == TOOL_TYPE_FILL)   ? UI_NONE : UI_INACTIVE;
        UI_Flag sbox_flags = (tool_type == TOOL_TYPE_SELECT) ? UI_NONE : UI_INACTIVE;
        UI_Flag size_flags = UI_NONE;
        UI_Flag grid_flags = (grid_visible) ? UI_NONE : UI_INACTIVE;
        UI_Flag pack_flags = UI_NONE;
        UI_Flag upak_flags = UI_NONE;
        UI_Flag rcam_flags = UI_NONE;
        UI_Flag maga_flags = (camera_zoom == MAX_EDITOR_ZOOM) ? UI_LOCKED : UI_NONE;
        UI_Flag mags_flags = (camera_zoom == MIN_EDITOR_ZOOM) ? UI_LOCKED : UI_NONE;
        UI_Flag view_flags = UI_NONE;

        if (current_editor_panel != EDITOR_PANEL_TILES) {
            brsh_flags = UI_LOCKED;
            fill_flags = UI_LOCKED;
            sbox_flags = UI_LOCKED;
        }

        if (history.state.empty()) {
            undo_flags = UI_LOCKED;
            redo_flags = UI_LOCKED;
        }
        if (history.current_position <= -1) {
            undo_flags = UI_LOCKED;
        }
        if (history.current_position == CAST(int, history.state.size())-1) {
            redo_flags = UI_LOCKED;
        }

        std::string view_img = (current_editor_view == EDITOR_VIEW_3D) ? "ICONVP2D" : "ICONVP3D";

        do_button_img(action_new_area,     bw,bh, blnk_flags, "ICONBLNK");
        do_button_img(action_open_area,    bw,bh, open_flags, "ICONOPEN");
        do_button_img(action_save_area,    bw,bh, save_flags, "ICONSAVE");
        do_button_img(action_save_as_area, bw,bh, svas_flags, "ICONSVAS");
        do_button_img(action_undo,         bw,bh, undo_flags, "ICONUNDO");
        do_button_img(action_redo,         bw,bh, redo_flags, "ICONREDO");
        do_button_img(action_tool_brush,   bw,bh, brsh_flags, "ICONBRSH");
        do_button_img(action_tool_fill,    bw,bh, fill_flags, "ICONFILL");
        do_button_img(action_tool_select,  bw,bh, sbox_flags, "ICONSBOX");
        do_button_img(action_resize,       bw,bh, size_flags, "ICONSIZE");
        do_button_img(action_grid,         bw,bh, grid_flags, "ICONGRID");
        /*
        do_button_img(action_pack,         bw,bh, pack_flags, "ICONPACK");
        do_button_img(action_unpack,       bw,bh, upak_flags, "ICONUPAK");
        */
        do_button_img(action_reset_camera, bw,bh, rcam_flags, "ICONRCAM");
        do_button_img(action_zoom_in,      bw,bh, maga_flags, "ICONMAGA");
        do_button_img(action_zoom_out,     bw,bh, mags_flags, "ICONMAGS");
        do_button_img(action_set_view,     bw,bh, view_flags,  view_img );

        end_panel();
    }

    FILDEF bool are_any_select_boxes_visible(); // Predefine.
    FILDEF void get_total_select_boundary(int* _l, int* _t, int* _r, int *_b); // Predefine.

    FILDEF void do_information_panel ()
    {
        /*
        // If there is a message that has been pushed we display that, if
        // not then we just display a default message for the status bar.
        const char* message = INFORMATION_PANEL_DEFAULT_MSG;
        if (!status_bar_pushed_message.empty()) {
            message = status_bar_pushed_message.c_str();
        }
        */

        Vec2 cursor(INFORMATION_PANEL_INNER_PAD, 0.0f);

        float x = 0.0f;
        float y = get_viewport().h-INFORMATION_PANEL_HEIGHT;
        float w = get_viewport().w-control_panel_width - TOOL_PANEL_WIDTH - 2.0f;
        float h = INFORMATION_PANEL_HEIGHT;

        float info_panel_width = w - (INFORMATION_PANEL_INNER_PAD * 2.0f);
        float advance = INFORMATION_PANEL_INNER_PAD;

        float l2_w = roundf(info_panel_width * INFORMATION_PANEL_LABEL_WIDTH); // Mouse.
        float l3_w = roundf(info_panel_width * INFORMATION_PANEL_LABEL_WIDTH); // Select.

        // Get the mouse position.
        int mx = 0, my = 0;
        if (get_window("WINMAIN").focus) {
            if (point_in_bounds_xywh(get_mouse_pos(), level_editor_viewport)) {
                mx = CAST(int, level_editor_mouse_tile.x);
                my = CAST(int, level_editor_mouse_tile.y);
            }
        }

        // Get the select bounds.
        int sx = 0, sy = 0, sw = 0, sh = 0;
        if (are_any_select_boxes_visible()) {
            int l,t,r,b;
            get_total_select_boundary(&l,&t,&r,&b);
            sx = l, sy = b, sw = (r-l)+1, sh = (t-b)+1;
        }

        std::string mouse_str = format_string("Position (%d,%d)", mx,my);
        std::string select_str = format_string("Selection (%d,%d,%d,%d)", sx,sy,sw,sh);

        // We ensure that the mouse and select labels are always big enough to
        // show their entire content and they take priority over the tool-tip.
        float l2_tw = get_text_width_scaled(editor_catalog.GET_TRUETYPE_RESOURCE(EDITOR_REGULAR_FONT), mouse_str.c_str());
        if (l2_w < l2_tw) { l2_w = l2_tw; }
        float l3_tw = get_text_width_scaled(editor_catalog.GET_TRUETYPE_RESOURCE(EDITOR_REGULAR_FONT), select_str.c_str());
        if (l3_w < l3_tw) { l3_w = l3_tw; }

        // Now we can calculate how much space is left for the tool-tip label.
        float l1_w = (info_panel_width - (l2_w + l3_w)) - (advance * 2.0f);

        begin_panel(x, y, w, h, UI_NONE, UI_COLOR_MEDIUM);

        set_panel_cursor(&cursor);
        set_panel_cursor_dir(UI_DIR_RIGHT);

        std::string area_file_name = current_area.file_name;
        if (area_file_name.empty()) { area_file_name = "[UNTITLED]"; }

        std::string message(format_string("%s %s", area_file_name.c_str(), (unsaved_changes) ? "*" : ""));

        do_label(UI_ALIGN_LEFT,  UI_ALIGN_CENTER, l1_w, h, message.c_str(),    EDITOR_REGULAR_FONT);
        advance_panel_cursor(INFORMATION_PANEL_INNER_PAD);
        do_label(UI_ALIGN_RIGHT, UI_ALIGN_CENTER, l2_w, h, mouse_str.c_str(),  EDITOR_REGULAR_FONT);
        advance_panel_cursor(INFORMATION_PANEL_INNER_PAD);
        do_label(UI_ALIGN_RIGHT, UI_ALIGN_CENTER, l3_w, h, select_str.c_str(), EDITOR_REGULAR_FONT);

        end_panel();
    }

    FILDEF bool mouse_inside_level_editor_viewport ()
    {
        Vec2 m = level_editor_mouse;
        Quad v = level_editor_viewport;

        return ((m.x>=v.x) && (m.y>=v.y) && (m.x<=(v.x+v.w)) && (m.y<=(v.y+v.h)));
    }

    FILDEF Vec2 mouse_to_tile_position ()
    {
        // Only continue calculating the tile position if the mouse is in bounds.
        if (!mouse_inside_level_editor_viewport()) { return Vec2(-1.0f, -1.0f); }

        Vec2 m = level_editor_mouse_world;

        m.x = floorf((m.x - level_editor_bounds.x) / DEFAULT_TILE_SIZE);
        m.y = floorf((m.y - level_editor_bounds.y) / DEFAULT_TILE_SIZE);

        return m;
    }
    FILDEF Vec2 mouse_to_thing_position ()
    {
        // @Improve: Doesn't feel very nice right now!!!
        // @Improve: Doesn't feel very nice right now!!!
        // @Improve: Doesn't feel very nice right now!!!
        // @Improve: Doesn't feel very nice right now!!!

        // Only continue calculating the thing position if the mouse is in bounds.
        if (!mouse_inside_level_editor_viewport()) { return Vec2(-1.0f, -1.0f); }

        Vec2 m = level_editor_mouse_world;

        m.x = floorf((m.x - level_editor_bounds.x) / (DEFAULT_TILE_SIZE/SUB_TILE_SIZE));
        m.y = floorf((m.y - level_editor_bounds.y) / (DEFAULT_TILE_SIZE/SUB_TILE_SIZE));

        m.x /= SUB_TILE_SIZE;
        m.y /= SUB_TILE_SIZE;

        return m;
    }

    FILDEF void push_level_editor_camera_transform ()
    {
        float hw = get_viewport().w / 2.0f;
        float hh = get_viewport().h / 2.0f;

        float hsw = (get_viewport().w / camera_zoom) / 2.0f;
        float hsh = (get_viewport().h / camera_zoom) / 2.0f;

        float l = hw - hsw;
        float r = hw + hsw;
        float b = hh + hsh;
        float t = hh - hsh;

        set_projection(glm::ortho(l, r, b, t));
        set_view(glm::translate(Mat4(1), Vec3(camera_pos.x, camera_pos.y, 0.0f)));
    }
    FILDEF void pop_level_editor_camera_transform ()
    {
        set_view(Mat4(1));
    }

    FILDEF bool tile_in_bounds (int _x, int _y)
    {
        int w = current_area.header.width;
        int h = current_area.header.height;

        return (_x >= 0 && _x < w && _y >= 0 && _y < h);
    }
    FILDEF bool thing_in_bounds (float _x, float _y)
    {
        float w = CAST(float, current_area.header.width);
        float h = CAST(float, current_area.header.height);

        return (_x >= 0 && _x < w && _y >= 0 && _y < h);
    }

    FILDEF void place_tile (int _x, int _y, Tile_ID _id, int _layer)
    {
        // @Incomplete: Not storing tile properties within the history.. should we???

        if (!tile_layer_active[_layer]) { return; }
        if (!tile_in_bounds(_x, _y)) { return; }

        auto& layer = current_area.tiles.at(_layer);

        History_Tile_Info info = {};
        info.x                 = _x;
        info.y                 = _y;
        info.old_id            = layer[_y * current_area.header.width + _x].id;
        info.new_id            = _id;
        info.layer             = _layer;
        add_to_history_tile_state(info);

        // @Incomplete: Doesn't handle other properties or do history stuff...
        layer.at(_y * current_area.header.width + _x).id = _id;
        unsaved_changes = true;
    }
    FILDEF void place_thing (float _x, float _y, Thing_ID _id)
    {
        if (!thing_layer_active) { return; }
        if (!thing_in_bounds(_x, _y)) { return; }

        // @Incomplete: Handle history...
        // @Incomplete: Handle history...
        // @Incomplete: Handle history...

        Thing thing;
        thing.id = _id;
        thing.pos = Vec3(_x, 0.0f, _y);
        current_area.things.push_back(thing);

        #if 0
        printf("THING:\n");
        printf("  ID:  %d\n", thing.id);
        printf("  Pos: %f %f %f\n", thing.pos.x, thing.pos.y, thing.pos.z);
        #endif
    }

    FILDEF bool are_any_select_boxes_visible ()
    {
        for (auto& bounds: tool_info.select.bounds) {
            if (bounds.visible) { return true; }
        }
        return false;
    }
    FILDEF void get_ordered_select_bounds (const Select_Bounds& _bounds, int* _l, int* _t, int* _r, int* _b)
    {
        // We do this here rather than ordering it in the actual handle
        // select function because otherwise it would cause some issues.

        if (_l) { *_l = MIN(_bounds.left, _bounds.right);  }
        if (_r) { *_r = MAX(_bounds.left, _bounds.right);  }
        if (_b) { *_b = MIN(_bounds.top,  _bounds.bottom); }
        if (_t) { *_t = MAX(_bounds.top,  _bounds.bottom); }
    }
    FILDEF void get_total_select_boundary (int* _l, int* _t, int* _r, int *_b)
    {
        if (_l) { *_l = 0; }
        if (_t) { *_t = 0; }
        if (_r) { *_r = 0; }
        if (_b) { *_b = 0; }

        if (!are_any_select_boxes_visible()) { return; }

        int min_l = INT_MAX;
        int max_t = 0;
        int max_r = 0;
        int min_b = INT_MAX;

        for (auto& bounds: tool_info.select.bounds) {
            if (bounds.visible) {
                int l,t,r,b;
                get_ordered_select_bounds(bounds, &l,&t,&r,&b);

                min_l = MIN(min_l, l);
                max_t = MAX(max_t, t);
                max_r = MAX(max_r, r);
                min_b = MIN(min_b, b);
            }
        }

        if (_l) { *_l = min_l; }
        if (_t) { *_t = max_t; }
        if (_r) { *_r = max_r; }
        if (_b) { *_b = min_b; }
    }
    FILDEF void deselect ()
    {
        tool_info.select.bounds.clear();
    }
    FILDEF void select_all ()
    {
        // @Incomplete
        // tab.old_select_state = tool_info.select.bounds;

        tool_info.select.bounds.clear();
        tool_info.select.bounds.push_back(Select_Bounds());

        tool_info.select.bounds.back().left    = 0;
        tool_info.select.bounds.back().top     = 0;
        tool_info.select.bounds.back().right   = current_area.header.width-1;
        tool_info.select.bounds.back().bottom  = current_area.header.height-1;
        tool_info.select.bounds.back().visible = true;

        // @Incomplete
        // Add this selection to the history.
        // new_history_state(HISTORY_ACTION_SELECT_STATE);
    }
    FILDEF void clear_select ()
    {
        // @Incomplete: Not handling history...
        // @Incomplete: Not handling history...
        // @Incomplete: Not handling history...

        if (!are_any_select_boxes_visible()) { return; }

        new_history_state(HISTORY_ACTION_TILE);
        // new_history_state(HISTORY_ACTION_CLEAR);

        for (auto& bounds: tool_info.select.bounds) {
            if (bounds.visible) {
                int l, t, r, b;
                get_ordered_select_bounds(bounds, &l, &t, &r, &b);

                // Clear all of the tiles within the selection.
                for (int i=0; i<Area::TILE_LAYER_TOTAL; ++i) {
                    for (int y=b; y<=t; ++y) {
                        for (int x=l; x<=r; ++x) {
                            place_tile(x, y, 0, i);
                        }
                    }
                }
            }
        }

        // We also deselect the select box(es) afterwards, feels right.
        // tab.history.state.back().old_select_state = tab.tool_info.select.bounds;
        deselect();
        // tab.history.state.back().new_select_state = tab.tool_info.select.bounds;

        unsaved_changes = true;
    }

    FILDEF void handle_brush ()
    {
        if (selected_tile == NO_TILE_SELECTED) { return; }

        bool place = (tool_state == TOOL_STATE_PLACE);
        Vec2 t = mouse_to_tile_position();

        int x = CAST(int, t.x);
        int y = CAST(int, t.y);

        int layer = get_tile_info(selected_tile).type;

        place_tile(x, y, (place) ? selected_tile : 0, layer);
    }

    FILDEF Tile_ID get_fill_find_id (int _x, int _y, int _layer)
    {
        if (!tile_in_bounds(_x, _y)) { return 0; }

        const auto& layer = current_area.tiles.at(_layer);
        return layer.at(_y * current_area.header.width + _x).id;
    }
    FILDEF bool inside_select_bounds (int _x, int _y)
    {
        for (auto& bounds: tool_info.select.bounds) {
            if (bounds.visible) {
                int sl,st,sr,sb; get_ordered_select_bounds(bounds, &sl,&st,&sr,&sb);
                if (_x >= sl && _x <= sr && _y >= sb && _y <= st) { return true; }
            }
        }
        return false;
    }
    FILDEF void check_fill_neighbour (int _x, int _y)
    {
        // If the select box is visible then check if we should be filling inside
        // or outside of the select box bounds. Based on that case we discard any
        // tiles/spawns that do not fit in the bounds we are to be filling within.
        if (are_any_select_boxes_visible()) {
            if (tool_info.fill.inside_select) {
                if (!inside_select_bounds(_x, _y)) { return; }
            } else {
                if (inside_select_bounds(_x, _y)) { return; }
            }
        }

        // If we have already visited this space then we don't need to again.
        size_t index = _y * current_area.header.width + _x;
        if (tool_info.fill.searched.at(index)) { return; }

        if (get_fill_find_id(_x, _y, tool_info.fill.layer) == tool_info.fill.find_id) {
            if (tool_info.fill.layer == Area::TILE_LAYER_WALL || (tool_info.fill.layer != Area::TILE_LAYER_WALL && get_fill_find_id(_x, _y, Area::TILE_LAYER_WALL) == 0)) { // FLATS and CEILS do not overstep walls!
                place_tile(_x, _y, tool_info.fill.replace_id, tool_info.fill.layer);
                tool_info.fill.frontier.push_back({ CAST(float, _x), CAST(float, _y) });
            }
        }

        tool_info.fill.searched.at(index) = true; // Mark as searched!
    }

    FILDEF void replace ()
    {
        auto& layer = current_area.tiles.at(tool_info.fill.layer);
        for (int y=0; y<current_area.header.height; ++y) {
            for (int x=0; x<current_area.header.width; ++x) {
                // If the select box is visible then check if we should be replacing inside
                // or outside of the select box bounds. Based on that case we discard any
                // tiles/spawns that do not fit in the bounds we are to be replacing within.
                if (are_any_select_boxes_visible()) {
                    if (tool_info.fill.inside_select) {
                        if (!inside_select_bounds(x, y)) { continue; }
                    } else {
                        if (inside_select_bounds(x, y)) { continue; }
                    }
                }

                if (layer[y * current_area.header.width + x].id == tool_info.fill.find_id) {
                    place_tile(x, y, tool_info.fill.replace_id, tool_info.fill.layer);
                }
            }
        }
    }
    FILDEF void fill ()
    {
        int w = current_area.header.width;
        int h = current_area.header.height;

        tool_info.fill.searched.resize(w*h, false);

        int start_x = CAST(int, tool_info.fill.start.x);
        int start_y = CAST(int, tool_info.fill.start.y);

        // Start tile marked searched as we can just replace it now.
        place_tile(start_x, start_y, tool_info.fill.replace_id, tool_info.fill.layer);

        tool_info.fill.searched.at(start_y * w + start_x) = true;
        tool_info.fill.frontier.push_back(tool_info.fill.start);

        // Once the frontier is empty this means all tiles within the potentially
        // enclosed space have been filled and there is no where else to expand
        // to within the level. Meaning that the fill is complete and we can leave.
        while (tool_info.fill.frontier.size() > 0) {
            Vec2 temp = tool_info.fill.frontier.at(0);

            int cx = CAST(int, temp.x);
            int cy = CAST(int, temp.y);

            tool_info.fill.frontier.erase(tool_info.fill.frontier.begin());

            // Check the neighbors, but don't try to access outside level bounds.
            if (cy > 0)     { check_fill_neighbour(cx,   cy-1); }
            if (cx < (w-1)) { check_fill_neighbour(cx+1, cy  ); }
            if (cy < (h-1)) { check_fill_neighbour(cx,   cy+1); }
            if (cx > 0)     { check_fill_neighbour(cx-1, cy  ); }
        }

        tool_info.fill.searched.clear();
        tool_info.fill.frontier.clear();
    }
    FILDEF void handle_fill ()
    {
        if (selected_tile == NO_TILE_SELECTED) { return; }

        Vec2 tile_pos = mouse_to_tile_position();

        int x = CAST(int, tile_pos.x);
        int y = CAST(int, tile_pos.y);

        // Do not bother starting a fill if out of bounds!
        if (!tile_in_bounds(x, y)) { return; }

        bool place = (tool_state == TOOL_STATE_PLACE);
        Tile_ID id = (place) ? selected_tile : 0;

        tool_info.fill.start      = tile_pos;
        tool_info.fill.layer      = get_tile_info(selected_tile).type;
        tool_info.fill.find_id    = get_fill_find_id(x, y, tool_info.fill.layer);
        tool_info.fill.replace_id = id;

        #if 0
        printf("FILL:\n");
        printf("  Start:   %d %d\n", x, y);
        printf("  Layer:   %d\n",    tool_info.fill.layer);
        printf("  Find:    %d\n",    tool_info.fill.find_id);
        printf("  Replace: %d\n",    tool_info.fill.replace_id);
        #endif

        // If the IDs are the same there is no need to fill.
        if (tool_info.fill.find_id == tool_info.fill.replace_id) { return; }

        // Determine if the origin of the fill is inside a selection box or not.
        // This part does not matter if a selection box is not currently present.
        tool_info.fill.inside_select = false;
        for (auto& bounds: tool_info.select.bounds) {
            if (bounds.visible) {
                int sl,st,sr,sb; get_ordered_select_bounds(bounds, &sl,&st,&sr,&sb);
                if (x >= sl && x <= sr && y >= sb && y <= st) { tool_info.fill.inside_select = true; }
            }
        }

        // Determine if we are doing a fill or find/replace.
        if (SDL_GetModState()&KMOD_ALT) { replace(); }
        else                            { fill();    }
    }

    FILDEF void handle_select ()
    {
        // Right clicking whilst using the select tool remove the current selection.
        if (tool_state == TOOL_STATE_ERASE) { deselect(); return; }

        // We do this because otherwise if we have a selection box up and we click
        // outside of the level editor viewport region then it will try calculate
        // a new selection box -- instead we want it to do absolutely nothing.
        if (!mouse_inside_level_editor_viewport()) { return; }

        // If it is the start of a new selection then we do some extra stuff.
        if (tool_info.select.start) {
            // @Incomplete
            // Cache the old state so we can add it to the history for undo/redo.
            // tab.old_select_state = tab.tool_info.select.bounds;

            // Clear the old selection box(es) if we are not adding a new one.
            if (!tool_info.select.add) {
                tool_info.select.bounds.clear();
            }
            tool_info.select.bounds.push_back(Select_Bounds());

            Select_Bounds& select_bounds = tool_info.select.bounds.back();

            Vec2 ta = mouse_to_tile_position();
            // Set the starting anchor point of the selection (clamp in bounds).
            select_bounds.top  = std::clamp(CAST(int, ta.y), 0, current_area.header.height-1);
            select_bounds.left = std::clamp(CAST(int, ta.x), 0, current_area.header.width-1);

            tool_info.select.start = false;
            select_bounds.visible = false;
        }

        Select_Bounds& select_bounds = tool_info.select.bounds.back();

        // We do this initial check so that if the user started by dragging
        // their selection box from outside the editor bounds then it will
        // not start the actual selection until their mouse enters the bounds.
        if (!select_bounds.visible) {
            Vec2 a = mouse_to_tile_position();
            if (tile_in_bounds(CAST(int, a.x), CAST(int, a.y))) {
                select_bounds.visible = true;
            }
        }

        if (select_bounds.visible) {
            // Set the second point of the selection box bounds.
            Vec2 tb = mouse_to_tile_position();
            select_bounds.bottom = std::clamp(CAST(int, tb.y), 0, current_area.header.height-1);
            select_bounds.right  = std::clamp(CAST(int, tb.x), 0, current_area.header.width-1);
        }
    }

    FILDEF void handle_current_tool ()
    {
        if (current_editor_panel == EDITOR_PANEL_LEVEL) { return; }
        if (tool_state == TOOL_STATE_IDLE) { return; }

        switch (tool_type) {
        case (TOOL_TYPE_BRUSH):  { handle_brush();  } break;
        case (TOOL_TYPE_FILL):   { handle_fill();   } break;
        case (TOOL_TYPE_SELECT): { handle_select(); } break;
        }
    }

    FILDEF bool clipboard_empty ()
    {
        for (auto& cb: clipboard) {
            for (auto& layer: cb.tiles) {
                if (!layer.empty()) { return false; }
            }
        }
        return true;
    }

    FILDEF void tile_copy ()
    {
        if (are_any_select_boxes_visible()) {
            // Clear the old clipboard content now we know we can actually copy.
            clipboard.clear();

            int sl = 0;
            int st = 0;
            int sr = 0;
            int sb = 0;

            get_total_select_boundary(&sl,&st,&sr,&sb);

            for (auto& bounds: tool_info.select.bounds) {
                if (bounds.visible) {
                    clipboard.push_back(Clipboard());
                    Clipboard& cb = clipboard.back();

                    int l, t, r, b;
                    get_ordered_select_bounds(bounds, &l, &t, &r, &b);

                    int w = (r-l)+1;
                    int h = (t-b)+1;

                    // Resize the clipboard tile buffer to be the new selection box size.
                    for (auto& layer: cb.tiles) { layer.assign(w*h, Tile()); }

                    // Important to cache so we can use during paste.
                    cb.x = l - sl;
                    cb.y = t - st;
                    cb.w = w;
                    cb.h = h;

                    // Copy the selected tiles into the buffer.
                    for (size_t i=0; i<cb.tiles.size(); ++i) {
                        if (!tile_layer_active[i]) { continue; }

                        const auto& src_layer = current_area.tiles[i];
                        auto& dst_layer = cb.tiles[i];
                        for (int y=b; y<=t; ++y) {
                            for (int x=l; x<=r; ++x) {
                                dst_layer[(y-b) * w + (x-l)] = src_layer[y * current_area.header.width + x];
                            }
                        }
                    }
                }
            }
        }
    }
    FILDEF void tile_paste ()
    {
        // @Incomplete: We are only copying IDs and not properties, which we would want to do when copying/cutting/pasting!

        if (clipboard_empty()) { return; }

        Vec2 tile_pos = level_editor_mouse_tile;
        new_history_state(HISTORY_ACTION_TILE);

        for (auto& cb: clipboard) {
            int x = CAST(int, tile_pos.x) + cb.x;
            int y = CAST(int, tile_pos.y) + cb.y;
            int w = cb.w;
            int h = cb.h;

            // Paste all of the clipboard tiles.
            for (size_t i=0; i<cb.tiles.size(); ++i) {
                const auto& src_layer = cb.tiles[i];
                for (int iy=y; iy<(y+h); ++iy) {
                    for (int ix=x; ix<(x+w); ++ix) {
                        place_tile(ix, iy, src_layer[(iy-y)*w+(ix-x)].id, CAST(int, i));
                    }
                }
            }
        }
    }

    FILDEF void copy ()
    {
        if (current_editor_panel == EDITOR_PANEL_TILES)
        {
            if (!are_any_select_boxes_visible()) { return; }

            tile_copy();
            deselect();
        }
        if (current_editor_panel == EDITOR_PANEL_THINGS)
        {
            // @Incomplete: ...
        }
    }
    FILDEF void cut ()
    {
        if (current_editor_panel == EDITOR_PANEL_TILES)
        {
            if (!are_any_select_boxes_visible()) { return; }

            tile_copy();
            clear_select();
        }
        if (current_editor_panel == EDITOR_PANEL_THINGS)
        {
            // @Incomplete: ...
        }

        unsaved_changes = true;
    }
    FILDEF void paste ()
    {
        if (current_editor_panel == EDITOR_PANEL_TILES) { tile_paste(); }
        if (current_editor_panel == EDITOR_PANEL_THINGS) { /* @Incomplete: ... */ }

        unsaved_changes = true;
    }

    GLOBAL const Vec4 GHOSTED_CURSOR_COLOR = { 0.2f, 0.9f, 0.2f, 0.4f };
    GLOBAL const float GHOSTED_CURSOR_ALPHA = 0.5f;

    FILDEF void draw_tile_cursor ()
    {
        if (selected_tile == NO_TILE_SELECTED) { return; }
        if (tool_type == TOOL_TYPE_SELECT) { return; }

        const Tile_Info& info = get_tile_info(selected_tile);

        Vec2 t = mouse_to_tile_position();

        float gx = level_editor_bounds.x + (t.x * DEFAULT_TILE_SIZE);
        float gy = level_editor_bounds.y + (t.y * DEFAULT_TILE_SIZE);

        /*
        set_draw_color(GHOSTED_CURSOR_COLOR);
        fill_quad(gx, gy, gx+DEFAULT_TILE_SIZE, gy+DEFAULT_TILE_SIZE);
        */

        Texture& tex = texture_catalog.GET_TEXTURE_RESOURCE(info.texture);
        tex.color.a = GHOSTED_CURSOR_ALPHA;
        draw_texture(tex, gx+(DEFAULT_TILE_SIZE / 2.0f), gy+(DEFAULT_TILE_SIZE / 2.0f));
        tex.color.a = 1.0f;
    }
    FILDEF void draw_thing_cursor ()
    {
        // @Incomplete: ...
    }

    FILDEF void draw_tile_clipboard ()
    {
        begin_stencil();
        for (auto& cb: clipboard) {
            int lw = current_area.header.width-1;
            int lh = current_area.header.height-1;

            int sw = cb.w-1;
            int sh = cb.h-1;

            Vec2 t = mouse_to_tile_position();

            int tx = CAST(int, t.x) + cb.x;
            int ty = CAST(int, t.y) + cb.y;

            float gx = level_editor_bounds.x + (tx * DEFAULT_TILE_SIZE);
            float gy = level_editor_bounds.y + (ty * DEFAULT_TILE_SIZE);
            float gw = cb.w * DEFAULT_TILE_SIZE;
            float gh = cb.h * DEFAULT_TILE_SIZE;

            stencil_mode_draw();
            set_draw_color(GHOSTED_CURSOR_COLOR);
            fill_quad(gx, gy, gx+gw, gy+gh);

            stencil_mode_erase();
            set_draw_color(1.0f, 1.0f, 1.0f, 1.0f);
            fill_quad(gx, gy, gx+gw, gy+gh);
        }
        end_stencil();

        // Stops us from drawing multiple copies of a tile where clipboards overlap.
        std::array<std::map<size_t, bool>, Area::TILE_LAYER_TOTAL> tile_space_occupied;
        for (auto& cb: clipboard) {
            int lw = current_area.header.width-1;
            int lh = current_area.header.height-1;

            int sw = cb.w-1;
            int sh = cb.h-1;

            Vec2 t = mouse_to_tile_position();

            int x = CAST(int, t.x) + cb.x;
            int y = CAST(int, t.y) + cb.y;

            float gx = level_editor_bounds.x + (x * DEFAULT_TILE_SIZE);
            float gy = level_editor_bounds.y + (y * DEFAULT_TILE_SIZE);
            float gw = cb.w * DEFAULT_TILE_SIZE;
            float gh = cb.h * DEFAULT_TILE_SIZE;

            // Draw all of the select buffer tiles.
            for (size_t i=0; i<cb.tiles.size(); ++i) {
                // If the layer is not active then we do not bother drawing its clipboard content.
                if (!tile_layer_active[i]) { continue; }

                float tx = gx;
                float ty = gy;

                const auto& layer = cb.tiles[i];
                auto& layer_space_occupied = tile_space_occupied[i];

                for (size_t j=0; j<layer.size(); ++j) {
                    Tile_ID id = layer[j].id;
                    if (id != 0) { // No point drawing empty tiles...
                        if (!layer_space_occupied.count(j)) {
                            Texture& tex = texture_catalog.GET_TEXTURE_RESOURCE(get_tile_info(id).texture);
                            tex.color.a = GHOSTED_CURSOR_ALPHA;
                            draw_texture(tex, tx+(DEFAULT_TILE_SIZE / 2.0f), ty+(DEFAULT_TILE_SIZE / 2.0f));
                            tex.color.a = 1.0f;
                            layer_space_occupied.insert(std::pair<size_t, bool>(j, true));
                        }
                    }

                    tx += DEFAULT_TILE_SIZE;
                    if ((tx+DEFAULT_TILE_SIZE) > (gx+gw)) {
                        ty += DEFAULT_TILE_SIZE;
                        tx = gx;
                    }
                }
            }
        }
    }
    FILDEF void draw_thing_clipboard ()
    {
        // @Incomplete: ...
    }

    FILDEF void do_level_editor ()
    {
        // Fill tool is not allowed for the THINGS mode!!!
        // if (current_editor_panel == EDITOR_PANEL_THINGS) {
        //     if (tool_type == TOOL_TYPE_FILL) {
        //         tool_type = TOOL_TYPE_BRUSH;
        //     }
        // }

        begin_panel(0.0f, 0.0f, get_viewport().w-control_panel_width - TOOL_PANEL_WIDTH - 2.0f, get_viewport().h-INFORMATION_PANEL_HEIGHT-1.0f, UI_NONE);

        push_level_editor_camera_transform();
        level_editor_mouse_world = screen_to_world(get_mouse_pos());
        level_editor_mouse = get_mouse_pos();
        level_editor_mouse_tile = mouse_to_tile_position();
        pop_level_editor_camera_transform();

        level_editor_viewport = get_viewport();

        // If we're in the level editor viewport then the cursor can be one of
        // the custom tool cursors based on what our current tool currently is.
        if (mouse_inside_level_editor_viewport() && get_window("WINMAIN").focus) {
            if (current_editor_panel == EDITOR_PANEL_TILES) {
                switch (tool_type) {
                case (TOOL_TYPE_BRUSH):  { set_cursor("CURBRUSH"); } break;
                case (TOOL_TYPE_FILL):   { set_cursor("CURPAINT"); } break;
                case (TOOL_TYPE_SELECT): { set_cursor("CURCROSS"); } break;
                }
            } else {
                set_cursor("CURARROW");
            }
        } else {
            // We do this check so it doesn't mess with text box UI cursors!
            if (get_cursor() != "CURBEAM") { set_cursor("CURARROW"); }
        }

        push_level_editor_camera_transform();

        level_editor_bounds.w = current_area.header.width  * DEFAULT_TILE_SIZE;
        level_editor_bounds.h = current_area.header.height * DEFAULT_TILE_SIZE;
        level_editor_bounds.x = (get_viewport().w - level_editor_bounds.w) / 2.0f;
        level_editor_bounds.y = (get_viewport().h - level_editor_bounds.h) / 2.0f;

        float x = level_editor_bounds.x;
        float y = level_editor_bounds.y;
        float w = level_editor_bounds.w;
        float h = level_editor_bounds.h;

        float tile_scale = DEFAULT_TILE_SIZE / TILE_IMAGE_SIZE;
        set_texture_draw_scale(tile_scale, tile_scale);

        Vec2 le_bounds_a = world_to_screen({ x  , y   });
        Vec2 le_bounds_b = world_to_screen({ x+w, y+h });

        // Because we mess with the orthographic projection matrix a pixel is no
        // longer 1.0f so we need to adjust by the current zoom to get a pixel.
        //
        // We do this because otherwise the outer border gets scaled incorrectly
        // and looks quite ugly. This method ensures it always remains 1px thick.
        float px = (1.0f / camera_zoom);

        set_draw_color(UI_COLOR_BLACK);
        fill_quad(x-px, y-px, x+w+px, y+h+px);
        set_draw_color(UI_COLOR_LIGHT);
        fill_quad(x, y, x+w, y+h);

        // Scissor the content of the level editor region to avoid any overspill.
        float scx = floorf(le_bounds_a.x);
        float scy = floorf(le_bounds_a.y);
        float scw = ceilf (le_bounds_b.x - scx);
        float sch = ceilf (le_bounds_b.y - scy);

        begin_scissor(scx, scy, scw, sch);

        // DRAW TILES
        int selected_layer = (selected_tile == NO_TILE_SELECTED) ? INT_MAX : get_tile_info(selected_tile).type;
        size_t index = 0;
        for (auto layer: current_area.tiles) {
            if (tile_layer_active[index]) {
                for (int iy=0; iy<current_area.header.height; ++iy) {
                    for (int ix=0; ix<current_area.header.width; ++ix) {
                        Tile_ID id = layer.at(iy * current_area.header.width + ix).id;
                        if (id != 0) {
                            Tile_Info info = get_tile_info(id);

                            float tx = level_editor_bounds.x + (ix * DEFAULT_TILE_SIZE);
                            float ty = level_editor_bounds.y + (iy * DEFAULT_TILE_SIZE);

                            Texture& tex = texture_catalog.GET_TEXTURE_RESOURCE(info.texture);
                            tex.color.a = (index > selected_layer) ? 0.5f : 1.0f;
                            draw_texture(tex, tx+(DEFAULT_TILE_SIZE / 2.0f), ty+(DEFAULT_TILE_SIZE / 2.0f));
                            tex.color.a = 1.0f;
                        }
                    }
                }
            }
            ++index;
        }

        // DRAW THINGS
        if (thing_layer_active) {
            for (const auto& thing: current_area.things) {
                if (thing.id != 0) {
                    Thing_Info info = get_thing_info(thing.id);

                    float tx = level_editor_bounds.x + (thing.pos.x * DEFAULT_TILE_SIZE);
                    float ty = level_editor_bounds.y + (thing.pos.z * DEFAULT_TILE_SIZE);

                    Texture& tex = sprite_catalog.GET_TEXTURE_RESOURCE(info.sprite);
                    tex.color.a = (current_editor_panel == EDITOR_PANEL_TILES) ? 0.5f : 1.0f;
                    draw_texture(tex, tx, ty);
                    tex.color.a = 1.0f;
                }
            }
        }

        // Draw a ghosted version of the currently selected tile or thing.
        if (current_editor_panel == EDITOR_PANEL_TILES) {
            if (mouse_inside_level_editor_viewport()) {
                if (!clipboard_empty() && SDL_GetModState()&KMOD_CTRL) {
                    draw_tile_clipboard();
                } else {
                    draw_tile_cursor();
                }
            }
        }
        if (current_editor_panel == EDITOR_PANEL_THINGS) {
            draw_thing_cursor();
        }

        end_scissor();

        // Draw the selection box(es) if visible.
        begin_stencil();
        for (auto& bounds: tool_info.select.bounds) {
            if (bounds.visible) {
                int il, it, ir, ib;
                get_ordered_select_bounds(bounds, &il, &it, &ir, &ib);

                float l =       CAST(float, il);
                float r = ceilf(CAST(float, ir)+0.5f);
                float b =       CAST(float, ib);
                float t = ceilf(CAST(float, it)+0.5f);

                float sx1 = x   + (l     * DEFAULT_TILE_SIZE);
                float sy1 = y   + (b     * DEFAULT_TILE_SIZE);
                float sx2 = sx1 + ((r-l) * DEFAULT_TILE_SIZE);
                float sy2 = sy1 + ((t-b) * DEFAULT_TILE_SIZE);

                stencil_mode_draw();
                set_draw_color(0.9f, 0.0f, 1.0f, 0.25f);
                fill_quad(sx1, sy1, sx2, sy2);

                stencil_mode_erase();
                set_draw_color(1.0f, 1.0f, 1.0f, 1.0f);
                fill_quad(sx1, sy1, sx2, sy2);
            }
        }
        end_stencil();

        // Draw the tile/thing grid for the level editor.
        if (grid_visible) {
            if (current_editor_panel == EDITOR_PANEL_THINGS) {
                set_draw_color(UI_COLOR_MED_LIGHT);
                for (float ix=x+(DEFAULT_TILE_SIZE/SUB_TILE_SIZE); ix<(x+w); ix+=(DEFAULT_TILE_SIZE/SUB_TILE_SIZE)) { draw_line(ix, y, ix, y+h); }
                for (float iy=y+(DEFAULT_TILE_SIZE/SUB_TILE_SIZE); iy<(y+h); iy+=(DEFAULT_TILE_SIZE/SUB_TILE_SIZE)) { draw_line(x, iy, x+w, iy); }
            }
            if (current_editor_panel != EDITOR_PANEL_LEVEL) {
                set_draw_color(UI_COLOR_EX_DARK);
                for (float ix=x+DEFAULT_TILE_SIZE; ix<(x+w); ix+=DEFAULT_TILE_SIZE) { draw_line(ix, y, ix, y+h); }
                for (float iy=y+DEFAULT_TILE_SIZE; iy<(y+h); iy+=DEFAULT_TILE_SIZE) { draw_line(x, iy, x+w, iy); }
            }
        }

        // Draw the black outline surrounding the level editor content.
        //
        // We do it this way because due to some rounding issues, the scissored
        // content of the level editor sometimes bleeds out of the level editor
        // area by a single pixel and would overlap the black border. By using
        // this stencil method and drawing the black border at the end we stop
        // that issue from occurring -- creating a nicer looking editor border.
        begin_stencil();
        stencil_mode_erase();
        set_draw_color(1.0f, 1.0f, 1.0f, 1.0f);
        fill_quad(x, y, x+w, y+h);
        stencil_mode_draw();
        set_draw_color(UI_COLOR_BLACK);
        fill_quad(x-px, y-px, x+w+px, y+h+px);
        end_stencil();

        set_texture_draw_scale(1.0f, 1.0f);
        pop_level_editor_camera_transform();

        end_panel();
    }
}

using namespace internal;

FILDEF void init_editor ()
{
    begin_debug_section("Editor Initialization:");
    defer { end_debug_section(); };

    if (!create_window("WINCOLOR", "Color Picker", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,250,302, 0,0, SDL_WINDOW_HIDDEN)) {
        LOG_ERROR(ERR_MED, "Failed to create the color picker window!");
        return;
    }
    set_window_icon("WINCOLOR", "ICOCOLOR");

    editor_catalog.create("EDITOR");

    current_editor_panel = EDITOR_PANEL_LEVEL;

    ui_panels.count = 0;

    ui_hot_id = UI_INVALID_ID;
    ui_hit_id = UI_INVALID_ID;

    ui_active_text_box = UI_INVALID_ID;
    ui_hot_text_box    = UI_INVALID_ID;
    ui_text_box_cursor = std::string::npos;

    ui_text_box_tab_window_id = 0;
    ui_make_next_text_box_active = false;
    ui_tab_handled = false;

    ui_cursor_blink_timer = 0.0f;
    ui_cursor_visible = true;

    layer_panel_scroll_offset = 0.0f;

    // Calculate the content height for the layer panel.
    layer_panel_content_height  = LAYER_PANEL_BUTTON_H * (Area::TILE_LAYER_TOTAL + 1); // +1 for THING layer.
    layer_panel_content_height -= 1.0f; // We don't care about the last separator!

    layer_panel_panel_height = 0.0f;

    grid_visible = true;

    history.current_position = -1;

    #if 0
    // We check to see if we are in an area when first opening the
    // editor as that will be the level we probably want to edit.
    //
    // If there is no area then we just create an empty level.
    current_area = get_current_area();
    if (!current_area) {
        // @Incomplete: ...
    }
    #else
    internal::create_blank_area();
    #endif

    for (int i=0; i<Area::TILE_LAYER_TOTAL; ++i) {
        tile_layer_active[i] = true;
    }
    thing_layer_active = true;

    if (::internal::tile_info_map.empty()) { selected_tile = NO_TILE_SELECTED; }
    else { selected_tile = ::internal::tile_info_map.begin()->first; }

    if (::internal::thing_info_map.empty()) { selected_thing = NO_THING_SELECTED; }
    else { selected_thing = ::internal::thing_info_map.begin()->first; }

    /*
    get_window("WINMAIN").close_callback = []() {
        main_running = false;
        if (save_changes_prompt() == ALERT_RESULT_CANCEL) {
            show_window("WINMAIN");
            main_running = true;
        }
    };
    */

    editor_initialized = true;
}

FILDEF void quit_editor ()
{
    // Nothing...
}

FILDEF void do_editor ()
{
    // Handle user interface blinking text box cursor.
    ui_cursor_blink_timer -= dt;
    if (ui_cursor_blink_timer <= 0.0f) {
        ui_cursor_visible = !ui_cursor_visible;
        ui_cursor_blink_timer += UI_CURSOR_BLINK_INTERVAL;
    }

    set_viewport(0, 0, get_render_target_w(), get_render_target_h());

    set_render_mode(RENDER_MODE_2D);
    render_clear(UI_COLOR_EX_DARK);

    do_control_panel();
    do_tool_panel();
    do_information_panel();

    do_level_editor();
}

FILDEF void handle_editor_events ()
{
    handle_ui_events();

    push_level_editor_camera_transform();
    defer { pop_level_editor_camera_transform(); };

    switch (main_event.type) {
    // CAMERA ZOOM
    case (SDL_MOUSEWHEEL): {
        if (SDL_GetModState()&KMOD_ALT) {
            if      (main_event.wheel.y > 0) { camera_zoom += (EDITOR_ZOOM_INCREMENT * camera_zoom); } // Zoom in.
            else if (main_event.wheel.y < 0) { camera_zoom -= (EDITOR_ZOOM_INCREMENT * camera_zoom); } // Zoom out.

            // Make sure the editor camera zoom stays within reasonable boundaries.
            camera_zoom = std::clamp(camera_zoom, MIN_EDITOR_ZOOM, MAX_EDITOR_ZOOM);
        }
    } break;
    // CAMERA PAN
    case (SDL_MOUSEMOTION): {
        if (SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_MMASK) {
            camera_pos.x += CAST(float, main_event.motion.xrel) / camera_zoom;
            camera_pos.y += CAST(float, main_event.motion.yrel) / camera_zoom;
        }
        handle_current_tool();
    } break;
    // HOTKEYS
    case (SDL_KEYDOWN):
    case (SDL_KEYUP): {
        bool pressed = main_event.key.state;
        bool ctrl    =  SDL_GetModState()&  KMOD_CTRL;
        bool alt     =  SDL_GetModState()&  KMOD_ALT;
        bool shift   =  SDL_GetModState()&  KMOD_SHIFT;
        bool none    = (SDL_GetModState()&~(KMOD_NUM|KMOD_CAPS))==KMOD_NONE;
        if (pressed && !text_box_is_active()) { // Don't want to perform key bindings when typing in a text box.
            switch (main_event.key.keysym.sym) {
            case (SDLK_n     ): { if (ctrl)        { action_new_area     (); }                                                                                 } break;
            case (SDLK_o     ): { if (ctrl)        { action_open_area    (); }                                                                                 } break;
            case (SDLK_s     ): { if (ctrl&&shift) { action_save_as_area (); } else if (ctrl) { action_save_area(); } else if (none) { action_tool_select(); } } break;
            case (SDLK_z     ): { if (ctrl&&shift) { action_history_begin(); } else if (ctrl) { action_undo     (); }                                          } break;
            case (SDLK_y     ): { if (ctrl&&shift) { action_history_end  (); } else if (ctrl) { action_redo     (); }                                          } break;
            case (SDLK_b     ): { if (none)        { action_tool_brush   (); }                                                                                 } break;
            case (SDLK_f     ): { if (none)        { action_tool_fill    (); }                                                                                 } break;
            case (SDLK_r     ): { if (ctrl)        { action_resize       (); }                                                                                 } break;
            case (SDLK_g     ): { if (none)        { action_grid         (); }                                                                                 } break;
            case (SDLK_0     ): { if (alt )        { action_reset_camera (); }                                                                                 } break;
            case (SDLK_MINUS ): { if (alt )        { action_zoom_out     (); }                                                                                 } break;
            case (SDLK_EQUALS): { if (alt )        { action_zoom_in      (); }                                                                                 } break;
            case (SDLK_SPACE ): { if (none)        { action_set_view     (); }                                                                                 } break;
            // Swap current panel + toggle layers.
            case (SDLK_1     ): { if (ctrl) { tile_layer_active[Area::TILE_LAYER_FLAT] = !tile_layer_active[Area::TILE_LAYER_FLAT]; } else if (none) {             current_editor_panel = EDITOR_PANEL_LEVEL;  } } break;
            case (SDLK_2     ): { if (ctrl) { tile_layer_active[Area::TILE_LAYER_WALL] = !tile_layer_active[Area::TILE_LAYER_WALL]; } else if (none) {             current_editor_panel = EDITOR_PANEL_TILES;  } } break;
            case (SDLK_3     ): { if (ctrl) { tile_layer_active[Area::TILE_LAYER_CEIL] = !tile_layer_active[Area::TILE_LAYER_CEIL]; } else if (none) { deselect(); current_editor_panel = EDITOR_PANEL_THINGS; } } break;
            case (SDLK_4     ): { if (ctrl) { thing_layer_active                       = !thing_layer_active;                       }                                                                            } break;
            // Non-toolbar actions.
            case (SDLK_d     ): { if (ctrl)        { deselect            (); }                                                                                 } break;
            case (SDLK_a     ): { if (ctrl)        { select_all          (); }                                                                                 } break;
            case (SDLK_DELETE): { if (none)        { clear_select        (); }                                                                                 } break;
            case (SDLK_c     ): { if (ctrl)        { copy                (); }                                                                                 } break;
            case (SDLK_x     ): { if (ctrl)        { cut                 (); }                                                                                 } break;
            case (SDLK_v     ): { if (ctrl)        { paste               (); }                                                                                 } break;
            }
        }
        // Determine whether new select boxes should be added or not.
        tool_info.select.add = ctrl;
    } break;
    // HANDLE TOOLS
    case (SDL_MOUSEBUTTONDOWN):
    case (SDL_MOUSEBUTTONUP): {
        bool pressed = main_event.button.state == SDL_PRESSED;
        if (pressed && is_there_a_hit_ui_element()) { break; }

        // TILES
        if (current_editor_panel == EDITOR_PANEL_TILES)
        {
            if (main_event.button.button == SDL_BUTTON_LEFT)
            {
                if (pressed) {
                    tool_state = TOOL_STATE_PLACE;
                    // This will be the start of a new selection!
                    if (tool_type == TOOL_TYPE_SELECT) {
                        tool_info.select.start = true;
                        tool_info.select.cached_size = tool_info.select.bounds.size();
                    }
                    if (tool_type == TOOL_TYPE_BRUSH || tool_type == TOOL_TYPE_FILL) {
                        new_history_state(HISTORY_ACTION_TILE);
                    }
                    handle_current_tool();
                } else {
                    tool_state = TOOL_STATE_IDLE;
                    if (tool_type == TOOL_TYPE_SELECT) {
                        if (tool_info.select.bounds.size() > tool_info.select.cached_size) {
                            // @Incomplete
                            // new_history_state(HISTORY_ACTION_SELECT_STATE);
                            tool_info.select.cached_size = tool_info.select.bounds.size();
                        }
                    }
                }
            }
            else if (main_event.button.button == SDL_BUTTON_RIGHT)
            {
                if (pressed) {
                    tool_state = TOOL_STATE_ERASE;
                    if (tool_type == TOOL_TYPE_BRUSH || tool_type == TOOL_TYPE_FILL) {
                        new_history_state(HISTORY_ACTION_TILE);
                    }
                    // Handle the current tool immediately so that placing/erasing
                    // doesn't require the user to move the mouse for it to work.
                    handle_current_tool();
                } else {
                    tool_state = TOOL_STATE_IDLE;
                }
            }
        }
        //THINGS
        if (current_editor_panel == EDITOR_PANEL_THINGS)
        {
            if (main_event.button.button == SDL_BUTTON_LEFT) {
                if (pressed) {
                    Vec2 pos = mouse_to_thing_position();
                    place_thing(pos.x, pos.y, selected_thing);
                }
            }
        }
    } break;
    }
}

FILDEF bool is_editor_initialized ()
{
    return editor_initialized;
}

} // Editor
