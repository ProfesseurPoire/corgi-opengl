#pragma once

#include <string>
#include <vector>

namespace corgi
{

struct image
{
    static image load(const std::string& path);

    std::vector<unsigned char> data;
    int                        width;
    int                        height;
};
}    // namespace corgi
