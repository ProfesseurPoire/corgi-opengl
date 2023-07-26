#pragma once

#include <corgi/opengl/mesh.h>
#include <corgi/opengl/pipeline.h>

namespace corgi
{
   

class renderer
{
public:


    renderer(unsigned short screen_width, unsigned short screen_height);

    // Normally, you should set the pipeline then call draw
    void draw(const mesh& m);
    void set_pipeline(pipeline& pipeline);


    // This is really for the fixed default pipeline to draw things
    // quickly on the screen 
    void set_default_color(float r, float g, float b, float a=1.0F);
    void draw_default_circle_on_screen(float x, float y, float radius);
    void draw_default_rect_on_screen(float x, float y, float width, float height);

private:

    unsigned short   screen_width_;
    unsigned short   screen_height_;

    void apply_pipeline(pipeline& pipeline);

    corgi::pipeline* pipeline_ {nullptr};

    /**
     * Default pipeline for specific draw operations 
     */
    corgi::pipeline default_pipeline_;

    std::unique_ptr<shader> default_fragment_shader_;
    std::unique_ptr<shader> default_vertex_shader_;
    std::unique_ptr<program> default_program_;
};
}    // namespace corgi
