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

    pipeline_->program_->end();
}

void renderer::set_pipeline(corgi::pipeline& pipeline)
{
    pipeline_ = &pipeline;

    if(!pipeline_)
        throw std::logic_error("renderer::draw(const mesh&) : No pipeline "
                               "attached to the renderer");

    if(!pipeline_->program_)
        throw std::logic_error("renderer::draw(const mesh&) : No program "
                               "attached to the pipeline");

    pipeline_->program_->use();

    for(auto& ubo : pipeline_->uniform_buffer_objects_)
        ubo->bind_uniform();
}

}    // namespace corgi