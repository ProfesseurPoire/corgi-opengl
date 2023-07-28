#include <corgi/opengl/Color.h>
#include <algorithm>

using namespace corgi;

color::color(int r, int g, int b, int a) noexcept
    : red_(std::clamp(r, 0, 255))
    , green_(std::clamp(g, 0, 255))
    , blue_(std::clamp(b, 0, 255))
    , alpha_(std::clamp(a, 0, 255))
{
}

color::color(float r, float g, float b, float a) noexcept
    : red_(static_cast<int>(std::clamp(r, 0.0f, 1.0f) * 255.0f))
    , green_(static_cast<int>(std::clamp(g, 0.0f, 1.0f) * 255.0f))
    , blue_(static_cast<int>(std::clamp(b, 0.0f, 1.0f) * 255.0f))
    , alpha_(static_cast<int>(std::clamp(a, 0.0f, 1.0f) * 255.0f))
{
}

void color::set_red(int red) noexcept
{
    red_ = std::clamp(red, 0, 255);
}

void color::set_red(float red) noexcept
{
    red_ = static_cast<int>(std::clamp(red, 0.0f, 1.0f) * 255.0f);
}

void color::set_green(float green) noexcept
{
    green_ = static_cast<int>(std::clamp(green, 0.0f, 1.0f) * 255.0f);
}

void color::set_green(int green) noexcept
{
    green_ = std::clamp(green, 0, 255);
}

void color::set_blue(float blue) noexcept
{
    blue_ = static_cast<int>(std::clamp(blue, 0.0f, 1.0f) * 255.0f);
}

void color::set_blue(int blue) noexcept
{
    blue_ = std::clamp(blue, 0, 255);
}

void color::set_alpha(float alpha) noexcept
{
    alpha_ = static_cast<int>(std::clamp(alpha, 0.0f, 1.0f) * 255.0f);
}

void color::set_alpha(int alpha) noexcept
{
    alpha_ = std::clamp(alpha, 0, 255);
}

float color::red() const noexcept
{
    return static_cast<float>(red_) / 255.0f;
}

int color::red_as_int() const noexcept
{
    return red_;
}

float color::green() const noexcept
{
    return static_cast<float>(green_) / 255.0f;
}

int color::green_as_int() const noexcept
{
    return green_;
}

float color::blue() const noexcept
{
    return static_cast<float>(blue_) / 255.0f;
}

int color::blue_as_int() const noexcept
{
    return blue_;
}

float color::alpha() const noexcept
{
    return static_cast<float>(alpha_) / 255.0f;
}

int color::alpha_as_int() const noexcept
{
    return alpha_;
}

bool color::operator==(const color& other) const noexcept
{
    return (red_ == other.red_ && green_ == other.green_ &&
            blue_ == other.blue_ && alpha_ == other.alpha_);
}

color color::operator-(const color& rhs) const noexcept
{
    color c;

    c.set_red(red_ - rhs.red_);
    c.set_green(green_ - rhs.green_);
    c.set_blue(blue_ - rhs.blue_);
    c.set_alpha(alpha_ - rhs.alpha_);

    return c;
}

color color::operator+(const color& rhs) const noexcept
{
    color c;

    c.set_red(red_ + rhs.red_);
    c.set_green(green_ + rhs.green_);
    c.set_blue(blue_ + rhs.blue_);
    c.set_alpha(alpha_ + rhs.alpha_);

    return c;
}
