#include <SDL2/SDL.h>
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

constexpr unsigned short screen_width = 500;
constexpr unsigned short screen_height = 500;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

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
                                   SDL_WINDOWPOS_CENTERED, screen_width,
                                   screen_height,
                                   flags.to_ullong());

    if(!window)
        std::cout << "Could not create window" << std::endl;

    const auto context = SDL_GL_CreateContext(window);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(0);

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

    auto& def_ubo = pipeline.add_ubo<default_ubo>(1);

    pipeline.samplers_.push_back({&texture, 0});
    renderer renderer(screen_width, screen_height);

    glEnable(GL_MULTISAMPLE);  


    bool quit = false;

    float angle = 0.f;
    const auto ortho = Matrix::ortho(-2, 2, -2, 2, -200, 200);

    renderer.set_clear_color({0.3f, 0.3F, 0.3F, 1.0F});

    while(!quit)
    {
        renderer.clear();

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


        // Maybe a bit cumbersome to get back the data
        // change the value etc 
        // Ideally it would be nice to be able to set something
        // directly 
        auto val = def_ubo.data().front();
        val.mvp  = ortho * Matrix::rotation_z(angle);
        def_ubo.set_value(val);

        renderer.set_pipeline(pipeline);
        renderer.draw(mesh);

        renderer.set_default_color(1.0F,1.0F, 0.0F);
        renderer.draw_default_circle_on_screen(150, 1, 70);

        
        renderer.set_default_color(1.0F, 0.0F, 1.0F);
        renderer.draw_default_rect_on_screen(-150.0F, 0.0F, 10, 10);
        SDL_GL_SwapWindow(window);

        angle += 0.001f;
    }

    SDL_Quit();
    return 1;
}