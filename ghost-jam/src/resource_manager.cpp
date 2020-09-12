#include "resource_types.cpp"

inline std::string Catalog::build_resource_path (std::string path_)
{
    if (resource_path_cached) { return (resource_path + path_); }

    std::string file_name(get_executable_path() + "resource_location.txt");
    if (!does_file_exist(file_name.c_str())) {
        resource_path_cached = true;
        return std::string();
    }

    char* relative_path = read_entire_file(file_name.c_str());
    defer { cstd_free(relative_path); };

    resource_path = get_executable_path() + relative_path;
    resource_path.erase(resource_path.find_last_not_of(" \t\n\r\f\v") + 1);

    resource_path_cached = true;

    return (resource_path + path_);
}

Catalog::~Catalog ()
{
    destroy();
}

/*
inline void Catalog::create_and_cache (std::string name_)
{
    create(name_);
    cache_all();
}
*/

void Catalog::create (std::string name_)
{
    absolute_path = build_resource_path(name_.c_str()) + "/";
    name = name_;

    LOG_DEBUG("Created Catalog %s", name.c_str());
}

void Catalog::destroy ()
{
    for (auto it: cached) {
        it.second->free();
        delete it.second;
    }
    cached.clear();
}

template<typename T>
T* Catalog::get_resource (std::string name_)
{
    if (cached.find(name_ + T::EXT) == cached.end()) {
        cache<T>(name_);
    }
    return dynamic_cast<T*>(cached[name_ + T::EXT]);
}

template<typename T>
void Catalog::cache (std::string name_)
{
    name_ += T::EXT;

    if (cached.find(name_) != cached.end()) { return; }

    std::string file_name(absolute_path + name_);
    T* resource = dynamic_cast<T*>(cached[name_]); // Will create an empty element.

    resource = new(T);
    resource->load(file_name);

    cached[name_] = resource;
}

/*
void Catalog::cache_all ()
{
    std::vector<std::string> files;
    list_path_files(absolute_path.c_str(), files);
    for (auto file: files) {
        cache(file);
    }

    LOG_DEBUG("Cached Catalog %s (%zd Files Cached)", name.c_str(), cached.size());
}
*/

template<typename T>
bool Catalog::exists (std::string name_)
{
    return get_resource<T>(name_);
}

std::string Catalog::build_path (std::string path_)
{
    return (absolute_path + path_);
}
