/* Copyright 2019 Benjamin Worpitz, René Widera
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <alpaka/core/Common.hpp>

#if BOOST_LANG_CUDA && BOOST_COMP_CLANG_CUDA
#   include <type_traits>
#else
#   include <utility>
#endif

namespace alpaka
{
    namespace core
    {
        //-----------------------------------------------------------------------------
        //! convert any type to a reverence type
        //
        // This function is equivalent to std::declval() but can be used
        // within an alpaka accelerator kernel too.
        // This function can be used only within std::decltype().
        //-----------------------------------------------------------------------------
#if BOOST_LANG_CUDA && BOOST_COMP_CLANG_CUDA
        template< class T >
        ALPAKA_FN_HOST_ACC
        typename std::add_rvalue_reference<T>::type
        declval();
#else
        using std::declval;
#endif
    }
}
