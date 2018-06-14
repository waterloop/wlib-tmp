#ifndef __WLIB_TMP_CONDITION_H__
#define __WLIB_TMP_CONDITION_H__

#include <wlib/tmp/IntegralConstant.h>

namespace wlp {

    // conditional
    template<bool, typename, typename> struct conditional;
    template<bool condition, typename if_true, typename if_false>
    struct conditional { typedef if_true type; };
    template<typename if_true, typename if_false>
    struct conditional<false, if_true, if_false> { typedef if_false type; };

    // or_
    template<typename...>               struct or_;
    template<>                          struct or_<>        : public false_type {};
    template<typename Q1>               struct or_<Q1>      : public Q1 {};
    template<typename Q1, typename Q2>  struct or_<Q1, Q2>  : public conditional<Q1::value, Q1, Q2>::type {};

    template<typename Q1, typename Q2, typename Q3, typename... Qn>
    struct or_<Q1, Q2, Q3, Qn...> : public conditional<Q1::value, Q1, or_<Q2, Q3, Qn...>>::type {};

    // and_
    template<typename...>               struct and_;
    template<>                          struct and_<>       : public true_type {};
    template<typename Q1>               struct and_<Q1>     : public Q1 {};
    template<typename Q1, typename Q2>  struct and_<Q1, Q2> : public conditional<Q1::value, Q2, Q1>::type {};

    template<typename Q1, typename Q2, typename Q3, typename... Qn>
    struct and_<Q1, Q2, Q3, Qn...>: public conditional<Q1::value, and_<Q2, Q3, Qn...>, Q1>::type {};

    // not_
    template<typename T> struct not_ : public integral_constant<bool, !T::value> {};

}

#endif
