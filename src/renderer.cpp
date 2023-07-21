#include <corgi/opengl/renderer.h>
#include <glad/glad.h>

namespace corgi
{
void renderer::draw(const mesh& m) 
{
    m.vertex_array()->bind();

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m.indexes().size()),
                   GL_UNSIGNED_INT, (void*)0);

    m.vertex_array()->end();
}

}    // namespace corgi