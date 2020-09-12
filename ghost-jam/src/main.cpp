#include "compiler.hpp"
#include "platform.hpp"

#include "external.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <ctime>
#include <cstdarg>

#include <sstream>

#include <vector>
#include <map>
#include <string>
#include <array>

#include <exception>
#include <type_traits>
#include <algorithm>

// @Temporary: Just conveneient for now as this is in multiple places...
#define FRAMEBUFFER_W (int)get_render_target_w()
#define FRAMEBUFFER_H (int)get_render_target_h()

#include "utility.hpp"
#include "version.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "time.hpp"
#include "window.hpp"
#include "cursor.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "truetype.hpp"
#include "mesh.hpp"
#include "mesh_data.hpp"
#include "framebuffer.hpp"
#include "font.hpp"
#include "skybox.hpp"
#include "level.hpp"
#include "area.hpp"
#include "anims.hpp"
#include "resource_manager.hpp"
#include "renderer.hpp"
#include "player.hpp"
    #include "audio.hpp"
#include "game.hpp"
#include "editor.hpp"

GLOBAL Skybox skybox; // @Temporary

#include "utility.cpp"
#include "debug.cpp"
#include "error.cpp"
#include "time.cpp"
#include "resource_manager.cpp"
#include "window.cpp"
#include "cursor.cpp"
#include "shader.cpp"
#include "texture.cpp"
#include "truetype.cpp"
#include "mesh.cpp"
#include "font.cpp"
#include "skybox.cpp"
#include "level.cpp"
#include "area.cpp"
#include "renderer.cpp"
#include "framebuffer.cpp"
#include "player.cpp"
    #include "audio.cpp"
#include "game.cpp"
#include "editor.cpp"
#include "anims.cpp"

int main (int _argc, char* _argv[])
{
    defer {
        quit_game();
    };

    try {
        init_game();
        do_game();
    } catch (const char* _msg) {
        LOG_ERROR(ERR_MAX, "%s", _msg);
    }

    return 0;
}
