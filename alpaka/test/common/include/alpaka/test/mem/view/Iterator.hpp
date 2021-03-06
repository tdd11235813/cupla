/* Copyright 2019 Benjamin Worpitz, Erik Zenker
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#pragma once

#include <alpaka/alpaka.hpp>

namespace alpaka
{
    //-----------------------------------------------------------------------------
    //! The test specifics.
    namespace test
    {
        //-----------------------------------------------------------------------------
        //! The test mem specifics.
        namespace mem
        {
            //-----------------------------------------------------------------------------
            //!
            namespace view
            {
                //-----------------------------------------------------------------------------
                //!
                namespace traits
                {
                    //#############################################################################
                    // \tparam T Type to conditionally make const.
                    // \tparam TSource Type to mimic the constness of.
                    template<
                        typename T,
                        typename TSource>
                    using MimicConst = typename std::conditional<
                        std::is_const<TSource>::value,
                        typename std::add_const<T>::type,
                        typename std::remove_const<T>::type>;

#if BOOST_COMP_GNUC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wcast-align" // "cast from 'Byte*' to 'Elem*' increases required alignment of target type"
#endif
                    //#############################################################################
                    template<
                        typename TView,
                        typename TSfinae = void>
                    class IteratorView
                    {
                        using TViewDecayed = typename std::decay<TView>::type;
                        using Dim = alpaka::dim::Dim<TViewDecayed>;
                        using Idx = alpaka::idx::Idx<TViewDecayed>;
                        using Elem = typename MimicConst<alpaka::elem::Elem<TViewDecayed>, TView>::type;

                    public:
                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST IteratorView(
                            TView & view,
                            Idx const idx) :
                                m_nativePtr(alpaka::mem::view::getPtrNative(view)),
                                m_currentIdx(idx),
                                m_extents(alpaka::extent::getExtentVec(view)),
                                m_pitchBytes(alpaka::mem::view::getPitchBytesVec(view))
                        {}

                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST IteratorView(
                            TView & view) :
                                IteratorView(view, 0)
                        {}

                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST_ACC auto operator++()
                        -> IteratorView&
                        {
                            ++m_currentIdx;
                            return *this;
                        }

                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST_ACC auto operator--()
                        -> IteratorView&
                        {
                            --m_currentIdx;
                            return *this;
                        }

                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST_ACC auto operator++(
                            int)
                        -> IteratorView
                        {
                            IteratorView iterCopy = *this;
                            m_currentIdx++;
                            return iterCopy;
                        }

                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST_ACC auto operator--(
                            int)
                        -> IteratorView
                        {
                            IteratorView iterCopy = *this;
                            m_currentIdx--;
                            return iterCopy;
                        }

                        //-----------------------------------------------------------------------------
                        template<typename TIter>
                        ALPAKA_FN_HOST_ACC auto operator==(
                            TIter &other) const
                        -> bool
                        {
                            return m_currentIdx == other.m_currentIdx;
                        }

                        //-----------------------------------------------------------------------------
                        template<typename TIter>
                        ALPAKA_FN_HOST_ACC auto operator!=(
                            TIter &other) const
                        -> bool
                        {
                            return m_currentIdx != other.m_currentIdx;
                        }

                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST_ACC auto operator*() const
                        -> Elem &
                        {
                            using Dim1 = dim::DimInt<1>;
                            using DimMin1 = dim::DimInt<Dim::value - 1u>;

                            vec::Vec<Dim1, Idx> const currentIdxDim1{m_currentIdx};
                            vec::Vec<Dim, Idx> const currentIdxDimx(idx::mapIdx<Dim::value>(currentIdxDim1, m_extents));

                            // [pz, py, px] -> [py, px]
                            auto const pitchWithoutOutermost(vec::subVecEnd<DimMin1>(m_pitchBytes));
                            // [ElemSize]
                            vec::Vec<Dim1, Idx> const elementSizeVec(static_cast<Idx>(sizeof(Elem)));
                            // [py, px] ++ [ElemSize] -> [py, px, ElemSize]
                            vec::Vec<Dim, Idx> const dstPitchBytes(vec::concat(pitchWithoutOutermost, elementSizeVec));
                            // [py, px, ElemSize] [z, y, x] -> [py*z, px*y, ElemSize*x]
                            auto const dimensionalOffsetsInByte(currentIdxDimx * dstPitchBytes);
                            // sum{[py*z, px*y, ElemSize*x]} -> offset in byte
                            auto const offsetInByte(dimensionalOffsetsInByte.foldrAll(
                                [](Idx a, Idx b)
                                {
                                    return static_cast<Idx>(a + b);
                                }));

                            using Byte = typename MimicConst<std::uint8_t, Elem>::type;
                            Byte* ptr(reinterpret_cast<Byte*>(m_nativePtr) + offsetInByte);

#if 0
                            std::cout
                                << " i1: " << currentIdxDim1
                                << " in: " << currentIdxDimx
                                << " dpb: " << dstPitchBytes
                                << " offb: " << offsetInByte
                                << " ptr: " << reinterpret_cast<void const *>(ptr)
                                << " v: " << *reinterpret_cast<Elem *>(ptr)
                                << std::endl;
#endif
                            return *reinterpret_cast<Elem *>(ptr);
                        }

                    private:
                        Elem * const m_nativePtr;
                        Idx m_currentIdx;
                        vec::Vec<Dim, Idx> const m_extents;
                        vec::Vec<Dim, Idx> const m_pitchBytes;
                    };
#if BOOST_COMP_GNUC
    #pragma GCC diagnostic pop
#endif

                    //#############################################################################
                    template<
                        typename TView,
                        typename TSfinae = void>
                    struct Begin
                    {
                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST static auto begin(
                            TView & view)
                        -> IteratorView<TView>
                        {
                            return IteratorView<TView>(view);
                        }
                    };

                    //#############################################################################
                    template<
                        typename TView,
                        typename TSfinae = void>
                    struct End
                    {
                        //-----------------------------------------------------------------------------
                        ALPAKA_FN_HOST static auto end(
                            TView & view)
                        -> IteratorView<TView>
                        {
                            auto extents = alpaka::extent::getExtentVec(view);
                            return IteratorView<TView>(view, extents.prod());
                        }
                    };
                }

                //#############################################################################
                template<
                    typename TView>
                using Iterator = traits::IteratorView<TView>;

                //-----------------------------------------------------------------------------
                template<
                    typename TView>
                ALPAKA_FN_HOST auto begin(
                    TView & view)
                -> Iterator<TView>
                {
                    return traits::Begin<TView>::begin(view);
                }

                //-----------------------------------------------------------------------------
                template<
                    typename TView>
                ALPAKA_FN_HOST auto end(
                    TView & view)
                -> Iterator<TView>
                {
                    return traits::End<TView>::end(view);
                }
            }
        }
    }
}
