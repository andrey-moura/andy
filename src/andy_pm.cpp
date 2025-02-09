#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <uva/format.hpp>
#ifdef __linux__
#   include <unistd.h>
#endif

#include <console.hpp>

std::vector<std::string> trusted_providers = {
    "andrey-moura",
};

std::filesystem::path temp = std::filesystem::temp_directory_path() / "uva-pm.txt";
std::filesystem::path src_dir;
std::filesystem::path current_path;

std::string invocation;

bool debug = false;
bool recursive = false;

int system_quiet(const std::string& command, std::string* __output = nullptr) {
    std::string actual_command = command + " > " + temp.string() + " 2>&1";
    if(debug) {
        std::cout << "Executing " << actual_command << std::endl;
    }
    int result = system(actual_command.c_str());

    std::string output;
    if(debug || (!result && __output)) {
        std::ifstream file(temp);
        file.seekg(0, std::ios::end);
        output.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        output.resize(file.tellg());

        file.read(output.data(), output.size());

        file.close();

        if(debug) {
            std::cout << output;
        }
    }

    if(!result && __output) {
        *__output = std::move(output);
    }

    return result;
}

void navigate(const std::filesystem::path& path) {
    if(debug) {
        std::cout << "Navigating to " << path.string() << std::endl;
    }
    std::filesystem::current_path(path);
}

int install_dependency(const std::string& dependency_name) {
    std::filesystem::path dependency_path = src_dir / dependency_name;

    std::cout << dependency_name << "...";

    if(!std::filesystem::exists(dependency_path)) {
        if(recursive) {
            navigate(current_path);

            std::cout << " Installing...";

            int result = system_quiet(std::format("{} {} -r", invocation, dependency_name));
            if(result) {
                uva::console::log_error(" Failed");
                std::cout << std::endl;

                std::cout << "See " << temp.string() << " or run with -d (or --debug) for more information" << std::endl;

                return result;
            } else {
                uva::console::log_success(" Ok");
            }
        } else {
            uva::console::log_error(" Missing");
            return 1;
        }
    }

    return 1;
}

int main(int argc, char* argv[]) {
    bool has_unresolved_dependencies = false;
    bool update_only = false;
#ifdef __linux__
    src_dir = std::filesystem::absolute("/usr/local/src");
#else
    throw std::runtime_error("Not implemented");
#endif
    current_path = std::filesystem::current_path();
    
    invocation = argv[0];
#ifdef __linux__
    if(invocation.starts_with("build")) {
        invocation = "./" + invocation;
    }
#endif

    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <provider/package>" << std::endl;
        return 1;
    }

    for(size_t i = 2; i < argc; i++) {
        std::string_view arg = argv[i];
        if(arg == "-r" || arg == "--recursive") {
            recursive = true;
        }
        else if(arg == "-d" || arg == "--debug") {
            debug = true;
        } else if(arg == "-u" || arg == "--update") {
            update_only = true;
        }
    }

    std::string_view provider_package = argv[1];
    size_t pos = provider_package.find('/');
    
    std::string_view provider;
    std::string_view package;
    
    if(pos == std::string::npos) {
        provider = "andrey-moura";
        package = provider_package;
    } else {
        package  = provider_package.substr(pos + 1);
        provider = provider_package.substr(0, pos);
    }

    std::cout << "Provider: " << provider;

    if(std::find(trusted_providers.begin(), trusted_providers.end(), provider) != trusted_providers.end()) {
        uva::console::log_success(" (trusted)");
    } else {
        uva::console::log_warning(" (untrusted)");
    }
    
    std::cout << "Package:  " << package << std::endl;

    std::cout << std::endl;

#ifdef __linux__
    std::cout << "Checking for sudo permissions...";

    if(geteuid()) {
        uva::console::log_error(" Run as sudo");
        return 1;
    }
    uva::console::log_success(" Ok");
#endif

    std::cout << "Checking for git...";

    int git_version = system_quiet("git --version");

    if(git_version) {
        uva::console::log_warning("Git is not installed or not in your PATH");
        return git_version;
    }

    uva::console::log_success(" Ok");

    std::cout << "Checking for CMake...";

    int cmake_version = system_quiet("cmake --version");

    if(cmake_version) {
        uva::console::log_warning("CMake is not installed or not in your PATH");
        return cmake_version;
    }

    uva::console::log_success(" Ok");

    std::cout << std::endl;

    std::filesystem::path repository_folder = src_dir / package;

    if(!update_only) {
        std::string repository_url = std::format("https://www.github.com/{}/{}", provider, package);

        std::cout << "Cloning " << repository_url << "...";

        if(std::filesystem::exists(repository_folder)) {
            std::filesystem::remove_all(repository_folder);
        }

        std::string command = std::format("git clone {} {} --recursive", repository_url, repository_folder.string());
        
        int result = system_quiet(command.c_str());

        uva::console::log_success(" Ok");

        std::cout << std::endl;
    }

    navigate(repository_folder);

    if(update_only) {
        std::cout << "Updating " << package << "...";

        std::string output;
        int result = system_quiet("git pull", &output);

        if(result) {
            uva::console::log_error(" Failed");
            std::cout << std::endl;

            std::cout << "See " << temp.string() << " or run with -d (or --debug) for more information" << std::endl;

            return result;
        } else {
            uva::console::log_success(" Ok");
        }

        std::cout << std::endl;
    }

    std::cout << "Checking dependencies..." << std::endl;

    std::filesystem::path package_file = repository_folder / "uva-pm.json";

    // if(std::filesystem::exists(package_file)) {
    //     std::string content = uva::file::read_all_text<char>(package_file);

    //     var package_json = uva::json::decode(content);

    //     var dependencies = package_json["dependencies"];

    //     if(dependencies) {
    //         if(!dependencies.is_a<var::array>()) {
    //             uva::console::log_error("Dependencies must be an array");
    //             return 1;
    //         }
    //         for(auto& dependency : dependencies.as<var::array>()) {
    //             if(!dependency.is_a<var::string>()) {
    //                 uva::console::log_error("Dependencies must be strings");
    //                 return 1;
    //             }

    //             if(!recursive) {
    //                 has_unresolved_dependencies = true;
    //                 break;
    //             }

    //             const std::string& dependency_name = dependency.as<var::string>();

    //             int result = install_dependency(dependency_name);

    //             if(result) {
    //                 uva::console::log_error(" Failed");
    //             } else {
    //                 uva::console::log_success(" Ok");
    //             }
    //         }

    //         std::cout << std::endl;

    //         if(has_unresolved_dependencies) {
    //             uva::console::log_error("Unresolved dependencies. If you want to recursively install dependencies, use the -r (or --recursive) flag");
    //             return 1;
    //         }
    //     }
    // }

    navigate(repository_folder);

    std::cout << "Building " << package << "..." << std::endl;

    std::filesystem::create_directory("build");

    if(!system("cmake -S . -B build")) {
        return 1;
    }

    if(!system("cmake --build build --config Release --parallel")) {
        return 1;
    }

    if(!system("cmake --install build")) {
        return 1;
    }

    return 0;
}