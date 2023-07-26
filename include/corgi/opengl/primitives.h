#pragma once

#include <corgi/opengl/mesh.h>

#include <numbers>

namespace corgi
{

namespace primitive
{
inline mesh build_circle_pos2(float radius, int discretisation)
{
    std::vector<float>    vertices;
    std::vector<unsigned> indexes;

    vertices.reserve(discretisation * 6);
    indexes.reserve(discretisation * 3);

    float delta = 2.0F * std::numbers::pi_v<float> / discretisation;

    for(int i = 0; i < discretisation; i++)
    {
        float angle  = delta * i;
        float angle2 = delta * (i + 1);

        vertices.push_back(0.0f);
        vertices.push_back(0.0F);

        vertices.push_back(cos(angle) * radius);
        vertices.push_back(sin(angle) * radius);

        vertices.push_back(cos(angle2) * radius);
        vertices.push_back(sin(angle2) * radius);

        indexes.push_back(i * 3);
        indexes.push_back(i * 3 + 1);
        indexes.push_back(i * 3 + 2);
    }

    return corgi::mesh(vertices, indexes, common_attributes::pos2);
}

inline mesh build_rect_pos2(float half_width, float half_height)
{
    std::vector<float>    vertices;
    std::vector<unsigned> indexes;

    vertices.reserve(2 * 4);
    indexes.reserve(3 * 2);

    vertices.push_back(-half_width);
    vertices.push_back(-half_height);

    vertices.push_back(half_width);
    vertices.push_back(-half_height);

    vertices.push_back(half_width);
    vertices.push_back(half_height);

    vertices.push_back(-half_width);
    vertices.push_back(half_height);

    indexes.push_back(0);
    indexes.push_back(1);
    indexes.push_back(2);

    indexes.push_back(2);
    indexes.push_back(3);
    indexes.push_back(0);

    return corgi::mesh(vertices, indexes, common_attributes::pos2);
}

inline mesh build_rect_pos2_uv(float half_width, float half_height)
{
    std::vector<float>    vertices;
    std::vector<unsigned> indexes;

    vertices.reserve(4 * 4);
    indexes.reserve(3 * 2);

    vertices.push_back(-half_width);
    vertices.push_back(-half_height);
    vertices.push_back(0.0f);
    vertices.push_back(0.0F);

    vertices.push_back(half_width);
    vertices.push_back(-half_height);
    vertices.push_back(1.0f);
    vertices.push_back(0.0F);

    vertices.push_back(half_width);
    vertices.push_back(half_height);
    vertices.push_back(1.0f);
    vertices.push_back(1.0F);

    vertices.push_back(-half_width);
    vertices.push_back(half_height);
    vertices.push_back(0.0f);
    vertices.push_back(1.0F);

    indexes.push_back(0);
    indexes.push_back(1);
    indexes.push_back(2);

    indexes.push_back(2);
    indexes.push_back(3);
    indexes.push_back(0);

    return corgi::mesh(vertices, indexes, common_attributes::pos2_uv);
}

}    // namespace primitive
}    // namespace corgi