#define AL_BUFFER_SIZE 32768

struct Sound_Buffer
{
    std::string id;
    ALuint buffer;
};

struct Sound_Source
{ 
    ALuint* buffer_ptr = nullptr;

    float pitch = 1.0f;
    float gain = 1.0f;

    bool looping = false;

    Vec3 posistion = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};

    int channel = -1;
    bool playing = false;

    bool overwritable = true;
    bool global = false;

};

void init_openal();
void load_sound_from_file();
void LoadOGG(const char* filename, std::vector<char> &fluffer, ALenum &boremat, ALsizei &freq);

void CreateNewBuffer(std::string _id, std::string _path);
ALuint* LoadSoundByID(std::string _id);

void   LoadSound();
void   PlaySound(Sound_Source& _sound_source);
void DeleteSound();
void UpdateSoundPos(Vec3 _pos);