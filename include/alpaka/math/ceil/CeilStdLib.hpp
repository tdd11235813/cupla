/* Copyright 2019 Axel Huebl, Benjamin Worpitz
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <alpaka/math/ceil/Traits.hpp>

#include <alpaka/core/Unused.hpp>

#include <type_traits>
#include <cmath>

namespace alpaka
{
    namespace math
    {
        //#############################################################################
        //! The standard library ceil.
        class CeilStdLib
        {
        public:
            using CeilBase = CeilStdLib;
        };

        namespace traits
        {
            //#############################################################################
            //! The standard library ceil trait specialization.
            template<
                typename TArg>
            struct Ceil<
                CeilStdLib,
                TArg,
                typename std::enable_if<
                    std::is_arithmetic<TArg>::value>::type>
            {
                ALPAKA_FN_HOST static auto ceil(
                    CeilStdLib const & ceil_ctx,
                    TArg const & arg)
                -> decltype(std::ceil(arg))
                {
                    alpaka::ignore_unused(ceil_ctx);
                    return std::ceil(arg);
                }
            };
        }
    }
}
