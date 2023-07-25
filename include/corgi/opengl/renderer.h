#pragma once

#include <corgi/opengl/mesh.h>
#include <corgi/opengl/pipeline.h>

namespace corgi
{
class renderer
{
public:
    void draw(const mesh& m);

    void set_pipeline(pipeline& pipeline);

private:
    corgi::pipeline* pipeline_ {nullptr};
};
}    // namespace corgi
