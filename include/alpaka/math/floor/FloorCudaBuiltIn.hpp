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

#include <alpaka/math/floor/Traits.hpp>

#include <cuda_runtime.h>
#include <type_traits>


namespace alpaka
{
    namespace math
    {
        //#############################################################################
        //! The standard library floor.
        class FloorCudaBuiltIn
        {
        public:
            using FloorBase = FloorCudaBuiltIn;
        };

        namespace traits
        {
            //#############################################################################
            //! The standard library floor trait specialization.
            template<
                typename TArg>
            struct Floor<
                FloorCudaBuiltIn,
                TArg,
                typename std::enable_if<
                    std::is_floating_point<TArg>::value>::type>
            {
                __device__ static auto floor(
                    FloorCudaBuiltIn const & floor,
                    TArg const & arg)
                -> decltype(::floor(arg))
                {
                    alpaka::ignore_unused(floor);
                    return ::floor(arg);
                }
            };
        }
    }
}

#endif
