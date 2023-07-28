#pragma once

namespace corgi
{
/**
 * @brief   Represents a RGBA color
 *
 *          A color is divided in 4 components :
 *              * red
 *              * green
 *              * blue
 *              * alpha
 *
 *          You can set the intensity of a componant with an integer represented by an int 
 *          or a float.
 *
 *          As an integer, values must range between 0 and 255
 *          As a float, values must range between 0.0f and 1.0f
 *
 *          Values will be clamped to the ranges. So if you try to
 *          sets the red component to 300, it will be clamped to 255.
 * 
 *          Ultimately, floats will be converted to an int internally
 */
class color
{
public:
    /**
     * @brief   Constructs a default color object
     * 
     *          Default color object uses fuschia. 
     *          The reason for this is to make it more obvious that a color has not been set 
     */
    color() noexcept = default;

    /**
     * @brief   Constructs a new color object
     */
    color(int r, int g, int b, int a = 255) noexcept;

    /**
     * @brief   Constructs a new color object
     *
     *          Values for each color component are clamped to the [0.0F, 1.0F] range
     */
    color(float r, float g, float b, float a = 1.0f) noexcept;

    /**
     * @brief   Sets the color's red component intensity
     *
     * @param red Intensity of the red component
     */
    void set_red(int red) noexcept;

    /**
     * @brief   Sets the color's red component intensity
     *
     * @param red Intensity of the red component clamped to the [0.0f, 1.0f] range
     */
    void set_red(float red) noexcept;

    /**
     * @brief   Sets the color's green component intensity
     *
     * @param green Intensity of the green component
     */
    void set_green(int green) noexcept;

    /**
     * @brief   Sets the color's green component intensity
     *
     * @param green Intensity of the green component clamped to the [0.0f,1.0f] range
     */
    void set_green(float green) noexcept;

    /**
     * @brief   Sets the color's blue component intensity
     *
     * @param blue Intensity of the blue component
     */
    void set_blue(int blue) noexcept;

    /**
     * @brief Sets the color's blue component intensity
     *
     * @param blue Intensity of the blue component clamped to the [0.0f,1.0f] range
     */
    void set_blue(float blue) noexcept;

    /**
     * @brief Sets the color's alpha component intensity
     *
     * @param alpha Intensity of the alpha component
     */
    void set_alpha(int alpha) noexcept;

    /**
     * @brief Sets the the color's alpha component intensity
     *
     * @param alpha Intensity of the alpha component clamped to the [0.0f,1.0f] range
     */
    void set_alpha(float alpha) noexcept;

    /**
     * @brief   Gets the red component's intensity
     * @return  Return value will range between 0.0f and 1.0f
     */
    float red() const noexcept;

    /**
     * @brief   Gets the red component's intensity
     * @return  Return value will range between 0 and 255
     */
    int red_as_int() const noexcept;

    /**
     * @brief   Gets the green component's intensity
     * @return  Return value will range between 0.0f and 1.0f
     */
    float green() const noexcept;

    /**
     * @brief   Gets the green component's intensity
     * @return  Return value will range between 0 and 255
     */
    int green_as_int() const noexcept;

    /**
     * @brief   Gets the blue component's intensity
     * @return  Return value will range between 0.0f and 1.0f
     */
    float blue() const noexcept;

    /**
     * @brief   Gets the blue component's intensity
     * @return  Return value will range between 0 and 255
     */
    int blue_as_int() const noexcept;

    /**
     * @brief   Gets the alpha component's intensity
     * @return  Return value will range between 0.0f and 1.0f
     */
    float alpha() const noexcept;

    /**
     * @brief   Gets the alpha component's intensity
     * @return  Return value will range between 0 and 255
     */
    int alpha_as_int() const noexcept;

    /**
     * @brief   Comparison operator
     *
     *          Returns true if both colors are equals
     *
     * @return true
     * @return false
     */
    bool operator==(const color& rhs) const noexcept;

    /**
     * @brief Substract 2 colors
     *
     * @param rhs
     * @return Color
     */
    color operator-(const color& rhs) const noexcept;

    /**
     * @brief   Add up 2 colors
     *
     * @param rhs
     * @return Color
     */
    [[nodiscard]] color operator+(const color& rhs) const noexcept;

private:
    int red_   = 255;
    int green_ = 0;
    int blue_  = 255;
    int alpha_ = 255;
};
}    // namespace corgi
