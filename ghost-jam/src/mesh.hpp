struct Vertex
{
    Vec3 pos;
    Vec2 texcoord;
    Vec4 color;
    Vec3 normal;

    // Only used if the mesh has normal maps enabled.
    // Vec3 tangent;
    // Vec3 bitangent;
};

typedef GLuint VAO;
typedef GLuint VBO;

struct Mesh
{
    VAO vao;
    VBO vbo;

    std::vector<Vertex> verts;

    bool has_normal;
};

STDDEF void create_mesh_from_data (Mesh& _mesh, const Vertex* _verts, size_t _count, bool _has_normal = false);
STDDEF void create_mesh_from_file (Mesh& _mesh, const char* _file_name, bool _has_normal = false);

FILDEF void free_mesh (Mesh& _mesh);
