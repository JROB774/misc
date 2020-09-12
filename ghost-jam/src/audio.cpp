ALCdevice* device;
ALCcontext* context;

float curr[3] = {0.0f, 0.0f, -1.0f};

ALboolean loop = AL_FALSE;
ALuint sound_array[16];

bool sound_test_flag = false;

std::vector<Sound_Buffer> buffer_array;
std::vector<Sound_Source*>  sound_queue;


void init_openal()
{
    //ALsizei size; 
    /*
    ALsizei frequency;
    ALsizei frequency2;

    ALenum format;
    ALenum format2;

    std::vector<char> data;
    std::vector<char> data2;

    */

    //Creation of Device and Context
    device = alcOpenDevice(NULL); //Selecting the preferred device

    if(device){
        context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
    }

    CreateNewBuffer("Goblin", "resources\\SOUND\\Goblin.ogg");
    CreateNewBuffer("Feet", "resources\\SOUND\\footstep-mono.ogg");
    CreateNewBuffer("SeaShanty", "resources\\SOUND\\seashanty2.ogg");
/*
    //Generating a set of buffers for use
    alGenBuffers(1, &temp_buffer);
    alGenBuffers(1, &temp_buffer2);

    //Loading the OGG File <TEST>
    LoadOGG("resources\\SOUND\\footstep-mono.ogg", data, format, frequency);
    LoadOGG("resources\\SOUND\\Goblin.ogg", data2, format2, frequency2);

    alBufferData(temp_buffer, format, &data[0], static_cast<ALsizei>(data.size()), frequency);
    alBufferData(temp_buffer2, format2, &data2[0], static_cast<ALsizei>(data2.size()), frequency2);

*/
    //Generate Sourcer
    
    alGenSources(16, sound_array);

    //Setup the Listener Variables
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    float orient[6] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0, 0.f};
    alListenerfv(AL_ORIENTATION, orient); 

/*
    //Attach buffer to source
    alSourcei(al_source[0], AL_BUFFER, buffer_array.at(0).buffer);

    alSourcef( al_source[0], AL_PITCH, 1.0f);
    alSourcef( al_source[0], AL_GAIN, 1.0f);
    alSource3f(al_source[0], AL_POSITION, curr[0], curr[1], curr[2]);
    alSource3f(al_source[0], AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei( al_source[0], AL_LOOPING, AL_FALSE);

    //Playing Sound
    //ALint source_state;
    //alSourcePlay(al_source);

    //printf("Playing error code: %s\n",DEBUG__AL_ERROR_LOOKUP(alGetError()).c_str());

    //alGetSourcei(source, AL_SOURCE_STATE, &source_state);

/*
    while(source_state == AL_PLAYING){
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        printf("TRUE");
    }


    //Cleaning

    /*
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &fluffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);*/
}

void LoadOGG(const char* filename, std::vector<char> &buffer, ALenum &boremat, ALsizei &freq)
{
    int endian = 0;
    int bit_stream;
    long bytes;
    char array[AL_BUFFER_SIZE]; //32KB buffer

    //Reading File
    FILE *fp;
    fp = fopen(filename, "rb");

    if(fp == NULL){printf("FAILED TO OPEN OGG\n");}

    vorbis_info* p_info;
    OggVorbis_File ogg_file;

    ov_open(fp, &ogg_file, NULL, 0); //Closes the fp;

    //Getting some information from the OGG file
    p_info = ov_info(&ogg_file, -1);

    if(p_info->channels == 1){
        boremat = AL_FORMAT_MONO16;
    }else{
        boremat = AL_FORMAT_STEREO16;
    }

    freq = p_info->rate;

    do{
        //Reada up to a buffers worth of sound data
        bytes = ov_read(&ogg_file, array, AL_BUFFER_SIZE, endian, 2, 1, &bit_stream);
        //Append to end of buffer
        buffer.insert(buffer.end(), array, array + bytes);
    }while(bytes > 0);

    ov_clear(&ogg_file);

}

void CreateNewBuffer(std::string _id, std::string _path)
{
    for(int i = 0; i < buffer_array.size(); i++){
        if(_id == buffer_array.at(i).id){
            printf("Error Sound with same ID already exists\n");
            return;
        }
    }

    //initalise temporary values
    ALsizei frequency;
    ALenum format;
    std::vector<char> data;

    //Create the new struct
    Sound_Buffer new_buffer;
    new_buffer.id = _id;

    //Generate the new buffer
    alGenBuffers(1, &new_buffer.buffer);

    //Store the OGG file and assign it to the buffer
    LoadOGG(_path.c_str(), data, format, frequency);
    alBufferData(new_buffer.buffer, format, &data[0], static_cast<ALsizei>(data.size()), frequency);

    //Store buffer struct in the Vector
    buffer_array.push_back(new_buffer);

    //cleaning up
    data.clear();
}

void CreateNewSource()
{

}

ALuint* LoadSoundByID(std::string _id)
{
    for(int i = 0; i < buffer_array.size(); i++){
        if(_id == buffer_array.at(i).id){return &buffer_array.at(i).buffer;}
    }

    printf("Failed to find Buffer by this ID\n");
    return nullptr;
}

void PlaySound(Sound_Source& _sound_source)
{
    //Overwriting source that is already playing
    if(_sound_source.overwritable == true){
        if(_sound_source.channel != -1){
            int i = _sound_source.channel;

            alSourcef( sound_array[i], AL_PITCH, _sound_source.pitch);
            alSourcef( sound_array[i], AL_GAIN, _sound_source.gain);
            alSource3f(sound_array[i], AL_POSITION, _sound_source.posistion[0], _sound_source.posistion[1], _sound_source.posistion[2]);
            alSource3f(sound_array[i], AL_VELOCITY, _sound_source.velocity[0], _sound_source.velocity[1], _sound_source.velocity[2]);
            if(_sound_source.looping){alSourcei( sound_array[i], AL_LOOPING, AL_TRUE);}
            else{alSourcei( sound_array[i], AL_LOOPING, AL_FALSE);}

            //binding the buffer to the source
            alSourceStop(sound_array[i]);
            alSourcei(sound_array[i], AL_BUFFER, *_sound_source.buffer_ptr);
            alSourcePlay(sound_array[i]);

            printf("Overwrite on Channel [%i]\n", i);
            return;
        }
    }

    //If out of space cut the quitest sound and insert the louder audio
    //if audio is quiter than the ones being played ignore it.
    ALint source_state;

    for(int i = 0; i < 16; i++)
    {
        alGetSourcei(sound_array[i], AL_SOURCE_STATE, &source_state);

        if(source_state != AL_PLAYING){
            //Setting all the source settings
            alSourcef( sound_array[i], AL_PITCH, _sound_source.pitch);
            alSourcef( sound_array[i], AL_GAIN, _sound_source.gain);
            alSource3f(sound_array[i], AL_POSITION, _sound_source.posistion[0], _sound_source.posistion[1], _sound_source.posistion[2]);
            alSource3f(sound_array[i], AL_VELOCITY, _sound_source.velocity[0], _sound_source.velocity[1], _sound_source.velocity[2]);
            if(_sound_source.looping){alSourcei( sound_array[i], AL_LOOPING, AL_TRUE);}
            else{alSourcei( sound_array[i], AL_LOOPING, AL_FALSE);}

            //binding the buffer to the source
            alSourcei(sound_array[i], AL_BUFFER, *_sound_source.buffer_ptr);
            alSourcePlay(sound_array[i]);

            _sound_source.channel = i;

            printf("Playing on Channel [%i]\n", i);
            
            break;
        }

        //Work out calculating the quietest sound and overwrite it if the option is enabled to.
        if(i == 15){

        }
    }

    if(_sound_source.global == true){sound_queue.push_back(&_sound_source);}
}

void DeleteSound()
{

}

void UpdateSoundPos(Vec3 _pos)
{
    //printf("POS X: %f Y: %f: Z: %f \n", _pos.x, _pos.y, _pos.z);
    alListener3f(AL_POSITION, _pos.x, _pos.y, _pos.z);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    float orient[6] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0, 0.f};
    alListenerfv(AL_ORIENTATION, orient); 

    for(int i = 0; i < sound_queue.size(); i++){
        alSource3f(sound_array[sound_queue.at(i)->channel], AL_POSITION, _pos.x, _pos.y, _pos.z);
    }
}