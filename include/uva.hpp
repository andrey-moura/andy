#pragma once

#ifdef NDEBUG
    #undef __UVA_DEBUG__
    #define __UVA_DEBUG_LEVEL_DEFAULT__ 0
#else
    #define __UVA_DEBUG__ 1
    #define __UVA_DEBUG_LEVEL_DEFAULT__ 1
#endif

#ifdef _MSC_VER 
    #define __UVA_WIN__
#endif

#ifdef _MSC_VER 
    #define UVA_FUNCTION_NAME __func__
#else
   #define UVA_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#if __UVA_OVERRIDE_DEBUG_LEVEL__ > 0
    constexpr size_t uva_debug_level = __UVA_OVERRIDE_DEBUG_LEVEL__;
    #define __UVA_DEBUG_LEVEL__ __UVA_OVERRIDE_DEBUG_LEVEL__
#else 
    constexpr size_t uva_debug_level = __UVA_DEBUG_LEVEL_DEFAULT__;

    #define __UVA_DEBUG_LEVEL__ __UVA_DEBUG_LEVEL_DEFAULT__
#endif

#if __UVA_DEBUG_LEVEL__ > 0
    #if __has_include(<console.hpp>)
    #   include <console.hpp>
    #endif

    template<int level>
    inline void UVA_CHECK_FAILED_F(const std::string& name, const std::string& file, const size_t line)
    {
        if constexpr (level == 1) {
            #if __has_include(<console.hpp>)
                uva::console::log_warning("CHECK FAILED: {} (level {}) at file {}:{}", name, level, file, line);
            #endif
        }
    }
#else
    template<int level>
    inline void UVA_CHECK_FAILED_F(const std::string& name, const std::string& file, const size_t line)
    {
        
    }
#endif

#define UVA_CHECK_FAILED(level, name) UVA_CHECK_FAILED_F<level>(name, file, line);
#define UVA_GENERATE_CHECK_PARAMS(...) __VA_ARGS__
#define UVA_GENERATE_CHECK(name, level, params, ...) inline void name##_F (params, const std::string& file, const size_t& line) {  if constexpr (uva_debug_level >= level) { __VA_ARGS__ } }

UVA_GENERATE_CHECK(UVA_CHECK_RESERVED_BUFFER, 1, UVA_GENERATE_CHECK_PARAMS(const std::string& buffer, size_t len), if(buffer.size() > len) { UVA_CHECK_FAILED(1, "UVA_CHECK_RESERVED_BUFFER") })
#define UVA_CHECK_RESERVED_BUFFER(buffer, len) UVA_CHECK_RESERVED_BUFFER_F(buffer, len, __FILE__, __LINE__);

namespace uva
{
    template <auto left, auto right, typename = void>
        struct function_is_same : std::false_type {};

    template <auto left, auto right>
        struct function_is_same<left, right, std::enable_if_t<left == right>> : std::true_type {};
};