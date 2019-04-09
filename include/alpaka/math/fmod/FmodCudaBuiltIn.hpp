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

#include <alpaka/math/fmod/Traits.hpp>

#include <cuda_runtime.h>
#include <type_traits>


namespace alpaka
{
    namespace math
    {
        //#############################################################################
        //! The standard library fmod.
        class FmodCudaBuiltIn
        {
        public:
            using FmodBase = FmodCudaBuiltIn;
        };

        namespace traits
        {
            //#############################################################################
            //! The standard library fmod trait specialization.
            template<
                typename Tx,
                typename Ty>
            struct Fmod<
                FmodCudaBuiltIn,
                Tx,
                Ty,
                typename std::enable_if<
                    std::is_floating_point<Tx>::value
                    && std::is_floating_point<Ty>::value>::type>
            {
                __device__ static auto fmod(
                    FmodCudaBuiltIn const & fmod,
                    Tx const & x,
                    Ty const & y)
                -> decltype(::fmod(x, y))
                {
                    alpaka::ignore_unused(fmod);
                    return ::fmod(x, y);
                }
            };
        }
    }
}

#endif
