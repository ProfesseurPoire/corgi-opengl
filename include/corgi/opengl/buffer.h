#pragma once

#include <vector>

namespace corgi
{
enum class buffer_type
{
    array_buffer,
    element_array_buffer
};

class buffer
{
public:
    explicit buffer(buffer_type t);

    buffer(const buffer& buffer) = delete;
    buffer(buffer&& buffer) noexcept;

    buffer& operator=(const buffer& buffer) = delete;
    buffer& operator=(buffer&& buffer) noexcept;

    ~buffer();

    /*!
     * @brief Returns the id (opengl id) of the buffer
     */
    unsigned id() const noexcept;

    buffer_type buffer_type() const noexcept;


    void set_data(const std::vector<float>& data);
    void set_data(const std::vector<unsigned>& data);

    /*!
     * @brief Binds the current buffer
     * Technically const since it doesn't changes the object
     */
    void bind() const;

    /**
     * \brief Unbind the current buffer
     * Technically const since it doesn't changes the object
     */
    void end() const;

private:
    unsigned           id_;
    corgi::buffer_type buffer_type_;
};
}    // namespace corgi