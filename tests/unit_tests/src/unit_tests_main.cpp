#include <SDL.h>
#include <SDL_video.h>
#include <corgi/opengl/buffer.h>
#include <corgi/test/test.h>

#include <bitset>

using namespace corgi;

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    std::bitset<32> flags;

    flags[std::bit_width(unsigned(SDL_WINDOW_OPENGL)) - 1]     = 1;
    flags[std::bit_width(unsigned(SDL_WINDOW_SHOWN)) - 1]      = 0;
    flags[std::bit_width(unsigned(SDL_WINDOW_RESIZABLE)) - 1]  = 1;
    flags[std::bit_width(unsigned(SDL_WINDOW_BORDERLESS)) - 1] = 0;

    auto window =
        SDL_CreateWindow("unit_tests", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 500, 500, flags.to_ullong());

    if(!window)
    {
        std::cout << "Could not create window" << std::endl;
    }

    const auto context = SDL_GL_CreateContext(window);

    SDL_GL_MakeCurrent(window, context);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    test::add_test("buffer", "constructor_with_values",
                   []()
                   {
                       std::vector v {1.0F, 2.0F, 3.0F};
                       const buffer<float, buffer_type::array_buffer> buffer(v);
                       check_true(buffer.id() != 0);
                   });

    test::add_test("buffer", "default_constructor",
                   []()
                   {
                       buffer<float, buffer_type::array_buffer> empty_buffer;
                       check_true(empty_buffer.id() == 0);
                       check_any_throw(empty_buffer.bind());
                   });

    test::add_test("buffer", "copy_constructor",
                   []()
                   {
                       std::vector v {1.0F, 2.0F, 3.0F};
                       buffer<float, buffer_type::array_buffer> buffer(v);
                       check_true(buffer.id() != 0);

                       auto copied_buffer(buffer);
                       check_true(copied_buffer.id() != 0);
                       check_true(copied_buffer.id() != buffer.id());
                       check_true(copied_buffer.data() == buffer.data());
                   });

    test::add_test(
        "buffer", "copy_assignment",
        []()
        {
            buffer<float, buffer_type::array_buffer> b1({1.0F, 2.0F, 3.0F});
            check_true(b1.id() != 0);

            buffer<float, buffer_type::array_buffer> b2({1.0F, 2.0F});
            check_true(b2.id() != 0);

            b2 = b1;

            check_true(b1.data() == b2.data());
            check_true(b1.id() != b2.id());
        });

    test::add_test(
        "buffer", "move_assignment",
        []()
        {
            buffer<float, buffer_type::array_buffer> b1({1.0F, 2.0F, 3.0F});
            check_true(b1.id() != 0);

            buffer<float, buffer_type::array_buffer> b2({1.0F, 2.0F});
            check_true(b2.id() != 0);

            unsigned previous_id = b1.id();

            b2 = std::move(b1);

            check_true(b1.id() == 0);
            check_true(b2.id() == previous_id);
        });

    test::add_test("buffer", "move_constructor",
                   []()
                   {
                       std::vector v {1.0F, 2.0F, 3.0F};
                       buffer<float, buffer_type::array_buffer> buffer(v);

                       check_true(buffer.id() != 0);

                       const auto buffer_id = buffer.id();
                       const auto move_buffer(std::move(buffer));

                       check_true(buffer.id() == 0);

                       assert_that(move_buffer.id(), test::equals(buffer_id));

                       check_true(move_buffer.data() == v);
                       check_true(buffer.data().empty());

                       // Getting the data from the opengl pointed buffer to
                       // make sure it correspond with the data in the object
                       GLint size;
                       move_buffer.bind();
                       glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE,
                                              &size);

                       std::vector<float> vv(size / sizeof(float));

                       glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, vv.data());

                       move_buffer.unbind();

                       check_true(vv == v);

                       // binding a moved buffer should trigger an exception
                       // since its id is equal to zero
                       check_any_throw(buffer.bind());

                       // Now trying to set data to the moved object should put
                       // it back in a valid state
                       buffer.set_data({1, 2});

                       buffer.bind();
                       buffer.unbind();

                       check_true(buffer.id() != 0);
                   });

    test::add_test("buffer", "bind_empty_buffer",
                   []()
                   {
                       buffer<float, buffer_type::array_buffer> b =
                           buffer<float, buffer_type::array_buffer>();
                       check_any_throw(b.bind());
                   });

    return test::run_all();
}
