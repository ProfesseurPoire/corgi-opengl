#pragma once

#include <glad/glad.h>

#include <exception>
#include <stdexcept>
#include <vector>

namespace corgi
{
enum class buffer_type
{
    array_buffer,
    element_array_buffer
};

template<class T, buffer_type type_>
class buffer
{
public:
    /**
     * \brief Default constructor. Puts the buffer in an empty state
     * \param type
     */
    buffer() {}

    buffer(std::vector<T> data)
        : data_(std::move(data))
    {
        glGenBuffers(1, &id_);

        if(id_ == 0)
            throw std::exception(
                "buffer::buffer(std::vector<T> data, buffer_type type) : id is "
                "equals to 0 after glGenBuffers");

        push_data();
    }

    buffer(const buffer& other)
        : data_(other.data_)
    {
        glGenBuffers(1, &id_);

        if(id_ == 0)
            throw std::exception("buffer::buffer(const buffer& other) : id is "
                                 "equals to 0 after glGenBuffers");

        push_data();
    }

    buffer(buffer&& other) noexcept
        : data_(std::move(other.data_))
    {
        id_ = other.id_;
        push_data();
        other.id_ = 0;
    }

    buffer& operator=(const buffer& other)
    {
        clear();

        data_ = other.data_;
        push_data();
        return *this;
    }

    buffer& operator=(buffer&& other) noexcept
    {
        clear();

        id_   = other.id_;
        data_ = std::move(other.data_);
        push_data();

        other.id_ = 0;
        return *this;
    }

    ~buffer() { clear(); }

    /**
     * \brief Puts the buffer in empty state
     */
    void clear()
    {
        glDeleteBuffers(1, &id_);
        id_ = 0;
        data_.clear();
    }

    static buffer_type type() noexcept { return type_; }

    bool empty() const { return id_ == 0; }

    void set_data(std::vector<T> data)
    {
        data_ = std::move(data);
        push_data();
    }

    /**
     * @brief Binds the current buffer so it is used for the following opengl
     * operations
     *
     * If the buffer is empty (from default constructor or moved operation) the
     * functions throws an exception
     *
     * @throws logic_error Thrown if the object is in empty state (no call to
     * glGenBuffer was made and thus id_ member variable is equal to 0)
     */
    void bind() const
    {
        if(id_ == 0)
            throw std::logic_error("buffer::bind : Can't bind an empty buffer");

        switch(type_)
        {
            case buffer_type::array_buffer:
                glBindBuffer(GL_ARRAY_BUFFER, id_);
                break;

            case buffer_type::element_array_buffer:
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
                break;
        }
    }

    /**
     * \brief Returns the data stored by the buffer
     * \return
     */
    const std::vector<T>& data() const { return data_; }

    unsigned id() const { return id_; }

    void unbind() const
    {
        switch(type_)
        {
            case buffer_type::array_buffer:
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                break;

            case buffer_type::element_array_buffer:
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                break;
        }
    }

private:
    /**
     * \brief Pushes the data to the GPU
     */
    void push_data()
    {
        // If id_ is equals to zero qui try to
        // regenerate a buffer id.
        // The only way for id_ to be equals to zero is if we moved the
        // buffer
        if(id_ == 0)
            glGenBuffers(1, &id_);

        bind();
        const auto s = static_cast<GLsizei>(sizeof(float) * data_.size());

        switch(type_)
        {
            case buffer_type::array_buffer:
                glBufferData(GL_ARRAY_BUFFER, s, data_.data(), GL_STATIC_DRAW);
                break;

            case buffer_type::element_array_buffer:
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, s, data_.data(),
                             GL_STATIC_DRAW);
                break;
        }
    }
    std::vector<T> data_;
    unsigned       id_ {0};
};

}    // namespace corgi