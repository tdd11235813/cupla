/* Copyright 2019 Axel Huebl, Benjamin Worpitz, Matthias Werner, René Widera
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#pragma once

#ifdef ALPAKA_ACC_GPU_HIP_ENABLED

#include <alpaka/core/Common.hpp>
#include <alpaka/core/Unused.hpp>

#if !BOOST_LANG_HIP
    #error If ALPAKA_ACC_GPU_HIP_ENABLED is set, the compiler has to support HIP!
#endif

#include <alpaka/idx/Traits.hpp>

#include <alpaka/vec/Vec.hpp>
#include <alpaka/core/Hip.hpp>
#include <alpaka/core/Positioning.hpp>

namespace alpaka
{
    namespace idx
    {
        namespace bt
        {
            //#############################################################################
            //! The HIP accelerator ND index provider.
            template<
                typename TDim,
                typename TIdx>
            class IdxBtHipBuiltIn
            {
            public:
                using IdxBtBase = IdxBtHipBuiltIn;

                //-----------------------------------------------------------------------------
                ALPAKA_FN_HOST_ACC IdxBtHipBuiltIn() = default;
                //-----------------------------------------------------------------------------
                IdxBtHipBuiltIn(IdxBtHipBuiltIn const &) = delete;
                //-----------------------------------------------------------------------------
                IdxBtHipBuiltIn(IdxBtHipBuiltIn &&) = delete;
                //-----------------------------------------------------------------------------
                auto operator=(IdxBtHipBuiltIn const & ) -> IdxBtHipBuiltIn & = delete;
                //-----------------------------------------------------------------------------
                auto operator=(IdxBtHipBuiltIn &&) -> IdxBtHipBuiltIn & = delete;
                //-----------------------------------------------------------------------------
                /*virtual*/ ALPAKA_FN_HOST_ACC ~IdxBtHipBuiltIn() = default;
            };
        }
    }

    namespace dim
    {
        namespace traits
        {
            //#############################################################################
            //! The GPU HIP accelerator index dimension get trait specialization.
            template<
                typename TDim,
                typename TIdx>
            struct DimType<
                idx::bt::IdxBtHipBuiltIn<TDim, TIdx>>
            {
                using type = TDim;
            };
        }
    }
    namespace idx
    {
        namespace traits
        {
            //#############################################################################
            //! The GPU HIP accelerator block thread index get trait specialization.
            template<
                typename TDim,
                typename TIdx>
            struct GetIdx<
                idx::bt::IdxBtHipBuiltIn<TDim, TIdx>,
                origin::Block,
                unit::Threads>
            {
                //-----------------------------------------------------------------------------
                //! \return The index of the current thread in the block.
                template<
                    typename TWorkDiv>
                ALPAKA_FN_HOST_ACC static auto getIdx(
                    idx::bt::IdxBtHipBuiltIn<TDim, TIdx> const & idx,
                    TWorkDiv const &)
                -> vec::Vec<TDim, TIdx>
                {
                    alpaka::ignore_unused(idx);
                    return offset::getOffsetVecEnd<TDim>(
                        vec::Vec<std::integral_constant<typename TDim::value_type, 3>, TIdx>(
                            static_cast<TIdx>(hipThreadIdx_z),
                            static_cast<TIdx>(hipThreadIdx_y),
                            static_cast<TIdx>(hipThreadIdx_x)));
                }
            };
        }
    }
    namespace idx
    {
        namespace traits
        {
            //#############################################################################
            //! The GPU HIP accelerator block thread index idx type trait specialization.
            template<
                typename TDim,
                typename TIdx>
            struct IdxType<
                idx::bt::IdxBtHipBuiltIn<TDim, TIdx>>
            {
                using type = TIdx;
            };
        }
    }
}

#endif
