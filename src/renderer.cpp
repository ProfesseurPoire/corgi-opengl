#include <corgi/opengl/renderer.h>
#include <glad/glad.h>
#include <corgi/opengl/uniform_buffers.h>
#include <corgi/opengl/shaders.h>
#include <corgi/opengl/primitives.h>

namespace corgi
{
    struct color_s
    {
        float r = 1.0F;
        float g = 1.0F;
        float b = 0.0F;
        float a = 1.0F;
    };

renderer::renderer(unsigned short screen_width, unsigned short screen_height)
: screen_width_(screen_width)
, screen_height_(screen_height)
{
    default_pipeline_.add_ubo<default_ubo>(1);
    default_pipeline_.add_ubo<color_s>(2);

    default_vertex_shader_.reset(new shader(common_shaders::simple_2d_texture_vertex_shader));
    default_fragment_shader_.reset(
        new shader(common_shaders::simple_2d_texture_fragment_shader));

    default_program_.reset(new program(*default_vertex_shader_,
                                        *default_fragment_shader_));

    default_pipeline_.program_ = default_program_.get();
}

void renderer::set_default_color(float r, float g, float b, float a)
{
    corgi::vec4 color;
    color.x = r;
    color.y = g;
    color.z = b;
    color.w = a;

    auto value = default_pipeline_.get_ubo<default_ubo>(1).data().front();
    value.color = color;
    value.use_color = true;
    default_pipeline_.get_ubo<default_ubo>(1).set_value(value);
}

void renderer::draw_default_circle_on_screen(float x, float y, float radius)
{
    mesh m = corgi::primitive::build_circle_pos2_uv(radius, 100);

    auto value = default_pipeline_.get_ubo<default_ubo>(1).data().front();
   

    value.mvp =

        Matrix::ortho(-screen_width_ / 2.0F, screen_width_ / 2.0f,
                      -screen_height_ / 2.0F, screen_height_ / 2.0F, -100,
                      100) *
        Matrix::translation(x, y, 0);

    default_pipeline_.get_ubo<default_ubo>(1).set_value(value);
    
        
    apply_pipeline(default_pipeline_);
    draw(m);
}

void renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::draw_default_rect_on_screen(float x, float y, float width, float height)
{
    mesh m = corgi::primitive::build_rect_pos2_uv(width/2.0F, height/2.0F);

    auto value = default_pipeline_.get_ubo<default_ubo>(1).data().front();

    value.mvp =

        Matrix::ortho(-screen_width_ / 2.0F, screen_width_ / 2.0f,
                      -screen_height_ / 2.0F, screen_height_ / 2.0F, -100,
                      100) *
        Matrix::translation(x, y, 0);

    default_pipeline_.get_ubo<default_ubo>(1).set_value(value);

    apply_pipeline(default_pipeline_);
    draw(m);
}

void renderer::set_clear_color(color clear_color)
{
    glClearColor(clear_color.red(), clear_color.green(), clear_color.blue(),
                 clear_color.alpha());

    clear_color_ = clear_color;
}

void renderer::draw(const mesh& m)
{
    m.vertex_array()->bind();

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m.indexes().size()),
                   GL_UNSIGNED_INT, (void*)0);

    m.vertex_array()->end();

   // pipeline_->program_->end();
}

void renderer::apply_pipeline(corgi::pipeline& new_pipeline)
{
    if (pipeline_->write_color != new_pipeline.write_color)
    {
        if(new_pipeline.write_color)
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        else
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    }

    if (pipeline_->enable_depth_test != new_pipeline.enable_depth_test)
    {
        if(new_pipeline.enable_depth_test)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    if(pipeline_->depth_mask != new_pipeline.depth_mask)
    {
        if(new_pipeline.depth_mask)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
    }

    if (pipeline_->program_->id() != new_pipeline.program_->id())
    {
        new_pipeline.program_->use();
    }

    for(auto& [location, ubo] : new_pipeline.uniform_buffer_objects_)
        ubo->bind_uniform();

    for(auto sampler : new_pipeline.samplers_)
    {
        glActiveTexture(GL_TEXTURE0 + sampler.binding);    // Texture unit 0
        sampler.texture->bind();
    }
}

void renderer::set_pipeline(corgi::pipeline& pipeline)
{
    apply_pipeline(*pipeline_);
    pipeline_ = &pipeline;
}

}    // namespace corgi