#include <colormap.hpp>

#include <stdexcept>

#include <file.hpp>

uva::drawing::color_map_iterator::color_map_iterator(uint8_t* __data, size_t __bpp)
    : m_data(__data), m_bits_per_pixel(__bpp), m_mask(uva::drawing::color_map::mask_by_bpp(__bpp)), m_mask_increment(0)
{

}

uint8_t uva::drawing::color_map_iterator::index() const
{
    return *m_data & (m_mask<<m_mask_increment);
}

void uva::drawing::color_map_iterator::advance()
{
    m_mask_increment+=m_bits_per_pixel;

    if(m_bits_per_pixel >= 8) {
        m_mask_increment = 0;
        ++m_data;
    }
}

uva::drawing::palette uva::drawing::color_map::from_rgba(const uva::color* data, const uva::size& size)
{
    m_size = size;
    uva::drawing::palette palette;

    size_t pixel_count = size.w * size.h;

    for(size_t i = 0; i < pixel_count; ++i) {
        palette.push(data[i]);
    }

    m_bits_per_pixel = closest_bpp(palette.size());
    m_pixels_per_byte = CHAR_BIT/m_bits_per_pixel;
    m_mask = mask_by_bpp(m_bits_per_pixel);

    size_t byte_count = pixel_count / m_pixels_per_byte;
    m_data.reserve(byte_count);

    uint8_t cur_byte = 0;
    size_t bit_count = 0;

    size_t last = palette.last_index();

    for(size_t i = 0; i < pixel_count; ++i) {
        if(bit_count < 8) {
            cur_byte = cur_byte << m_bits_per_pixel;

            size_t index = palette.find(data[i]);

            if(index != 0) 
            {
                std::string();
            }

            if(index == std::string::npos || index >= 255) {
                index = last;
            }

            cur_byte = cur_byte | (uint8_t)index;
            bit_count += m_bits_per_pixel;
        }

        if(bit_count == 8) {
            m_data.push_back(cur_byte);
            cur_byte = 0;

            bit_count = 0;
        } 
    }

    if(m_data.size() != byte_count) {
        throw std::runtime_error("failed to load pixels");
    }

    return palette;
}

uva::drawing::color_map_iterator uva::drawing::color_map::begin()
{
    return color_map_iterator(m_data.data(), m_bits_per_pixel);
}

uva::image uva::drawing::color_map::to_image(const uva::drawing::palette& palette)
{
    size_t byte_count = m_size.area();

    uva::color* colors = new uva::color[byte_count];

    auto it = begin();

    for(size_t i = 0; i < byte_count; ++i)
    {
        size_t c = it.index();
        if(c >= palette.size()) {
            c = palette.last_index();
        }
        colors[i] = palette[c];
        it.advance();
    }

    return uva::image(colors, m_size, false);
}

size_t uva::drawing::color_map::bpp_color_count(const size_t& bpp)
{
    return (size_t)((size_t)1<<bpp);
}

const std::vector<size_t>& uva::drawing::color_map::supported_bpps()
{
    static std::vector<size_t> bpps = {
        1, 2, 4, 8
    };

    return bpps;
}

const std::vector<size_t>& uva::drawing::color_map::bpps_color_count()
{
    static std::vector<size_t> bpps_count;

    if(bpps_count.empty()) {
        for(const auto& bpp : supported_bpps()) {
            bpps_count.push_back(bpp_color_count(bpp));
        }
    }

    return bpps_count;
}

size_t uva::drawing::color_map::closest_bpp(const size_t& color_count)
{
    const auto& bpps = supported_bpps();
    const auto& bpps_count = bpps_color_count();
    
    for(size_t i = 0; i < bpps.size(); ++i)
    {
        if(bpps_count[i] >= color_count) {
            return bpps[i];
        }
    }

    throw std::runtime_error("failed to find the closest bpp for " + std::to_string(color_count) + " colors");
}

size_t uva::drawing::color_map::mask_by_bpp(const size_t& bpp)
{
    return (1<<bpp)-1;
}

void uva::drawing::color_map::write_to_file(const std::string& path)
{
    uva::file::write_all<uint8_t>(path, m_data);
}

void uva::drawing::color_map::read_from_file(const std::string& path)
{

}
