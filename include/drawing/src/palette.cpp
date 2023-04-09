#include <drawing.hpp>

#include <file.hpp>

void uva::drawing::palette::push(const uva::color& c)
{
    auto it = std::lower_bound(colors.begin(), colors.end(), c );
    auto end = colors.end();
    uva::color* data = colors.data();

    if(it == end || *it != c)
    {
        colors.insert(it, c);
    }
}

size_t uva::drawing::palette::find(const uva::color& c) const
{
    auto it = std::lower_bound(colors.begin(), colors.end(), c );
    auto begin = colors.begin();
    auto end = colors.end();

    if(it == end || *it != c)
    {
        return std::string::npos;
    }

    return it - begin;
}

size_t uva::drawing::palette::last_index() const
{
    return colors.size()-1;
}

const std::vector<uva::color>& uva::drawing::palette::data() const
{
    return colors;
}

const uva::color* uva::drawing::palette::c_data() const
{
    return colors.data();
}

void uva::drawing::palette::read_from_file(const std::string& path)
{
    colors = uva::file::read_all<uva::color>(path);
}

void uva::drawing::palette::write_to_file(const std::string& path)
{
    uva::file::write_all(path, colors);
}

size_t uva::drawing::palette::size() const
{
    return colors.size();
}

const uva::color& uva::drawing::palette::operator[](const size_t& i) const
{
    return colors[i];
}