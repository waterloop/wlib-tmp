#ifndef __WLIB_TMP_CONVERTIBLE_H__
#define __WLIB_TMP_CONVERTIBLE_H__

#include <wlib/tmp/TypeTraits.h>
#include <wlib/tmp/Declval.h>

namespace wlp {

    // is_convertible
    namespace det {
        template<typename From, typename To, bool = or_<is_void<From>, is_function<To>, is_array<To>>::value>
        struct is_convertible_helper { typedef typename is_void<To>::type type; };

        template<typename From, typename To>
        struct is_convertible_helper<From, To, false> {
        private:
            template<typename U> static void aux(U);
            template<typename F, typename T, typename = decltype(aux<T>(declval<F>()))> static true_type    test(int);
            template<typename, typename>                                                static false_type   test(...);
        public:
            typedef decltype(test<From, To>(0)) type;
        };
    }
    template<typename From, typename To>
    struct is_convertible : public det::is_convertible_helper<From, To>::type {};

}

#endif
