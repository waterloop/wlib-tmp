#ifndef __WLIB_TMP_INTEGRALCONSTANT_H__
#define __WLIB_TMP_INTEGRALCONSTANT_H__

namespace wlp {

    // integral_constant
    template<typename T, T t>
    struct integral_constant {
        static constexpr T value = t;
        typedef T value_type;
        typedef integral_constant<T, t> type;
    };
    typedef integral_constant<bool, true>   true_type;
    typedef integral_constant<bool, false>  false_type;

}

#endif
