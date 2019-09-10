/* Copyright 2019 Axel Huebl, Benjamin Worpitz, Bert Wesarg, Valentin Gehrke
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#ifdef ALPAKA_ACC_GPU_CUDA_ENABLED

#include <alpaka/core/BoostPredef.hpp>

#if !BOOST_LANG_CUDA
    #error If ALPAKA_ACC_GPU_CUDA_ENABLED is set, the compiler has to support CUDA!
#endif

#include <alpaka/math/sqrt/Traits.hpp>

#include <alpaka/core/Unused.hpp>

#include <cuda_runtime.h>

#include <type_traits>

namespace alpaka
{
    namespace math
    {
        //#############################################################################
        //! The standard library sqrt.
        class SqrtCudaBuiltIn
        {
        public:
            using SqrtBase = SqrtCudaBuiltIn;
        };

        namespace traits
        {
            //#############################################################################
            //! The standard library sqrt trait specialization.
            template<
                typename TArg>
            struct Sqrt<
                SqrtCudaBuiltIn,
                TArg,
                typename std::enable_if<
                    std::is_floating_point<TArg>::value>::type>
            {
                __device__ static auto sqrt(
                    SqrtCudaBuiltIn const & sqrt_ctx,
                    TArg const & arg)
                -> decltype(::sqrt(arg))
                {
                    alpaka::ignore_unused(sqrt_ctx);
                    return ::sqrt(arg);
                }
            };
        }
    }
}

#endif
