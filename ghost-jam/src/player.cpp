namespace internal
{
    STDDEF void update_player_vectors (Player& player)
    {
        Vec3 front;
        front.x = cos(glm::radians(player.camera.pitch)) * cos(glm::radians(player.camera.yaw));
        front.y = sin(glm::radians(player.camera.pitch));
        front.z = cos(glm::radians(player.camera.pitch)) * sin(glm::radians(player.camera.yaw));

        player.camera.front = glm::normalize(front);
        player.camera.right = glm::normalize(glm::cross(player.camera.front, player.camera.world_up));
        player.camera.up    = glm::normalize(glm::cross(player.camera.right, player.camera.front));
    }
}

STDDEF void init_player (Player& player)
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
    hide_cursor();
    internal::update_player_vectors(player);
}

STDDEF void update_player (Player& player)
{
    const u8* key_state = SDL_GetKeyboardState(NULL);
    float speed = player.camera.speed * dt;

    bool sprint = key_state[SDL_SCANCODE_LSHIFT] || key_state[SDL_SCANCODE_RSHIFT];
    if (sprint) {
        speed *= 2.5f;
    }

    if (key_state[SDL_SCANCODE_W]) { player.camera.pos += player.camera.front * speed; }
    if (key_state[SDL_SCANCODE_S]) { player.camera.pos -= player.camera.front * speed; }
    if (key_state[SDL_SCANCODE_A]) { player.camera.pos -= player.camera.right * speed; }
    if (key_state[SDL_SCANCODE_D]) { player.camera.pos += player.camera.right * speed; }

    int relative_x, relative_y;
    SDL_GetRelativeMouseState(&relative_x, &relative_y);

    float mouse_x = (float)relative_x;
    float mouse_y = (float)relative_y;

    if (!SDL_GetRelativeMouseMode()) {
        mouse_x = 0.0f;
        mouse_y = 0.0f;
    }

    player.camera.yaw   += mouse_x * player.camera.sensitivity;
    player.camera.pitch -= mouse_y * player.camera.sensitivity;

    if (player.camera.pitch >  89.0f) { player.camera.pitch =  89.0f; }
    if (player.camera.pitch < -89.0f) { player.camera.pitch = -89.0f; }

    internal::update_player_vectors(player);
}

FILDEF Mat4 get_player_view (Player& player)
{
    return glm::lookAt(player.camera.pos, player.camera.pos + player.camera.front, player.camera.up);
}

FILDEF void set_player_pos (Player& player, int _x, int _y)
{
    player.camera.pos.x = (float)_x;
    player.camera.pos.z = (float)_y;
}

FILDEF void set_player_dir (Player& player, Player_Dir _dir)
{
    switch (_dir) {
    case(PLAYER_DIR_NORTH): { player.camera.yaw = -90.0f; } break;
    case(PLAYER_DIR_EAST): { player.camera.yaw = 0.0f; } break;
    case(PLAYER_DIR_SOUTH): { player.camera.yaw = 90.0f; } break;
    case(PLAYER_DIR_WEST): { player.camera.yaw = 180.0f; } break;
    }
}
