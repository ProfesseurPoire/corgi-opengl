﻿#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <corgi/math/vec2.h>
#include <corgi/opengl/buffer.h>
#include <corgi/opengl/image.h>
#include <corgi/opengl/primitives.h>
#include <corgi/opengl/program.h>
#include <corgi/opengl/renderer.h>
#include <corgi/opengl/shader.h>
#include <corgi/opengl/shaders.h>
#include <corgi/opengl/texture.h>
#include <corgi/opengl/uniform_buffers.h>
#include <corgi/opengl/vertex_array.h>
#include <glad/glad.h>

#include <bit>
#include <bitset>
#include <iostream>
#include <numbers>
#include <vector>

using namespace corgi;

struct mmesh
{
    std::vector<float>    vertices;
    std::vector<unsigned> indexes;
};

mmesh build_circle(float center_x,
                   float center_y,
                   float radius,
                   int   discretisation)
{
    mmesh m;

    m.vertices.reserve(discretisation * 6);
    m.indexes.reserve(discretisation * 3);

    float delta = 2.0F * std::numbers::pi_v<float> / discretisation;

    for(int i = 0; i < discretisation; i++)
    {
        float angle  = delta * i;
        float angle2 = delta * (i + 1);

        m.vertices.push_back(center_x);
        m.vertices.push_back(center_y);

        m.vertices.push_back(center_x + cos(angle) * radius);
        m.vertices.push_back(center_y + sin(angle) * radius);

        m.vertices.push_back(center_x + cos(angle2) * radius);
        m.vertices.push_back(center_y + sin(angle2) * radius);

        m.indexes.push_back(i * 3);
        m.indexes.push_back(i * 3 + 1);
        m.indexes.push_back(i * 3 + 2);
    }
    return m;
}

struct f4
{
    float x = 1.0F;
    float y = 0.0F;
    float z = 0.0F;
    float w = 1.0F;
};

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    std::bitset<32> flags;

    flags[std::bit_width(unsigned(SDL_WINDOW_OPENGL)) - 1]     = 1;
    flags[std::bit_width(unsigned(SDL_WINDOW_SHOWN)) - 1]      = 1;
    flags[std::bit_width(unsigned(SDL_WINDOW_RESIZABLE)) - 1]  = 1;
    flags[std::bit_width(unsigned(SDL_WINDOW_BORDERLESS)) - 1] = 0;

    auto window =
        SDL_CreateWindow("dijkstra", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 500, 500, flags.to_ullong());

    if(!window)
        std::cout << "Could not create window" << std::endl;

    const auto context = SDL_GL_CreateContext(window);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(0);

    glClearColor(0.8F, 0.8F, 0.8F, 1.0F);

    image img = image::load("C:/dev/corgi-opengl/resources/majestic.png");

    texture texture = corgi::texture(
        "image", img.width, img.height, min_filter::linear, mag_filter::linear,
        wrap::repeat, wrap::repeat, format::rgba, internal_format::rgba,
        data_type::unsigned_byte, img.data.data());

    mesh mesh = primitive::build_rect_pos2_uv(0.5f, 0.5f);

    shader vs(common_shaders::simple_2d_texture_vertex_shader);
    shader fs(common_shaders::simple_2d_texture_fragment_shader);

    program prog(vs, fs);

    corgi::pipeline pipeline;
    pipeline.program_ = &prog;

    // So that way I can still update the ubo's data easily but still not the
    // easiest thing to manipulate but hey, 3D configurable pipeline are
    // everything but easy Since MVP matrix is kinda a given maybe it'd be nice
    // to have at least that in every ubo. Or maybe prepare some struct for
    // that?

    // Or maybe I should keep track of the struct I have to the function.

    // Whatever I end up doing for now I think that's fine on the renderer part

    const auto ortho = Matrix::ortho(-2, 2, -2, 2, -200, 200);

    default_ubo def_ubo;

    auto vubo = new uniform_buffer_object<default_ubo, shader_stage::vertex>(
        {def_ubo}, 1);

    auto ubo = new uniform_buffer_object<f4, shader_stage::fragment>({f4()}, 2);

    auto data = ubo->data();
    // Change stuff
    // Then feed it back ?
    ubo->set_data(data);

    // It would be nice to have a nicer syntax for this
    // But overall ... it works!
    pipeline.uniform_buffer_objects_.emplace_back(
        (uniform_buffer_object_interface*)ubo);

    pipeline.uniform_buffer_objects_.emplace_back(
        (uniform_buffer_object_interface*)vubo);

    pipeline.samplers_.push_back({&texture, 0});
    renderer renderer;

    bool quit = false;

    float angle = 0.f;

    while(!quit)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_Event e;

        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    quit = true;
                    return 1;
            }
        }

        // Really need to make this thing a bit easier to manages

        default_ubo v;
        v.mvp = {ortho * Matrix::rotation_z(angle)};

        vubo->set_data({v});

        renderer.set_pipeline(pipeline);
        renderer.draw(mesh);
        SDL_GL_SwapWindow(window);

        angle += 0.001f;
    }

    SDL_Quit();
    return 1;
}