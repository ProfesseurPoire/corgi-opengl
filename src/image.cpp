#include <corgi/opengl/image.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#    define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#    define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include <corgi/opengl/stb_image.h>
// #include "stb_image_write.h"

namespace corgi
{
image image::load(const std::string& path)
{
    int x, y, channels;
    // Images are horizontal on OpenGL otherwise
    stbi_set_flip_vertically_on_load(true);

    stbi_uc* imageData =
        stbi_load(path.c_str(), &x, &y, &channels, STBI_rgb_alpha);

    std::vector data_(imageData, imageData + x * y * channels);

    return {data_, x, y};
}
}    // namespace corgi