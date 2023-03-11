#include <file.hpp>

void uva::file::write_all_bytes(const std::filesystem::path& path, const uint8_t* bytes, size_t len)
{
    write_all_bytes(path.string(), bytes, len);
}

void uva::file::write_all_bytes(const std::string& path, const uint8_t* bytes, size_t len)
{
    std::ofstream stream(path, std::ios::binary);
    stream.write((char*)bytes, len);
}

void uva::file::insert_text(const std::filesystem::path &path, size_t position, const char *text, size_t len)
{
    std::string content = read_all_text<char>(path);

    std::ofstream stream(path, std::ios::binary);
    stream.write(content.c_str(), std::min(content.size(), position));
    stream.write(text, len);

    if(position < content.size()) {
        stream.write(content.c_str()+position, content.size()-position);
    }
}
