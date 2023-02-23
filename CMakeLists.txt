#Require a minimum version
cmake_minimum_required(VERSION 3.16)

project(uva)

# Commum libs
find_package(Threads)

# Optional libs
# binary, networking
find_package(OpenSSL)

# cspec must be included before ALL projects
include("include/cspec/CMakeLists.txt")
include("include/binary/CMakeLists.txt")
include("include/faker/CMakeLists.txt")
include("include/routing/CMakeLists.txt")
include("include/string/CMakeLists.txt")
include("include/diagnostics/CMakeLists.txt")
include("include/console/CMakeLists.txt")
include("include/core/CMakeLists.txt")

include_directories(include/)