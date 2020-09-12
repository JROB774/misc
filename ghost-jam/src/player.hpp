GLOBAL constexpr float YAW         = -90.0f;
GLOBAL constexpr float PITCH       =   0.0f;
GLOBAL constexpr float SPEED       =   4.0f;
GLOBAL constexpr float SENSITIVITY =   0.2f;
GLOBAL constexpr float FOV         =  90.0f;

struct Camera
{
    glm::vec3 pos      = glm::vec3(0.0f,  0.0f,  0.0f);
    glm::vec3 front    = glm::vec3(0.0f,  0.0f, -1.0f);
    glm::vec3 up       = glm::vec3(0.0f,  1.0f,  0.0f);
    glm::vec3 right    = glm::vec3();
    glm::vec3 world_up = glm::vec3(0.0f,  1.0f,  0.0f);

    float yaw          = YAW;
    float pitch        = PITCH;

    float speed        = SPEED;
    float sensitivity  = SENSITIVITY;
    float fov          = FOV;
};

enum Player_Dir
{
    PLAYER_DIR_NORTH,
    PLAYER_DIR_EAST,
    PLAYER_DIR_SOUTH,
    PLAYER_DIR_WEST,
};

struct Player
{
    Camera camera;
};

STDDEF void init_player (Player& player);
STDDEF void update_player (Player& player);

FILDEF Mat4 get_player_view (Player& player);

FILDEF void set_player_pos (Player& player, int _x, int _y);
FILDEF void set_player_dir (Player& player, Player_Dir _dir);
