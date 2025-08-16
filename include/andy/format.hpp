#ifdef __linux__
    #define FMT_HEADER_ONLY
    #include "fmt/include/fmt/format.h"

    #define USE_FMT_FORMT

    namespace std
    {
        using namespace fmt;
    };
#else
    #include <format>
#endif