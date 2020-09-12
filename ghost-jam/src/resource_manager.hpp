#pragma once

struct Base_Resource
{
    virtual void load (std::string file_name_) = 0;
    virtual void free () = 0;
};

#include "resource_types.hpp"

struct Catalog
{
public:
    SHARED inline std::string build_resource_path (std::string path_);

   ~Catalog ();

    // inline void create_and_cache (std::string name_);

    void create (std::string name_);
    void destroy ();

    template<typename T>
    T* get_resource (std::string name_);

    template<typename T>
    void cache (std::string name_);
    // void cache_all ();

    template<typename T>
    bool exists (std::string name_);

    std::string build_path (std::string path_);

private:
    SHARED inline std::string resource_path = std::string();
    SHARED inline bool resource_path_cached = false;

    std::string absolute_path;
    std::string name;

    std::unordered_map<std::string, Base_Resource*> cached;
};
