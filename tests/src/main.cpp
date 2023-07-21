#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <corgi/opengl/buffer.h>
#include <corgi/opengl/program.h>
#include <corgi/opengl/renderer.h>
#include <corgi/opengl/shader.h>
#include <corgi/opengl/shaders.h>
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

int main(int argc, char** argv)
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
    {
        std::cout << "Could not create window" << std::endl;
    }

    SDL_Surface* window_surface = SDL_GetWindowSurface(window);

    SDL_Event   e;
    SDL_Keycode kc;

    bool quit = false;

    const auto context = SDL_GL_CreateContext(window);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(0);

    renderer renderer;

    const auto circle = build_circle(0, 0, 0.2f, 50);

    glClearColor(0.8F, 0.8F, 0.8F, 1.0F);

    buffer vertex_buffer(buffer_type::array_buffer);
    buffer index_buffer(buffer_type::element_array_buffer);

    vertex_buffer.set_data(circle.vertices);
    index_buffer.set_data(circle.indexes);

    vertex_array vao(common_attributes::pos2, vertex_buffer, index_buffer);

    shader vs(common_shaders::simple_2d_vertex_shader);
    shader fs(common_shaders::simple_2d_fragment_shader);

    program prog(vs, fs);

    while(!quit)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    quit = true;
                    return 1;
            }
        }

        prog.use();
        vao.bind();
        glDrawElements(GL_TRIANGLES,
                       static_cast<GLsizei>(circle.indexes.size()),
                       GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(0);

        //renderer.draw(m);

        // L'objectif c'est de pouvoir écrire quelque chose comme
        // pour faire comme un mode par défaut
        // renderer.draw_circle(1.5f, 100);
        // ou renderer.draw(circle_mesh);

        prog.end();

        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 1;
}