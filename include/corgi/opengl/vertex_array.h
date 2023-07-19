#pragma once
#include <corgi/opengl/buffer.h>
#include <corgi/opengl/vertex_attribute.h>

#include <vector>

namespace corgi
{

enum class primitive_type : char
{
    triangles,
    quads,
    lines
};

	class vertex_array
	{
        public:

			vertex_array(std::vector<vertex_attribute> vertex_attributes,
                         buffer&                       vertex_buffer,
                         buffer&                       index_buffer);
            vertex_array(const vertex_array& other) = delete;
            vertex_array(vertex_array&& other) noexcept;

			vertex_array& operator=(const vertex_array& other) = delete;
            vertex_array& operator=(vertex_array&& other) noexcept;

            ~vertex_array();


			void bind();

			unsigned id() const;

		private:

			unsigned id_;
            buffer*                      vertex_buffer_;
            buffer*                      index_buffer_;
            std::vector<vertex_attribute> vertex_attributes_;
	};
}