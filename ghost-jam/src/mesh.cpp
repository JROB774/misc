STDDEF void create_mesh_from_data (Mesh& _mesh, const Vertex* _verts, size_t _count, bool _has_normal)
{
    glGenVertexArrays(1, &_mesh.vao);
    glGenBuffers(1, &_mesh.vbo);

    glBindVertexArray(_mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, _mesh.vbo);

    _mesh.verts.reserve(_count);
    _mesh.verts.assign(_verts, _verts+_count);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0*sizeof(GLfloat)));
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // TexCoord
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5*sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // Color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9*sizeof(GLfloat)));
    glEnableVertexAttribArray(3); // Normal

    /*
    _mesh.has_normal = _has_normal;
    if (_mesh.has_normal) {
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(12*sizeof(GLfloat)));
        glEnableVertexAttribArray(4); // Tangent
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(15*sizeof(GLfloat)));
        glEnableVertexAttribArray(5); // Bitangent

        for (size_t i=0; i<_mesh.verts.size(); i+=3) {
            Vec3 pos0 = _mesh.verts.at(i+0).pos;
            Vec3 pos1 = _mesh.verts.at(i+1).pos;
            Vec3 pos2 = _mesh.verts.at(i+2).pos;
            Vec2 uv0  = _mesh.verts.at(i+0).texcoord;
            Vec2 uv1  = _mesh.verts.at(i+1).texcoord;
            Vec2 uv2  = _mesh.verts.at(i+2).texcoord;

            Vec3 tangent;
            Vec3 bitangent;

            Vec3 edge0    = pos1 - pos0;
            Vec3 edge1    = pos2 - pos0;
            Vec2 deltaUV0 =  uv1 -  uv0;
            Vec2 deltaUV1 =  uv2 -  uv0;

            GLfloat f = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

            tangent.x = f * (deltaUV1.y * edge0.x - deltaUV0.y * edge1.x);
            tangent.y = f * (deltaUV1.y * edge0.y - deltaUV0.y * edge1.y);
            tangent.z = f * (deltaUV1.y * edge0.z - deltaUV0.y * edge1.z);
            tangent = glm::normalize(tangent);

            bitangent.x = f * (-deltaUV1.x * edge0.x + deltaUV0.x * edge1.x);
            bitangent.y = f * (-deltaUV1.x * edge0.y + deltaUV0.x * edge1.y);
            bitangent.z = f * (-deltaUV1.x * edge0.z + deltaUV0.x * edge1.z);
            bitangent = glm::normalize(bitangent);

            _mesh.verts.at(i+0).tangent = tangent;
            _mesh.verts.at(i+1).tangent = tangent;
            _mesh.verts.at(i+2).tangent = tangent;

            _mesh.verts.at(i+0).bitangent = bitangent;
            _mesh.verts.at(i+1).bitangent = bitangent;
            _mesh.verts.at(i+2).bitangent = bitangent;
        }
    }
    */

    glBufferData(GL_ARRAY_BUFFER, _count*sizeof(Vertex), &_mesh.verts[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

STDDEF void create_mesh_from_file (Mesh& _mesh, const char* _file_name)
{
    // @Incomplete: Not implemented yet...
    ASSERT(false);
}

FILDEF void free_mesh (Mesh& _mesh)
{
    glDeleteVertexArrays(1, &_mesh.vao);
    glDeleteBuffers(1, &_mesh.vbo);

    _mesh.verts.clear();
}
