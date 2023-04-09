#pragma once

#include <vector>
#include <algorithm>
#include <string>

#include <color.hpp>

namespace uva
{
    namespace drawing
    {
        class palette
        {
            std::vector<uva::color> colors;
            public:
                void push(const uva::color& c);
                size_t find(const uva::color& c) const;
                size_t last_index() const;

                size_t size() const;
                const uva::color* c_data() const;
                const std::vector<uva::color>& data() const;
                
                void read_from_file(const std::string& path);
                void write_to_file(const std::string& path);

                const uva::color& operator[](const size_t& i) const;
        }; // namespace palette
        
    }; // namespace drawing
    
}; // namespace uva
