#ifndef __WLIB_TMP_DECLVAL_H__
#define __WLIB_TMP_DECLVAL_H__

#include <wlib/tmp/TypeTraits.h>

namespace wlp {

    // declval
    namespace det {
        template<typename T> struct declval_helper {
            static constexpr bool stop = false;
            static constexpr typename add_rvalue_reference<T>::type delegate();
        };
    }
    template<typename T>
    inline typename add_rvalue_reference<T>::type declval() noexcept {
        static_assert(det::declval_helper<T>::stop, "declval");
        return det::declval_helper<T>::delegate();
    }

}

#endif

