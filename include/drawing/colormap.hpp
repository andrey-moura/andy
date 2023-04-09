#pragma once

#include <vector>
#include <algorithm>
#include <string>

#include <color.hpp>
#include <size.hpp>
#include <palette.hpp>

#include <image.hpp>

namespace uva
{
    namespace drawing
    {
        struct color_map_iterator
        {
            color_map_iterator(uint8_t* __data, size_t __bpp);
        private:
            uint8_t* m_data;
            size_t m_bits_per_pixel;
            size_t m_mask;
            size_t m_mask_increment;
        public:
            uint8_t index() const;
        public:
            void advance();
        };

        class color_map
        {
            std::vector<uint8_t> m_data;
            uva::size m_size;
            size_t m_mask;
            size_t m_bits_per_pixel;
            size_t m_pixels_per_byte;
        public:
            uva::drawing::palette from_rgba(const uva::color* data, const uva::size& size);

            void write_to_file(const std::string& path);
            void read_from_file(const std::string& path);

            uva::image to_image(const uva::drawing::palette& palette);

            color_map_iterator begin();
        public:
            static size_t bpp_color_count(const size_t& bpp);
            static const std::vector<size_t>& supported_bpps();
            static const std::vector<size_t>& bpps_color_count();
            static size_t closest_bpp(const size_t& color_count);
            static size_t mask_by_bpp(const size_t& bpp);

        }; // namespace color_map
        
    }; // namespace drawing
    
}; // namespace uva
