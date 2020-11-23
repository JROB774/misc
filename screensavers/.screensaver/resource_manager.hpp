#pragma once

FILDEF void  init_resource_manager ();
INLDEF void* get_resource (const char* file_name, size_t* size);

#define get_resource_string(file_name, size) CAST(const char*, get_resource(file_name, size))
#define get_resource_bytes( file_name, size) CAST(const u8  *, get_resource(file_name, size))
