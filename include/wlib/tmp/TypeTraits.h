#ifndef __WLIB_TMP_TYPETRAITS_H__
#define __WLIB_TMP_TYPETRAITS_H__

#include <stdint.h>

#include <wlib/tmp/NullptrType.h>
#include <wlib/tmp/IntegralConstant.h>
#include <wlib/tmp/Condition.h>
#include <random>

namespace wlp {

    // remove const and volatile
    template<typename T> struct remove_const                { typedef T type; };
    template<typename T> struct remove_const<T const>       { typedef T type; };
    template<typename T> struct remove_volatile             { typedef T type; };
    template<typename T> struct remove_volatile<T volatile> { typedef T type; };
    template<typename T> struct remove_cv
    { typedef typename remove_const<typename remove_volatile<T>::type>::type type; };

    // is_void
    namespace det {
        template<typename>  struct is_void_helper       : public false_type {};
        template<>          struct is_void_helper<void> : public true_type {};
    }
    template<typename T> struct is_void :
        public det::is_void_helper<typename remove_const<T>::type>::type {};

    // is_lvalue_reference, is_rvalue_reference, is_reference
    template<typename>      struct is_lvalue_reference          : public false_type {};
    template<typename T>    struct is_lvalue_reference<T &>     : public true_type {};
    template<typename>      struct is_rvalue_reference          : public false_type {};
    template<typename T>    struct is_rvalue_reference<T &&>    : public true_type {};
    template<typename T> struct is_reference :
        public or_<is_lvalue_reference<T>, is_rvalue_reference<T>>::type {};

    // is_pointer
    namespace det {
        template<typename>      struct is_pointer_helper        : public false_type {};
        template<typename T>    struct is_pointer_helper<T *>   : public true_type {};
    }
    template<typename T> struct is_pointer :
        public det::is_pointer_helper<typename remove_cv<T>::type>::type {};

    // remove_pointer
    namespace det {
        template<typename T, typename>      struct remove_pointer_helper            { typedef T type; };
        template<typename T, typename U>    struct remove_pointer_helper<T, U *>    { typedef U type; };
    }
    template<typename T> struct remove_pointer :
        public det::remove_pointer_helper<T, typename remove_cv<T>::type> {};

    // remove_reference
    template<typename T> struct remove_reference        { typedef T type; };
    template<typename T> struct remove_reference<T &>   { typedef T type; };
    template<typename T> struct remove_reference<T &&>  { typedef T type; };

    // remove_extent
    template<typename T>            struct remove_extent        { typedef T type; };
    template<typename T>            struct remove_extent<T[]>   { typedef T type; };
    template<typename T, size_t sz> struct remove_extent<T[sz]> { typedef T type; };

    // is_array, is_sized_array, is_unsized_array
    template<typename>              struct is_array                 : public false_type {};
    template<typename T>            struct is_array<T[]>            : public true_type {};
    template<typename T, size_t sz> struct is_array<T[sz]>          : public true_type {};
    template<typename>              struct is_sized_array           : public false_type {};
    template<typename T>            struct is_sized_array<T[]>      : public false_type {};
    template<typename T, size_t sz> struct is_sized_array<T[sz]>    : public true_type {};
    template<typename>              struct is_unsized_array         : public false_type {};
    template<typename T>            struct is_unsized_array<T[]>    : public true_type {};
    template<typename T, size_t sz> struct is_unsized_array<T[sz]>  : public false_type {};

    // get_array_size
    template<typename> struct get_array_size {};
    template<typename T, size_t sz> struct get_array_size<T[sz]> :
        public integral_constant<size_t, sz>::type {};

    // is_function
    template<typename>                     struct is_function                       : public false_type {};
    template<typename R, typename... Args> struct is_function<R(Args...)>           : public true_type {};
    template<typename R, typename... Args> struct is_function<R(Args...) &>         : public true_type {};
    template<typename R, typename... Args> struct is_function<R(Args...) &&>        : public true_type {};
    template<typename R, typename... Args> struct is_function<R(Args...) const>     : public true_type {};
    template<typename R, typename... Args> struct is_function<R(Args...) const &>   : public true_type {};
    template<typename R, typename... Args> struct is_function<R(Args...) const &&>  : public true_type {};

    // is_object
    template<typename T> struct is_object :
        public not_<or_<is_function<T>, is_reference<T>, is_void<T>>>::type {};

    // is_referenceable
    template<typename T> struct is_referenceable : public or_<is_object<T>, is_reference<T>>::type {};
    template<typename R, typename... Args> struct is_referenceable<R(Args...)> : public true_type {};

    // add_pointer
    namespace det {
        template<typename T, bool = or_<is_referenceable<T>, is_void<T>>::value>
        struct add_pointer_helper           { typedef T type; };
        template<typename T>
        struct add_pointer_helper<T, true>  { typedef typename remove_reference<T>::type *type; };
    }
    template<typename T> struct add_pointer : public det::add_pointer_helper<T> {};

    // decay
    namespace det {
        template<typename T, bool = is_array<T>::value, bool = is_function<T>::value> struct decay_helper;
        template<typename T> struct decay_helper<T, false, false> { typedef typename remove_const<T>::type type; };
        template<typename T> struct decay_helper<T, true, false>  { typedef typename remove_extent<T>::type *type; };
        template<typename T> struct decay_helper<T, false, true>  { typedef typename add_pointer<T>::type type; };
    }
    template<typename T> struct decay
    { typedef typename det::decay_helper<typename remove_reference<T>::type>::type type; };

    // add_lvalue_reference, add_rvalue_reference
    namespace det {
        template<typename T, bool = is_referenceable<T>::value> struct add_lvalue_reference_helper { typedef T type; };
        template<typename T, bool = is_referenceable<T>::value> struct add_rvalue_reference_helper { typedef T type; };
        template<typename T> struct add_lvalue_reference_helper<T, true> { typedef T &type; };
        template<typename T> struct add_rvalue_reference_helper<T, true> { typedef T &&type; };
    }
    template<typename T> struct add_lvalue_reference : public det::add_lvalue_reference_helper<T> {};
    template<typename T> struct add_rvalue_reference : public det::add_rvalue_reference_helper<T> {};

    // add_const
    template<typename T> struct add_const { typedef T const type; };

    // is_null_pointer
    namespace det {
        template<typename>  struct is_null_pointer_helper               : public false_type {};
        template<>          struct is_null_pointer_helper<nullptr_t>    : public true_type {};
    }
    template<typename T> struct is_null_pointer :
        public det::is_null_pointer_helper<typename remove_cv<T>::type>::type {};

    // is_integral
    namespace det {
        template<typename>  struct is_integral_helper                       : public false_type {};
        template<>          struct is_integral_helper<bool>                 : public true_type {};
        template<>          struct is_integral_helper<char>                 : public true_type {};
        template<>          struct is_integral_helper<signed char>          : public true_type {};
        template<>          struct is_integral_helper<unsigned char>        : public true_type {};
        template<>          struct is_integral_helper<wchar_t>              : public true_type {};
        template<>          struct is_integral_helper<char16_t>             : public true_type {};
        template<>          struct is_integral_helper<char32_t>             : public true_type {};
        template<>          struct is_integral_helper<short>                : public true_type {};
        template<>          struct is_integral_helper<unsigned short>       : public true_type {};
        template<>          struct is_integral_helper<int>                  : public true_type {};
        template<>          struct is_integral_helper<unsigned int>         : public true_type {};
        template<>          struct is_integral_helper<long>                 : public true_type {};
        template<>          struct is_integral_helper<unsigned long>        : public true_type {};
        template<>          struct is_integral_helper<long long>            : public true_type {};
        template<>          struct is_integral_helper<unsigned long long>   : public true_type {};
    }
    template<typename T> struct is_integral :
        public det::is_integral_helper<typename remove_cv<T>::type>::type {};

    // is_floating_point
    namespace det {
        template<typename>  struct is_floating_point_helper                 : public false_type {};
        template<>          struct is_floating_point_helper<float>          : public true_type {};
        template<>          struct is_floating_point_helper<double>         : public true_type {};
        template<>          struct is_floating_point_helper<long double>    : public true_type {};
    }
    template<typename T> struct is_floating_point :
        public det::is_floating_point_helper<typename remove_cv<T>::type>::type {};

    // is_arithmetic
    template<typename T> struct is_arithmetic :
        public or_<is_integral<T>, is_floating_point<T>>::type {};

    // is_fundamental
    template<typename T> struct is_fundamental :
        public or_<is_arithmetic<T>, is_void<T>, is_null_pointer<T>>::type {};

}

#endif
