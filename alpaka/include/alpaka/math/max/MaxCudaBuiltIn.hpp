/* Copyright 2019 Axel Huebl, Benjamin Worpitz, Bert Wesarg
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#pragma once

#ifdef ALPAKA_ACC_GPU_CUDA_ENABLED

#include <alpaka/core/Common.hpp>
#include <alpaka/core/Unused.hpp>

#if !BOOST_LANG_CUDA
    #error If ALPAKA_ACC_GPU_CUDA_ENABLED is set, the compiler has to support CUDA!
#endif

#include <alpaka/math/max/Traits.hpp>

#include <cuda_runtime.h>
#include <type_traits>


namespace alpaka
{
    namespace math
    {
        //#############################################################################
        //! The standard library max.
        class MaxCudaBuiltIn
        {
        public:
            using MaxBase = MaxCudaBuiltIn;
        };

        namespace traits
        {
            //#############################################################################
            //! The standard library integral max trait specialization.
            template<
                typename Tx,
                typename Ty>
            struct Max<
                MaxCudaBuiltIn,
                Tx,
                Ty,
                typename std::enable_if<
                    std::is_integral<Tx>::value
                    && std::is_integral<Ty>::value>::type>
            {
                __device__ static auto max(
                    MaxCudaBuiltIn const & max,
                    Tx const & x,
                    Ty const & y)
                -> decltype(::max(x, y))
                {
                    alpaka::ignore_unused(max);
                    return ::max(x, y);
                }
            };
            //#############################################################################
            //! The standard library mixed integral floating point max trait specialization.
            template<
                typename Tx,
                typename Ty>
            struct Max<
                MaxCudaBuiltIn,
                Tx,
                Ty,
                typename std::enable_if<
                    std::is_arithmetic<Tx>::value
                    && std::is_arithmetic<Ty>::value
                    && !(std::is_integral<Tx>::value
                        && std::is_integral<Ty>::value)>::type>
            {
                __device__ static auto max(
                    MaxCudaBuiltIn const & max,
                    Tx const & x,
                    Ty const & y)
                -> decltype(::fmax(x, y))
                {
                    alpaka::ignore_unused(max);
                    return ::fmax(x, y);
                }
            };
        }
    }
}

#endif
