#include <corgi/opengl/buffer.h>

namespace corgi
{

enum class shader_stage
{
    vertex,
    fragment
};

class uniform_buffer_object_interface
{
public:
    virtual void bind_uniform() = 0;

private:
};

template<class T, shader_stage stage>
class uniform_buffer_object : public buffer<T, buffer_type::uniform>,
                              uniform_buffer_object_interface
{
public:
    uniform_buffer_object(std::vector<T> data, int location)
        : corgi::buffer<T, buffer_type::uniform>(data)
        , location_(location)
    {
    }

    void bind_uniform() override
    {
        auto id = this->id_;
        glBindBufferBase(GL_UNIFORM_BUFFER, location_, id);
    }

private:
    int location_;
};
}    // namespace corgi
