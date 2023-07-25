#pragma once

#include <string>

namespace corgi
{
class Image;

enum class min_filter : char
{
    nearest                = 0,
    linear                 = 1,
    nearest_mipmap_nearest = 2,
    nearest_mipmap_linear  = 3,
    linear_mipmap_linear   = 4,
    linear_mipmap_nearest  = 5
};

enum class mag_filter : char
{
    nearest = 0,
    linear  = 1
};

enum class format : char
{
    red,
    rg,
    rgb,
    bgr,
    rgba,
    bgra,
    red_integer,
    rg_integer,
    rgb_integer,
    bgr_integer,
    rgba_integer,
    bgra_integer,
    stencil_index,
    depth_component,
    depth_stencil
};

// TODO : More exist
enum class internal_format : char
{
    depth_component,
    depth_stencil,
    red,
    rg,
    rgb,
    rgba,
    r8,
    r16,
    rg8,
    rg16,
    rg32_f,
    rg32_i,
    rg32_ui,
    depth24_stencil8
};

enum class data_type : char
{
    unsigned_byte,
    byte,
    unsigned_short,
    short_,
    unsigned_int,
    int_,
    half_float,
    float_,
    unsigned_int24_8
};

enum class wrap : char
{
    clamp_to_border      = 0,
    clamp_to_edge        = 1,
    mirrored_repeat      = 2,
    mirror_clamp_to_edge = 3,
    repeat               = 4
};

struct create_info
{
    min_filter      min_filter {min_filter::nearest};
    mag_filter      mag_filter {mag_filter::nearest};
    data_type       data_type {data_type::unsigned_byte};
    internal_format internal_format {internal_format::rgba};
    format          format {format::rgba};
    wrap            wrap_s {wrap::repeat};
    wrap            wrap_t {wrap::repeat};
    int             width;
    int             height;
    void*           data;
};

//! @brief  Represents an usable OpenGL texture. Must be loaded using an image
//! object
//!
//! Warning, you must make sure to call "use" before using any method
//! modifying the texture's state and when you're done with the texture,
//! don't forget to call close. This is done so we can chain textures
//! operation without having to tell OpenGL to switch texture, which is a
//! costly operation
class texture
{
public:
    texture(create_info info);

    texture();

    /*!
     * @brief	Construct a texture from a .tex file
     */
    texture(const std::string& path, const std::string& relative_path);

    /*!
     * @brief	Generates a new texture
     *			Copies the name
     */
    texture(const std::string& name,
            unsigned           width,
            unsigned           height,
            min_filter         min_filter,
            mag_filter         mag_filter,
            wrap               wrap_s,
            wrap               wrap_t,
            format             format,
            internal_format    internal_format,
            data_type          dt,
            unsigned char*     data = nullptr);

    texture(texture&& texture) noexcept;
    texture(const texture& texture) = delete;

    // TODO :	Can't copy since the texture object doesn't hold
    //			the image data on the CPU which is probably
    //			a mistake. Might be easier if it kept a reference
    //			to an image, just so I could for instance update it
    //			or something
    texture& operator=(texture&& texture) noexcept;
    texture& operator=(const texture& texture) = delete;

    ~texture();

    bool operator==(const texture& other) const noexcept;

    bool operator<(const texture& other) const noexcept;

    /*!
     *	@brief	Returns the texture's the number returned by the GPU to
     *			identify the current Texture
     */
    unsigned int id() const noexcept;

    void bind() const;

    void unbind() const;

    /*!
     * @brief	Returns the filter used when minifying pixels
     * @return	Potential values are :
     *			* MinFilter::Nearest
     *			* MinFilter::Linear
     *			* MinFilter::NearestMipmapNearest
     *			* MinFilter::NearestMipmapLinear
     *			* MinFilter::LinearMipmapLinear
     *			* MinFilter::LinearMipmapNearest
     */
    min_filter min_filter() const noexcept;

    /*!
     * @brief	Returns the filter used when magnifying pixels
     * @return	Potential values are :
     *			* MagFilter::Linear
     *			* MagFilter::Nearest
     */
    mag_filter mag_filter() const noexcept;

    /*!
     * @brief Returns the wrap value for the s coordinate
     */
    wrap wrap_s() const noexcept;

    /*!
     * @brief Returns the wrap value for the t coordinate
     */
    wrap wrap_t() const noexcept;

    unsigned width() const noexcept;
    unsigned height() const noexcept;

    void width(unsigned width) noexcept;
    void height(unsigned height) noexcept;

    /*!
     *	@brief Change the filter used for pixel minification
     *
     *	Don't forget to call the @ref apply_changes() function
     *	after you changed this filter.
     *
     *  @param[in] filter : Potential values are
     *						* MinFilter::Nearest
     *						* MinFilter::Linear
     *						*
     *MinFilter::NearestMipmapNearest
     *						* MinFilter::NearestMipmapLinear
     *						* MinFilter::LinearMipmapLinear
     *						* MinFilter::LinearMipmapNearest
     */
    void min_filter(corgi::min_filter filter) noexcept;

    /*!
     *	@brief Changes the filter used for pixel magnification
     *
     *  Don't forget to call the @ref apply_changes() function
     *	after you changed a filter
     *
     *  @param[in] filter : Potential Values are
     *						* MagFilter::Nearest
     *						* MagFilter::Linear
     */
    void mag_filter(corgi::mag_filter filter) noexcept;

    /*!
     *	@brief Changes the way the s coordinate is wrapped
     *
     *  @param[in] wrap :	Values are
     *  					* Wrap::ClampToBorder,
     *  					* Wrap::ClampToEdge,
     *  					* Wrap::MirroredRepeat,
     *  					* Wrap::MirrorClampToEdge,
     *  					* Wrap::Repeat
     */
    void wrap_s(wrap wrap) noexcept;

    /*!
     *	@brief Changes the way the t coordinate is wrapped
     *
     *  @param[in] wrap :	Values are
     *						* Wrap::ClampToBorder,
     *						* Wrap::ClampToEdge,
     *						* Wrap::MirroredRepeat,
     *						* Wrap::MirrorClampToEdge,
     *						* Wrap::Repeat
     */
    void wrap_t(wrap wrap) noexcept;

    /*!
     * @brief Used by the font to do stuff
     */
    // void build_texture(int width, int height);

    /*!
     * @brief  Returns the aspect ratio of the texture
     * The texture ration is width/height
     */
    float ratio() const;

    const char* name() const;

    /*!
     * @brief Apply the changes done to the texture
     *
     *	Sending data to the GPU is a costly operation. So to
     *	avoid having to call the GPU each time the user changes one
     *	parameter, the user must call this function to commit its
     *	changes, thus updating the texture only once
     *
     *	Though I don't really like that I have to remember to call
     *	this function after playing with a texture
     */
    void apply_changes();
    void unpack_pixels();

    /*  void tex_sub_image(int mipmap_level, int x, int y, int width, int
       height, Image::format, void * pixels);*/

private:

    void generate_opengl_texture();

    void update_gl_min_filter();
    void update_gl_mag_filter();

    void update_gl_wrap_s();
    void update_gl_wrap_t();

    std::string name_;

    unsigned int id_ = 0u;    // 4 bytes

    // Todo : I could probably put all of that in half the size but
    // Padding would eat it anyways so
    corgi::min_filter min_filter_ = min_filter::nearest;    // 1 byte
    corgi::mag_filter mag_filter_ = mag_filter::nearest;    // 1 byte
    wrap              wrap_s_     = wrap::repeat;           // 1 byte
    wrap              wrap_t_     = wrap::repeat;           // 1 byte

    // Using unsigned because glTexImage2D uses GLSizei for width/height
    unsigned width_ {0u};
    unsigned height_ {0u};

    format          format_;
    internal_format internal_format_;
    data_type       data_type_;
    void*           data_;

    // Total size : 20 bytes
};
}    // namespace corgi
