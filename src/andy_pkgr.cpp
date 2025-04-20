#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {

    bool push = false;
    int version_to_update = -1;

    for(int i = 1; i < argc; i++) {
        std::string_view arg = argv[i];
        if(arg == "--major") {
            version_to_update = 0;
        } else if(arg == "--minor") {
            version_to_update = 1;
        } else if(arg == "--patch") {
            version_to_update = 2;
        }
        else if(arg == "--push") {
            push = true;
        } else {
            std::cerr << "Invalid argument: '" << arg << "'" << std::endl;
            return 1;
        }
    }
    
    std::filesystem::path path = std::filesystem::current_path();
    std::filesystem::path version_path = path / "VERSION";

    if(!std::filesystem::exists(version_path)) {
        std::cerr << "Version file not found" << std::endl;
        return 1;
    }

    std::ifstream file(version_path);
    std::string version;
    int versions[3];
    std::getline(file, version);

    std::string_view version_view(version);

    std::string_view versions_str[3];

    auto invalid_version_format = [&]() {
        std::cerr << "Invalid version: '" << version << "'" << std::endl;
        exit(1);
    };
    auto print_version = [&]() {
        std::cout << "\tMajor: " << versions[0] << std::endl;
        std::cout << "\tMinor: " << versions[1] << std::endl;
        std::cout << "\tPatch: " << versions[2] << std::endl;
    };
    if(version_view.ends_with("-beta")) {
        version_view.remove_suffix(5);
    }
    for(int i = 0; i < 3; i++) {
        const char* start = version_view.data();
        const char* end = nullptr;
        while(version_view.size()) {
            if(version_view[0] == '.') {
                end = version_view.data();
                version_view.remove_prefix(1);
                break;
            }

            if(!std::isdigit(version_view[0])) {
                invalid_version_format();
            }

            version_view.remove_prefix(1);
        }

        if(i == 2 && !end) {
            end = version_view.data();
        } else if(!version_view.size()) {
            invalid_version_format();
        }

        versions_str[i] = std::string_view(start, end - start);

        versions[i] = std::stoi(std::string(versions_str[i]));
    }

    if(version_view.size()) {
        invalid_version_format();
    }

    std::cout << "Current version: " << std::endl;
    print_version();
    std::cout << std::endl;

    if(version_to_update >= 0) {
        std::cout << "Updating to:" << std::endl;

        versions[version_to_update]++;
        print_version();
    } else {
        std::cout << "Not updating version" << std::endl;
    }

    std::string package_name = path.filename().string();
    std::string version_folder_name = package_name;
    std::string version_str;
    version_str.reserve(10);
    for(int i = 0; i < 3; i++) {
        version_str += std::to_string(versions[i]);
        if(i < 2) {
            version_str += ".";
        }
    }
    version_folder_name.push_back('-');
    version_folder_name.append(version_str);
    std::filesystem::path version_folder = path / version_folder_name;
    if(std::filesystem::exists(version_folder)) {
        std::cerr << "Version folder already exists" << std::endl;
        return 1;
    }
    std::filesystem::create_directory(version_folder);
    std::filesystem::path src_dir;
    std::filesystem::path bin_dir;
    std::filesystem::path include_dir;
#ifdef __linux__
    src_dir = version_folder / "usr/local/src";
    src_dir /= package_name;
    bin_dir = version_folder / "usr/local/bin";
    include_dir = version_folder / "usr/local/include";
#else
    std::cerr << "Not implemented." << std::endl;
    return 1;
#endif
    std::filesystem::create_directories(src_dir);
    std::filesystem::create_directories(bin_dir);
    std::filesystem::create_directories(include_dir);

    for(auto file : std::filesystem::directory_iterator(path)) {
        if(!file.is_regular_file()) {
            if(file.path().filename() == "build" || file.path().filename() == "logs") {
                continue;
            }
        } else {
            if(file.path().filename() == "control") {
                continue;
            }
        }
        std::filesystem::copy(file, src_dir / file.path().filename());
    }

    std::filesystem::copy(path / "include", include_dir, std::filesystem::copy_options::recursive);

    std::vector<std::string_view> extensions_to_copy_in_build_folder;
    
    std::filesystem::path build_dir;
#ifdef __linux__
    build_dir = path / "build";
    extensions_to_copy_in_build_folder = {""};
#else
    std::cerr << "Not implemented." << std::endl;
    return 1;
#endif

    for(auto file : std::filesystem::directory_iterator(build_dir)) {
        if(file.is_regular_file()) {
            if(file.path().stem() == "CMakeCache" || file.path().stem() == "Makefile") {
                continue;
            }
            for(auto extension : extensions_to_copy_in_build_folder) {
                if(file.path().extension() == extension) {
                    std::filesystem::copy(file, bin_dir / file.path().filename());
                }
            }
        }
    }
#ifdef __linux__
    std::filesystem::path control_file_path = version_folder / "DEBIAN";
    std::filesystem::create_directories(control_file_path);
    std::ofstream control(control_file_path / "control");
    control << "Package: " << package_name << std::endl;
    control << "Version: " << version_str << std::endl;
    control << "Architecture: all" << std::endl;
#endif
    std::ofstream version_file(src_dir / "VERSION", std::ios::trunc);
    version_file << version_str;
    version_file.close();
#ifdef __linux__
    std::string command = "dpkg-deb --build " + version_folder.string();
    if(system(command.c_str())) {
        return 1;
    }
#else
    std::cerr << "Not implemented." << std::endl;
    return 1;
#endif
    std::filesystem::copy_file(src_dir / "VERSION", path / "VERSION", std::filesystem::copy_options::overwrite_existing);
    std::filesystem::remove_all(version_folder);

    if(push) {
        // ssh to server
#ifdef __linux__
        std::string ssh_target = "ubuntu@andy-lang.org";
        std::string folder = "/home/ubuntu/releases/" + package_name + "/" + version_str;
        // Create the folder on the server
        std::string command = "ssh ubuntu@andy-lang.org 'mkdir -p " + folder + "'";
        std::cout << "Creating folder on server..." << std::endl;
        if(system(command.c_str())) {
            return 1;
        }
        command = "scp " + version_folder.string() + ".deb " + ssh_target + ":" + folder + "/" + version_folder.filename().string() + ".deb";
        std::cout << "Pushing to server..." << std::endl;
        if(system(command.c_str())) {
            return 1;
        }
        return 0;
#else
        std::cerr << "Not implemented." << std::endl;
        return 1;
#endif
    }
    return 0;
}