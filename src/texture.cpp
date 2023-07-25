#include <corgi/opengl/texture.h>
#include <glad/glad.h>

#include <map>

using namespace corgi;

static corgi::mag_filter parse_mag_filter(const std::string& str)
{
    static std::map<std::string, corgi::mag_filter> mag_filters = {
        {"nearest", corgi::mag_filter::nearest},
        {"linear", corgi::mag_filter::linear}};
    return mag_filters.at(str);
}

static corgi::min_filter parse_min_filter(const std::string& str)
{
    static std::map<std::string, corgi::min_filter> min_filters_ = {
        {"nearest", corgi::min_filter::nearest},
        {"linear", corgi::min_filter::linear},
        {"nearest_mipmap_nearest", corgi::min_filter::nearest_mipmap_nearest},
        {"nearest_mipmap_linear", corgi::min_filter::nearest_mipmap_linear},
        {"linear_mipmap_linear", corgi::min_filter::linear_mipmap_linear},
        {"linear_mipmap_nearest", corgi::min_filter::linear_mipmap_nearest}};
    return min_filters_.at(str);
}

static corgi::wrap load_wrap(const std::string& str)
{
    static std::map<std::string, corgi::wrap> wraps = {
        {"repeat", corgi::wrap::repeat},
        {"clamp_to_border", corgi::wrap::clamp_to_border},
        {"clamp_to_edge", corgi::wrap::clamp_to_edge},
        {"mirrored_repeat", corgi::wrap::mirrored_repeat},
        {"mirror_clamp_to_edge", corgi::wrap::mirror_clamp_to_edge}};
    return wraps.at(str);
}

#define check_gl_error()                                        \
    {                                                           \
        GLenum result;                                          \
                                                                \
        while((result = glGetError()) != GL_NO_ERROR)           \
        {                                                       \
            switch(result)                                      \
            {                                                   \
                case GL_INVALID_ENUM:                           \
                    log_error("Invalid Enum");                  \
                    break;                                      \
                                                                \
                case GL_INVALID_VALUE:                          \
                    log_error("Invalid Value");                 \
                    break;                                      \
                                                                \
                case GL_INVALID_OPERATION:                      \
                    log_error("Invalid Operation");             \
                    break;                                      \
                                                                \
                case GL_INVALID_FRAMEBUFFER_OPERATION:          \
                    log_error("Invalid Framebuffer Operation"); \
                    break;                                      \
                                                                \
                case GL_OUT_OF_MEMORY:                          \
                    log_error("Out of Memory");                 \
                    break;                                      \
                default:                                        \
                    break;                                      \
            }                                                   \
        }                                                       \
    }

texture::texture(create_info info)
    : min_filter_(info.min_filter)
    , mag_filter_(info.mag_filter)
    , data_type_(info.data_type)
    , wrap_s_(info.wrap_s)
    , wrap_t_(info.wrap_t)
    , format_(info.format)
    , internal_format_(info.internal_format)
    , width_(info.width)
    , height_(info.height)
    , data_(info.data)
{
    // Generate the new texture
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id_);
    // We bind the texture
    glBindTexture(GL_TEXTURE_2D, id_);
    // check_gl_error();

    GLenum format {GL_RGBA};
    GLint  internal_format {GL_RGBA};
    GLenum t {GL_UNSIGNED_BYTE};

    switch(min_filter_)
    {
        case corgi::min_filter::nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        case corgi::min_filter::linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        case corgi::min_filter::nearest_mipmap_nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_NEAREST_MIPMAP_NEAREST);
            break;
        case corgi::min_filter::nearest_mipmap_linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_NEAREST_MIPMAP_LINEAR);
            break;
        case corgi::min_filter::linear_mipmap_linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
            break;
        case corgi::min_filter::linear_mipmap_nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_NEAREST);
            break;
    }
    // check_gl_error();
    switch(mag_filter_)
    {
        case corgi::mag_filter::nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case corgi::mag_filter::linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
    }
    // check_gl_error();
    switch(wrap_s_)
    {
        case corgi::wrap::clamp_to_border:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_BORDER);
            break;

        case corgi::wrap::clamp_to_edge:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            break;

        case corgi::wrap::mirrored_repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            GL_MIRRORED_REPEAT);
            break;

        case corgi::wrap::mirror_clamp_to_edge:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            GL_MIRROR_CLAMP_TO_EDGE);
            break;

        case corgi::wrap::repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            break;
    }
    // check_gl_error();
    switch(wrap_t_)
    {
        case corgi::wrap::clamp_to_border:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_BORDER);
            break;

        case corgi::wrap::clamp_to_edge:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;

        case corgi::wrap::mirrored_repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            GL_MIRRORED_REPEAT);
            break;

        case corgi::wrap::mirror_clamp_to_edge:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            GL_MIRROR_CLAMP_TO_EDGE);
            break;

        case corgi::wrap::repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
    }
    // check_gl_error();
    switch(format_)
    {
        case corgi::format::red:
            format = GL_RED;
            break;
        case corgi::format::rg:
            format = GL_RG;
            break;
        case corgi::format::rgb:
            format = GL_RGB;
            break;
        case corgi::format::bgr:
            format = GL_BGR;
            break;
        case corgi::format::rgba:
            format = GL_RGBA;
            break;
        case corgi::format::bgra:
            format = GL_BGRA;
            break;
        case corgi::format::red_integer:
            format = GL_RED_INTEGER;
            break;
        case corgi::format::rg_integer:
            format = GL_RG_INTEGER;
            break;
        case corgi::format::rgb_integer:
            format = GL_RGB_INTEGER;
            break;
        case corgi::format::bgr_integer:
            format = GL_BGR_INTEGER;
            break;
        case corgi::format::rgba_integer:
            format = GL_RGBA_INTEGER;
            break;
        case corgi::format::bgra_integer:
            format = GL_BGRA_INTEGER;
            break;
        case corgi::format::stencil_index:
            format = GL_STENCIL_INDEX;
            break;
        case corgi::format::depth_component:
            format = GL_DEPTH_COMPONENT;
            break;
        case corgi::format::depth_stencil:
            format = GL_DEPTH_STENCIL;
            break;
        default:
            break;
    }
    // check_gl_error();
    switch(internal_format_)
    {
        case corgi::internal_format::depth_component:
            internal_format = GL_DEPTH_COMPONENT;
            break;
        case corgi::internal_format::depth_stencil:
            internal_format = GL_DEPTH_STENCIL;
            break;
        case corgi::internal_format::red:
            internal_format = GL_RED;
            break;
        case corgi::internal_format::rg:
            internal_format = GL_RG;
            break;
        case corgi::internal_format::rgb:
            internal_format = GL_RGB;
            break;
        case corgi::internal_format::rgba:
            internal_format = GL_RGBA;
            break;
        case corgi::internal_format::r8:
            internal_format = GL_R8;
            break;
        case corgi::internal_format::r16:
            internal_format = GL_R16;
            break;
        case corgi::internal_format::rg8:
            internal_format = GL_RG8;
            break;
        case corgi::internal_format::rg16:
            internal_format = GL_RG16;
            break;
        case corgi::internal_format::rg32_f:
            internal_format = GL_RG32F;
            break;
        case corgi::internal_format::rg32_i:
            internal_format = GL_RG32I;
            break;
        case corgi::internal_format::rg32_ui:
            internal_format = GL_RG32UI;
            break;
        case corgi::internal_format::depth24_stencil8:
            internal_format = GL_DEPTH24_STENCIL8;
            break;
        default:
            break;
    }
    // check_gl_error();
    switch(data_type_)
    {
        case corgi::data_type::unsigned_byte:
            t = GL_UNSIGNED_BYTE;
            break;
        case corgi::data_type::byte:
            t = GL_BYTE;
            break;
        case corgi::data_type::unsigned_short:
            t = GL_UNSIGNED_SHORT;
            break;
        case corgi::data_type::short_:
            t = GL_SHORT;
            break;
        case corgi::data_type::unsigned_int:
            t = GL_UNSIGNED_INT;
            break;
        case corgi::data_type::int_:
            t = GL_INT;
            break;
        case corgi::data_type::half_float:
            t = GL_HALF_FLOAT;
            break;
        case corgi::data_type::float_:
            t = GL_FLOAT;
            break;
        case corgi::data_type::unsigned_int24_8:
            t = GL_UNSIGNED_INT_24_8;
        default:
            break;
    }
    // check_gl_error();

    int max_size;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
    log_info(max_size);

    glTexImage2D(GL_TEXTURE_2D,
                 0,                  // Level
                 internal_format,    // Internal format
                 width_, height_,
                 0,         // Border
                 format,    // format
                 t,         // type
                 data_      // data
    );

    check_gl_error();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

texture::texture()
{
    log_info("Creating new empty texture");
}

texture::texture(const std::string& path, const std::string& relative_path)
    : name_(relative_path.c_str())
{
    auto size = filesystem::size(path.c_str());

#ifdef __linux__
    FILE* fp = fopen(path.c_str(), "r");    // non-Windows use "r"
#else
    FILE* fp = fopen(path.c_str(), "rb");    // non-Windows use "r"
#endif

    std::vector<char> readBuffer(static_cast<unsigned int>(size));

    rapidjson::FileReadStream is(fp, readBuffer.data(),
                                 static_cast<unsigned int>(size));

    rapidjson::Document document;
    document.ParseStream(is);

    assert(document.HasMember("wrap_s"));
    assert(document.HasMember("wrap_t"));
    assert(document.HasMember("min_filter"));
    assert(document.HasMember("mag_filter"));

    // help
    std::string pp       = path.c_str();
    auto        img_path = pp.substr(0, pp.size() - 4) + ".img";

    std::ifstream corgi_image(img_path,
                              std::ifstream::binary | std::ifstream::in);

    int w;
    int h;
    int channels;

    corgi_image.read(reinterpret_cast<char*>(&w), sizeof w);
    corgi_image.read(reinterpret_cast<char*>(&h), sizeof h);
    corgi_image.read(reinterpret_cast<char*>(&channels), sizeof channels);

    unsigned char* pixels = new unsigned char[w * h * 4];

    corgi_image.read(reinterpret_cast<char*>(pixels), w * h * 4);

    width_      = w;
    height_     = h;
    min_filter_ = parse_min_filter(document["min_filter"].GetString());
    mag_filter_ = parse_mag_filter(document["mag_filter"].GetString());
    wrap_s_     = load_wrap(document["wrap_s"].GetString());
    wrap_t_     = load_wrap(document["wrap_t"].GetString());

    // log_info("texture Constructor for "+path);

    id_ = RenderCommand::generate_texture_object();

    RenderCommand::bind_texture_object(id_);
    RenderCommand::texture_parameter(min_filter_);
    RenderCommand::texture_parameter(mag_filter_);
    RenderCommand::texture_wrap_s(wrap_s_);
    RenderCommand::texture_wrap_t(wrap_t_);

    RenderCommand::initialize_texture_object(
        format::rgba, internal_format::rgba, width_, height_,
        data_type::unsigned_byte, pixels);

    RenderCommand::end_texture();
    fclose(fp);
    delete[] pixels;
}

texture::texture(texture&& texture) noexcept
    : name_(std::move(texture.name_))
    , id_(texture.id_)
    , min_filter_(texture.min_filter_)
    , mag_filter_(texture.mag_filter_)
    , wrap_s_(texture.wrap_s_)
    , wrap_t_(texture.wrap_t_)
    , width_(texture.width_)
    , height_(texture.height_)
{
    // log_info("texture Move Constructor for "+ name_);

    texture.id_         = 0u;
    texture.width_      = static_cast<unsigned short>(0);
    texture.height_     = static_cast<unsigned short>(0);
    texture.min_filter_ = min_filter::nearest;
    texture.mag_filter_ = mag_filter::nearest;
    texture.wrap_s_     = wrap::repeat;
    texture.wrap_t_     = wrap::repeat;
}

texture& texture::operator=(texture&& texture) noexcept
{
    // log_info("Move Affectation texture for "+ name_);

    if(id_ != 0)
        RenderCommand::delete_texture_object(id_);

    name_       = std::move(texture.name_);
    id_         = texture.id_;
    min_filter_ = texture.min_filter_;
    mag_filter_ = texture.mag_filter_;
    wrap_s_     = texture.wrap_s_;
    wrap_t_     = texture.wrap_t_;
    width_      = texture.width_;
    height_     = texture.height_;

    texture.id_         = 0u;
    texture.width_      = static_cast<unsigned short>(0);
    texture.height_     = static_cast<unsigned short>(0);
    texture.min_filter_ = min_filter::nearest;
    texture.mag_filter_ = mag_filter::nearest;
    texture.wrap_s_     = wrap::repeat;
    texture.wrap_t_     = wrap::repeat;

    return *this;
}

const char* texture::name() const
{
    return name_.c_str();
}

texture::texture(const std::string& name,
                 unsigned           width,
                 unsigned           height,
                 min_filter         min_f,
                 mag_filter         mag_f,
                 wrap               wrap_s,
                 wrap               wrap_t,
                 format             format,
                 internal_format    internal_format,
                 data_type          dt,
                 unsigned char*     data)
    : name_(name)
    , id_(RenderCommand::generate_texture_object())
    , min_filter_(min_f)
    , mag_filter_(mag_f)
    , wrap_s_(wrap_s)
    , wrap_t_(wrap_t)
    , width_(static_cast<unsigned short>(width))
    , height_(static_cast<unsigned short>(height))
{
    // log_info("texture Constructor for "+name);

    RenderCommand::bind_texture_object(id_);
    RenderCommand::initialize_texture_object(format, internal_format, width,
                                             height, dt, data);

    RenderCommand::texture_parameter(min_f);
    RenderCommand::texture_parameter(mag_f);
    RenderCommand::texture_wrap_s(wrap_s);
    RenderCommand::texture_wrap_t(wrap_t);
    RenderCommand::end_texture();
}

texture::~texture()
{
    // log_info("texture Destructor for "+name_);
    RenderCommand::delete_texture_object(id_);
}

bool texture::operator==(const texture& other) const noexcept
{
    if(name_ != other.name())
        return false;

    // I wonder if the id is enough here?
    if(id_ != other.id_)
        return false;

    if(width_ != other.width())
        return false;

    if(height_ != other.height())
        return false;

    if(min_filter_ != other.min_filter_)
        return false;

    if(mag_filter_ != other.mag_filter_)
        return false;

    if(wrap_s_ != other.wrap_s_)
        return false;

    if(wrap_t_ != other.wrap_t_)
        return false;

    return true;
}

bool texture::operator<(const texture& other) const noexcept
{
    // Maybe the id is enough to compare textures
    if(id_ != other.id_)
        return id_ < other.id_;

    if(name_ != other.name_)
        return name_ < other.name_;

    if(width_ != other.width_)
        return width_ < other.width_;

    if(height_ != other.height_)
        return height_ < other.height_;

    if(min_filter_ != other.min_filter_)
        return min_filter_ < other.min_filter_;

    if(mag_filter_ != other.mag_filter_)
        return mag_filter_ < other.mag_filter_;

    if(wrap_s_ != other.wrap_s_)
        return wrap_s_ < other.wrap_s_;

    if(wrap_t_ != other.wrap_t_)
        return wrap_t_ < other.wrap_t_;

    return true;
}

long long texture::memory_usage() const
{
    return sizeof(texture) + width_ * height_ * 4;
}

void texture::apply_changes()
{
    if(id_ == 0)
        log_warning("Could not apply texture's changes");

    RenderCommand::bind_texture_object(id_);
    RenderCommand::texture_parameter(min_filter_);
    RenderCommand::texture_parameter(mag_filter_);
    RenderCommand::texture_wrap_s(wrap_s_);
    RenderCommand::texture_wrap_t(wrap_t_);
    RenderCommand::end_texture();
}

unsigned texture::id() const noexcept
{
    return id_;
}

texture::min_filter texture::min_filter() const noexcept
{
    return min_filter_;
}

texture::mag_filter texture::mag_filter() const noexcept
{
    return mag_filter_;
}

texture::wrap texture::wrap_s() const noexcept
{
    return wrap_s_;
}

texture::wrap texture::wrap_t() const noexcept
{
    return wrap_t_;
}

unsigned texture::width() const noexcept
{
    return width_;
}

unsigned texture::height() const noexcept
{
    return height_;
}

void texture::width(unsigned width) noexcept
{
    width_ = width;
}

void texture::height(unsigned height) noexcept
{
    height_ = height;
}

//
//
// static corgi::containers::std::vector<std::string> wraps
//{
//    "clamp_to_border",
//    "clamp_to_edge",
//    "mirrored_repeat",
//    "mirror_clamp_to_edge",
//    "repeat"
//};
//
// static Vector<std::string> min_filters
//{
//    "nearest",
//    "linear",
//    "nearest_mipmap_nearest",
//    "nearest_mipmap_linear",
//    "linear_mipmap_linear",
//    "linear_mipmap_nearest"
//};
//
// static Vector<std::string> mag_filters
//{
//    "nearest",
//    "linear"
//};
//
//// If str exist inside strings, returns its index
// int get_index(Vector<std::string>& strings, const std::string& str)
//{
//     const auto iterator = std::find(strings.begin(), strings.end(), str);
//
//     if (iterator == strings.end())
//     {
//        // log_warning("Could not find" + str + " index inside the Vector");
//         return 0;
//     }
//     return std::distance(strings.begin(), iterator);
// }
//
// static std::string min_filter_to_string(texture::min_filter filter)
//{
//     return min_filters[static_cast<int>(filter)];
// }
//
// static texture::min_filter string_to_min_filter(const std::string& str)
//{
//     return static_cast<texture::min_filter>(get_index(min_filters, str));
// }
//
// static std::string mag_filter_to_string(texture::mag_filter filter)
//{
//     return mag_filters[static_cast<int>(filter)];
// }
//
// static texture::mag_filter string_to_mag_filter(const std::string& str)
//{
//     return static_cast<texture::mag_filter>(get_index(mag_filters, str));
// }
//
// static std::string wrap_to_string(texture::wrap wrp)
//{
//     return wraps[static_cast<int>(wrp)];
// }
//
// static texture::wrap string_to_wrap(const std::string& str)
//{
//     return static_cast<texture::wrap>(get_index(wraps, str));
// }
//
// void texture::load( const std::string& file)
//{
//     //_name = file;
//
//     //std::string path = ResourceFolder::instance()->folder() + "/images/" +
//     file;
//
//     //// We also try to load the .asset file. If it doesn't exist, it gets
//     //// created
//
//     //filesystem::ini::Document doc;
//     //ConstString asset_path = path + ".asset";
//
//     //if (filesystem::file_exist(asset_path))
//     //{
//     //    doc.read(asset_path);
//
//     //    _wrap_s = string_to_wrap(doc.value("wrap_s", "wrap").as_string());
//     //    _wrap_t = string_to_wrap(doc.value("wrap_t", "wrap").as_string());
//     //    _min_filter = string_to_min_filter(doc.value("min_filter",
//     "filters").as_string());
//     //    _mag_filter = string_to_mag_filter(doc.value("mag_filter",
//     "filters").as_string());
//
//     //}
//     //else  // No asset file as been founded, default one created
//     //{
//     //    doc.set_value(wrap_to_string(_wrap_s), "wrap_s", "wrap");
//     //    doc.set_value(wrap_to_string(_wrap_t), "wrap_t", "wrap");
//     //    doc.set_value(min_filter_to_string(_min_filter), "min_filter",
//     "filters");
//     //    doc.set_value(mag_filter_to_string(_mag_filter), "mag_filter",
//     "filters");
//
//     //    doc.write(asset_path);
//     //}
//
//     //Image img;
//     //img.load_from_png_file(path.c_str());
//     //image(img);
// }

void texture::min_filter(min_filter filter) noexcept
{
    min_filter_ = filter;
}

void texture::mag_filter(mag_filter filter) noexcept
{
    mag_filter_ = filter;
}

void texture::wrap_s(wrap wrap) noexcept
{
    wrap_s_ = wrap;
}

void texture::wrap_t(wrap wrap) noexcept
{
    wrap_t_ = wrap;
}

float texture::ratio() const
{
    return static_cast<float>(width_) / static_cast<float>(height_);
}

void texture::unpack_pixels()
{
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

// void texture::build_texture(int width, int height)
// {
//    /* _width = width;
// 	_height = height;

// 	glTexImage2D
// 	(
// 		GL_TEXTURE_2D,
// 		0,
// 		GL_RGBA,
// 		_width,
// 		_height,
// 		0,
// 		GL_RGBA,
// 		GL_UNSIGNED_BYTE,
// 		0
// 	);
// 	check_gl_error();*/
// }
