#ifndef __WLIB_TMP_TYPETRAITS_H__
#define __WLIB_TMP_TYPETRAITS_H__

#include <wlib/tmp/IntegralConstant.h>
#include <wlib/tmp/Condition.h>

namespace wlp {

    // remove const and volatile
    template<typename T> struct remove_const            { typedef T type; };
    template<typename T> struct remove_const<T const>   { typedef T type; };
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

    // is_reference
    template<typename>      struct is_lvalue_reference      : public false_type {};
    template<typename T>    struct is_lvalue_reference<T &> : public true_type {};
    template<typename>      struct is_rvalue_reference          : public false_type {};
    template<typename T>    struct is_rvalue_reference<T &&>    : public true_type {};

    template<typename T> struct is_reference :
        public or_<is_lvalue_reference<T>, is_rvalue_reference<T>>::type {};

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

    // is_object

    // is_referenceable

    // add_pointer

}

#endif
