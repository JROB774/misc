#pragma once

// Easier access and less writing for getting specific resources.
#define GET_LEVEL_RESOURCE(   name_) get_resource<Level_Resource   >(name_)->data
#define GET_TRUETYPE_RESOURCE(name_) get_resource<Truetype_Resource>(name_)->data
#define GET_SHADER_RESOURCE(  name_) get_resource<Shader_Resource  >(name_)->data
#define GET_TEXTURE_RESOURCE( name_) get_resource<Texture_Resource >(name_)->data
#define GET_FONT_RESOURCE(    name_) get_resource<Font_Resource    >(name_)->data
#define GET_ANIMS_RESOURCE(   name_) get_resource<Anims_Resource   >(name_)->data

#define LEVEL_EXISTS(    name_) exists<Level_Resource   >(name_)
#define TRUETYPE_EXISTS( name_) exists<Truetype_Resource>(name_)
#define SHADER_EXISTS(   name_) exists<Shader_Resource  >(name_)
#define TEXTURE_EXISTS(  name_) exists<Texture_Resource >(name_)
#define FONT_EXISTS(     name_) exists<Font_Resource    >(name_)
#define ANIMS_EXISTS(    name_) exists<Anims_Resource   >(name_)

struct Level_Resource: public Base_Resource
{
    static inline const std::string EXT = ".png";

    Level data;

    void load (std::string file_name_);
    void free ();
};
struct Truetype_Resource: public Base_Resource
{
    static inline const std::string EXT = ".ttf";

    Truetype data;

    void load (std::string file_name_);
    void free ();
};
struct Shader_Resource: public Base_Resource
{
    static inline const std::string EXT = ".shader";

    Shader data;

    void load (std::string file_name_);
    void free ();
};
struct Texture_Resource: public Base_Resource
{
    static inline const std::string EXT = ".png";

    Texture data;

    void load (std::string file_name_);
    void free ();
};
struct Font_Resource: public Base_Resource
{
    static inline const std::string EXT = ".gon";

    Font data;

    void load (std::string file_name_);
    void free ();
};
struct Anims_Resource: public Base_Resource
{
    static inline const std::string EXT = ".gon";

    Anims data;

    void load(std::string file_name_);
    void free();
};
