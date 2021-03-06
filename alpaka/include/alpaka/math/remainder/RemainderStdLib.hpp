/* Copyright 2019 Axel Huebl, Benjamin Worpitz
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#pragma once

#include <alpaka/math/remainder/Traits.hpp>

#include <alpaka/core/Unused.hpp>

#include <type_traits>
#include <cmath>

namespace alpaka
{
    namespace math
    {
        //#############################################################################
        //! The standard library remainder.
        class RemainderStdLib
        {
        public:
            using RemainderBase = RemainderStdLib;
        };

        namespace traits
        {
            //#############################################################################
            //! The standard library remainder trait specialization.
            template<
                typename Tx,
                typename Ty>
            struct Remainder<
                RemainderStdLib,
                Tx,
                Ty,
                typename std::enable_if<
                    std::is_integral<Tx>::value
                    && std::is_integral<Ty>::value>::type>
            {
                ALPAKA_FN_HOST static auto remainder(
                    RemainderStdLib const & remainder_ctx,
                    Tx const & x,
                    Ty const & y)
                -> decltype(std::remainder(x, y))
                {
                    alpaka::ignore_unused(remainder_ctx);
                    return std::remainder(x, y);
                }
            };
        }
    }
}
