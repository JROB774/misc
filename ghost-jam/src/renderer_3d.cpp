FILDEF void init_renderer_3d ()
{
    internal::projection_matrix = Mat4(1);
    internal::view_matrix       = Mat4(1);

    create_mesh_from_data(cube_mesh, CUBE_MESH, STATIC_ARRAY_COUNT(CUBE_MESH), true);
    create_mesh_from_data(plane_mesh, PLANE_MESH, STATIC_ARRAY_COUNT(PLANE_MESH), true);
    create_mesh_from_data(sprite_mesh, SPRITE_MESH, STATIC_ARRAY_COUNT(SPRITE_MESH), false);
}

FILDEF void quit_renderer_3d ()
{
    free_mesh(sprite_mesh);
    free_mesh(plane_mesh);
    free_mesh(cube_mesh);
}

FILDEF void enable_renderer_3d ()
{
    glEnable(GL_DEPTH_TEST);

    set_projection(glm::perspective(glm::radians(90.0f), get_render_target_w()/get_render_target_h(), 0.001f, 1024.0f));
    set_view(Mat4(1));
}

FILDEF void draw_mesh (const Mesh& _mesh, Mat4 _model)
{
    ASSERT(internal::render_mode == RENDER_MODE_3D);

    set_uniform_mat4("model", _model);

    glBindVertexArray(_mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, CAST(GLsizei, _mesh.verts.size()));
}
