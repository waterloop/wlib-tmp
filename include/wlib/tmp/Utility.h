#ifndef __WLIB_TMP_UTILITY_H__
#define __WLIB_TMP_UTILITY_H__

#include <wlib/tmp/TypeTraits.h>

namespace wlp {

    // move
    template<typename T>
    constexpr typename remove_reference<T>::type &&move(T &&t) noexcept
    { return static_cast<typename remove_reference<T>::type &&>(t); }

    // forward
    template<typename T>
    constexpr T &&forward(typename remove_reference<T>::type &t) noexcept
    { return static_cast<T &&>(t); }
    template<typename T>
    constexpr T &&forward(typename remove_reference<T>::type &&t) noexcept
    { return static_cast<T &&>(t); }

    // is_same
    template<typename, typename>    struct is_same          : public false_type {};
    template<typename T>            struct is_same<T, T>    : public true_type {};

    // enable_if
    template<bool, typename = void> struct enable_if            {};
    template<typename T>            struct enable_if<true, T>   { typedef T type; };

}

#endif
