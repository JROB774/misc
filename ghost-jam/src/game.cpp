namespace internal
{
    enum Game_Mode
    {
        GAME_MODE_GAME,
        GAME_MODE_EDITOR,
    };

    GLOBAL Game_Mode current_game_mode;

    FILDEF void lock_mouse () { SDL_SetRelativeMouseMode(SDL_TRUE); SDL_ShowCursor(SDL_DISABLE); hide_cursor(); }
    FILDEF void unlock_mouse () { SDL_SetRelativeMouseMode(SDL_FALSE); SDL_ShowCursor(SDL_DISABLE); show_cursor(); }

    FILDEF void set_game_mode_game ()
    {
        current_game_mode = GAME_MODE_GAME;
        lock_mouse();
        set_cursor("CURARROW");
    }

    FILDEF void set_game_mode_editor ()
    {
        if (!Editor::is_editor_initialized()) {
            Editor::init_editor();
        }

        current_game_mode = GAME_MODE_EDITOR;
        unlock_mouse();
    }

    /*Anim_State anim_test = {-1,0};*/

    FILDEF void do_game_mode_game ()
    {
        update_player(game.player);

        //Sound Update
        UpdateSoundPos(game.player.camera.pos);

        set_render_mode(RENDER_MODE_3D);
        enable_framebuffer();

        set_projection(glm::perspective(glm::radians(game.player.camera.fov), get_render_target_w()/get_render_target_h(), 0.001f, 1024.0f));
        set_view(get_player_view(game.player));

        // 3D STUFF
        render_clear(0.0f, 0.0f, 0.0f, 1.0f);
        draw_level(area_catalog.GET_LEVEL_RESOURCE("test"));

        // 2D STUFF
        set_render_mode(RENDER_MODE_2D);

/*        set_texture_draw_scale(10, 10);
        anim_test.time += dt;
        draw_texture_animated(anims_catalog.GET_ANIMS_RESOURCE("GIB"), sprite_catalog.GET_TEXTURE_RESOURCE("GIB"), 300, 200, anim_test);

        set_texture_draw_scale(1, 1);
        */
        // set_texture_draw_scale(2.0f, 2.0f);
        // draw_bitmap_text(font_catalog.GET_FONT_RESOURCE("FNTDBG"), 20, 20, "Pizza Time\nSpider-Man\t:3");

        disable_framebuffer();
        set_render_mode(RENDER_MODE_2D);

        // Magenta so we know if the framebuffer is filling the screen or not.
        render_clear(1.0f, 0.0f, 1.0f, 1.0f);
        draw_framebuffer();
    }

    FILDEF void do_game_mode_editor ()
    {
        Editor::do_editor();
    }
}

FILDEF void init_game ()
{
    internal::current_game_mode = internal::GAME_MODE_GAME;

    main_running = true;

    std::set_terminate([](){ LOG_ERROR(ERR_MAX, "Unhandled exception!"); });

    begin_debug_section("Initialization:");

    u32 sdl_flags = SDL_INIT_EVERYTHING;
    if (SDL_Init(sdl_flags) != 0) {
        LOG_ERROR(ERR_MAX, "Failed to initialize SDL! (%s)", SDL_GetError());
        return;
    }
    LOG_DEBUG("Initialized SDL2 Library");
    if (FT_Init_FreeType(&internal::freetype) != 0) {
        LOG_ERROR(ERR_MAX, "Failed to initialize FreeType!");
        return;
    }
    LOG_DEBUG("Initialized FreeType2 Library");

    // DUMP DEBUG INFO //////////////////////////////////////////////
    int num_display_modes = SDL_GetNumVideoDisplays();
    int num_video_drivers = SDL_GetNumVideoDrivers();

    LOG_DEBUG("Platform: %s", SDL_GetPlatform());
    if (num_display_modes > 0) {
        begin_debug_section("Displays:");
        for (int i=0; i<num_display_modes; ++i) {
            SDL_DisplayMode display_mode = {};
            if (SDL_GetCurrentDisplayMode(i, &display_mode) == 0) {
                const char* name = SDL_GetDisplayName(i);
                int w            = display_mode.w;
                int h            = display_mode.h;
                int hz           = display_mode.refresh_rate;
                LOG_DEBUG("(%d) %s %dx%d %dHz", i, name, w, h, hz);
            }
        }
        end_debug_section();
    }
    if (num_video_drivers > 0) {
        begin_debug_section("Drivers:");
        for (int i=0; i<num_video_drivers; ++i) {
            const char* name = SDL_GetVideoDriver(i);
            LOG_DEBUG("(%d) %s", i, name);
        }
        end_debug_section();
    }
    // DUMP DEBUG INFO //////////////////////////////////////////////

    if (!init_window()) { return; }
    if (!init_renderer()) { return; }

    get_window("WINMAIN").resize_callback = []() {
        resize_framebuffer(FRAMEBUFFER_W, FRAMEBUFFER_H);
    };

    generate_framebuffer(FRAMEBUFFER_W, FRAMEBUFFER_H);

    area_catalog   .create("AREAS");
    data_catalog   .create("DATA");
    effect_catalog .create("EFFECTS");
    font_catalog   .create("FONTS");
    shader_catalog .create("SHADERS");
    sky_catalog    .create("SKYS");
    sprite_catalog .create("SPRITES");
    texture_catalog.create("TEXTURES");
    anims_catalog.create("ANIMS");

    SDL_ShowCursor(SDL_DISABLE);

    init_cursors();
    init_tiles(); // @Temporary

    load_tile_data();
    load_thing_data();

    init_player(game.player);

    #if 0
    set_framebuffer_effect("BASIC");
    #else
    set_framebuffer_effect("CRUNCH");
    #endif

    end_debug_section();

    // @Temporary: Just load the skybox like this for now...
    #if 1
    load_skybox(skybox, "PUMPKIN");
    #else
    load_skybox(skybox, "DBGSKY");
    #endif

    setup_delta_timer();

    //JAMIE: COMMENTED OUT TO STOP IT FROM PISSING OFF JOSH
    init_openal();

    Sound_Source background_music;
    background_music.buffer_ptr = LoadSoundByID("SeaShanty");
    background_music.looping = true;
    background_music.global = true;
    background_music.gain = 0.1f;
    //PlaySound(background_music);
}

FILDEF void quit_game ()
{
    Editor::quit_editor();

    free_skybox(skybox);

    quit_cursors();

    destroy_framebuffer();

    quit_renderer();
    quit_window();

    quit_error_system();
    quit_debug_system();

    FT_Done_FreeType(internal::freetype);
    SDL_Quit();
}

//Sound Test
Sound_Source new_sound;

FILDEF void do_game ()
{
    while (main_running) {
        if (internal::current_game_mode == internal::GAME_MODE_EDITOR) {
            Editor::internal::reset_ui_state();
        }

        while (SDL_PollEvent(&main_event)) {
            switch (main_event.type) {
                // case (SDL_QUIT): { main_running = false; } break;
                case (SDL_KEYDOWN): {
                    switch (main_event.key.keysym.sym) {
                    // Just use ALT-F4 instead!
                    /*
                    case (SDLK_ESCAPE): {
                        main_running = false;
                    } break;
                    */
                    case (SDLK_F1): {
                        if (internal::current_game_mode == internal::GAME_MODE_GAME) {
                            internal::set_game_mode_editor();
                        } else {
                            internal::set_game_mode_game();
                        }
                    } break;
                    case (SDLK_F2): {
                        if (internal::current_game_mode == internal::GAME_MODE_GAME) {
                            if (SDL_GetRelativeMouseMode()) {
                                internal::unlock_mouse();
                            } else {
                                internal::lock_mouse();
                            }
                        }
                    } break;
                    //Sound Test
                    case (SDLK_F3): {
                        new_sound.buffer_ptr = LoadSoundByID("Feet");
                        PlaySound(new_sound);
                        break;
                    }

                    case (SDLK_F5): {
                        new_sound.buffer_ptr = LoadSoundByID("Goblin");
                        PlaySound(new_sound);
                        break;
                    }

                    }
                } break;
            }
            handle_window_events();
            if (internal::current_game_mode == internal::GAME_MODE_EDITOR) {
                Editor::handle_editor_events();
            }
        }

        if (internal::current_game_mode == internal::GAME_MODE_GAME) { internal::do_game_mode_game(); }
        if (internal::current_game_mode == internal::GAME_MODE_EDITOR) { internal::do_game_mode_editor(); }

        cap_current_frame();
        render_present();
        update_delta_time();
    }
}
