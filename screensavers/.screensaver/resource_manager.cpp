#include "resources.cpp"

// @Improve: For now this is a fairly limited system and we're just using fixed size values.
//           In the future we're gonna wanna improve it as things get easier to implement.

GLOBAL constexpr int RESOURCE_ENTRY_COUNT = 32;

struct Resource_Entry
{
    const char* name   = NULL;
    u32         offset = 0;
    u32         size   = 0;
};

GLOBAL Resource_Entry resource_entries[RESOURCE_ENTRY_COUNT];

FILDEF void init_resource_manager ()
{
    // The part of the embedded PAK file we are currently reading from.
    const u8* cursor = RESOURCES;

    u32 file_count = *(CAST(const u32*, cursor));
    cursor += sizeof(u32);
    ASSERT(file_count <= RESOURCE_ENTRY_COUNT);

    // Read each of the resource file entries and cache the metadata.
    for (u32 i=0; i<file_count; ++i)
    {
        Resource_Entry& entry = resource_entries[i];

        entry.name = CAST(const char*, cursor);

        // Go until we hit the ending null-terminator.
        size_t name_length = 0;
        while (*(cursor++)) {}

        entry.offset = *(CAST(const u32*, cursor));
        cursor += sizeof(u32);
        entry.size   = *(CAST(const u32*, cursor));
        cursor += sizeof(u32);

        // #if defined(BUILD_DEBUG)
        // printf("%-32s (Size %.6d | Offset %.6d)\n",
        //     entry.name, entry.size, entry.offset);
        // #endif // BUILD_DEBUG
    }
}

INLDEF void* get_resource (const char* file_name, size_t* size)
{
    for (int i=0; i<RESOURCE_ENTRY_COUNT; ++i) {
        Resource_Entry& re = resource_entries[i];
        if ((re.name) && (cstr_equals(re.name, file_name))) {
            *size = re.size;
            return CAST(void*, RESOURCES + re.offset);
        }
    }
    return NULL;
}
