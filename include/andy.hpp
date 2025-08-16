#pragma once

#include <cstddef>
#include <string>
#include <map>
#include <utility>

#ifdef NDEBUG
    #undef __UVA_DEBUG__
#endif

#ifdef __UVA_DEBUG__
    #define __UVA_DEBUG_LEVEL_DEFAULT__ 1
#else
    #define __UVA_DEBUG_LEVEL_DEFAULT__ 0
#endif

#ifdef _MSC_VER 
    #define __UVA_WIN__
    #define UVA_FUNCTION_NAME __func__
    #define ANDY_EXPORT_SYMBOL __declspec(dllexport)
#else
    #define ANDY_EXPORT_SYMBOL __attribute__((visibility("default")))
    #define UVA_FUNCTION_NAME __PRETTY_FUNCTION__
#endif

#if __cplusplus >= 202002L
    #define __UVA_CPP20__
#endif

#if __cplusplus >= 201703L
    #define __UVA_CPP17__
#endif

#if __UVA_OVERRIDE_DEBUG_LEVEL__ > 0
    constexpr size_t andy_debug_level = __UVA_OVERRIDE_DEBUG_LEVEL__;
    #define __UVA_DEBUG_LEVEL__ __UVA_OVERRIDE_DEBUG_LEVEL__
#else 
    constexpr size_t andy_debug_level = __UVA_DEBUG_LEVEL_DEFAULT__;

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
                andy::console::log_warning("CHECK FAILED: {} (level {}) at file {}:{}", name, level, file, line);
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
#define UVA_GENERATE_CHECK(name, level, params, ...) inline void name##_F (params, const std::string& file, const size_t& line) {  if constexpr (andy_debug_level >= level) { __VA_ARGS__ } }

UVA_GENERATE_CHECK(UVA_CHECK_RESERVED_BUFFER, 1, UVA_GENERATE_CHECK_PARAMS(const std::string& buffer, size_t len), if(buffer.size() > len) { UVA_CHECK_FAILED(1, "UVA_CHECK_RESERVED_BUFFER") })
#define UVA_CHECK_RESERVED_BUFFER(buffer, len) UVA_CHECK_RESERVED_BUFFER_F(buffer, len, __FILE__, __LINE__);

namespace andy
{
    template <auto left, auto right, typename = void>
    struct function_is_same : std::false_type {};

    template <auto left, auto right>
    struct function_is_same<left, right, std::enable_if_t<left == right>> : std::true_type {};

    template<typename T>
    struct has_const_iterator
    {
    private:
        typedef char                      yes;
        typedef struct { char array[2]; } no;

        template<typename C> static yes test(typename C::const_iterator*);
        template<typename C> static no  test(...);
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(yes);
        typedef T type;
    };

    template <typename T>
    struct has_begin_end
    {
        template<typename C> static char (&f(typename std::enable_if<
        std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
        typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

        template<typename C> static char (&f(...))[2];

        template<typename C> static char (&g(typename std::enable_if<
        std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
        typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

        template<typename C> static char (&g(...))[2];

        static bool const beg_value = sizeof(f<T>(0)) == 1;
        static bool const end_value = sizeof(g<T>(0)) == 1;
    };

    template<typename T> 
    struct is_container : std::integral_constant<bool, has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value> 
    { };

    template <typename T, typename = int>
    struct is_reservable : std::false_type { };

    template <typename T>
    struct is_reservable <T, decltype(&T::reserve, 0)> : std::true_type { };

    template <typename>
    struct is_pair : std::false_type { };

    template <typename k, typename v>
    struct is_pair<std::pair<k, v>> : std::true_type { };

    template <typename>
    struct is_map : std::false_type { };

    template <typename k, typename v>
    struct is_map<std::map<k, v>> : std::true_type { };

    template<typename T>
    struct is_pointer { static const bool value = false; };

    template<typename T>
    struct is_pointer<T*> { static const bool value = true; };

    template<typename C, typename V>
    void add_to_container(C& container, V&& value) {
        if constexpr(is_map<C>::value)
        {  
            container.insert(std::forward<V>(value));
        } else
        {
            container.push_back(std::forward<V>(value));
        }
    }

    template<typename T>
    struct rebinder;

    template<template<typename...> typename T, typename... Args>
    struct rebinder<T<Args...>> {
        template<typename... Us>
        using rebind = T<Us...>;
    };

#ifdef __UVA_CPP20__
    template<typename T, typename... Us>
    using rebound = rebinder<T>::template rebind<Us...>;
#endif
}