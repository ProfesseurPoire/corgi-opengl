#include <corgi/opengl/buffer.h>

namespace corgi
{


class uniform_buffer_object_interface
{
public:
    virtual void bind_uniform() = 0;

private:
};

template<class T>
class uniform_buffer_object : public buffer<T, buffer_type::uniform>,
                              public uniform_buffer_object_interface
{
public:
    uniform_buffer_object(T data, int location)
        : corgi::buffer<T, buffer_type::uniform>({data})
        , location_(location)
    {
    }

    uniform_buffer_object(int location)
        : corgi::buffer<T, buffer_type::uniform>({ T() })
        , location_(location)
    {

    }

    void set_value(T data) { 
        std::vector<T> datas = {data};

        this->set_data(datas);
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
