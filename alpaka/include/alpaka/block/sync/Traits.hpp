/* Copyright 2019 Benjamin Worpitz
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#pragma once

#include <alpaka/meta/IsStrictBase.hpp>

#include <alpaka/core/Common.hpp>

#include <type_traits>

namespace alpaka
{
    //-----------------------------------------------------------------------------
    //! The grid block specifics
    namespace block
    {
        //-----------------------------------------------------------------------------
        //! The block synchronization specifics.
        namespace sync
        {
            //-----------------------------------------------------------------------------
            //! The block synchronization traits.
            namespace traits
            {
                //#############################################################################
                //! The block synchronization operation trait.
                template<
                    typename TBlockSync,
                    typename TSfinae = void>
                struct SyncBlockThreads;

                //#############################################################################
                //! The block synchronization and predicate operation trait.
                template<
                    typename TOp,
                    typename TBlockSync,
                    typename TSfinae = void>
                struct SyncBlockThreadsPredicate;
            }

            //-----------------------------------------------------------------------------
            //! Synchronizes all threads within the current block (independently for all blocks).
            //!
            //! \tparam TBlockSync The block synchronization implementation type.
            //! \param blockSync The block synchronization implementation.
            ALPAKA_NO_HOST_ACC_WARNING
            template<
                typename TBlockSync>
            ALPAKA_FN_ACC auto syncBlockThreads(
                TBlockSync const & blockSync)
            -> void
            {
                traits::SyncBlockThreads<
                    TBlockSync>
                ::syncBlockThreads(
                    blockSync);
            }

            namespace traits
            {
                //#############################################################################
                //! The AllocVar trait specialization for classes with BlockSyncBase member type.
                template<
                    typename TBlockSync>
                struct SyncBlockThreads<
                    TBlockSync,
                    typename std::enable_if<
                        meta::IsStrictBase<
                            typename TBlockSync::BlockSyncBase,
                            TBlockSync
                        >::value
                    >::type>
                {
                    //-----------------------------------------------------------------------------
                    ALPAKA_NO_HOST_ACC_WARNING
                    ALPAKA_FN_ACC static auto syncBlockThreads(
                        TBlockSync const & blockSync)
                    -> void
                    {
                        // Delegate the call to the base class.
                        block::sync::syncBlockThreads(
                            static_cast<typename TBlockSync::BlockSyncBase const &>(blockSync));
                    }
                };
            }

            //-----------------------------------------------------------------------------
            //! Defines operation functors.
            namespace op
            {
                //#############################################################################
                //! The addition function object.
                struct Count
                {
                    enum { InitialValue = 0u};

                    ALPAKA_NO_HOST_ACC_WARNING
                    template<
                        typename T>
                    ALPAKA_FN_HOST_ACC auto operator()(
                        T const & currentResult,
                        T const & value) const
                    -> T
                    {
                        return currentResult + static_cast<T>(value != static_cast<T>(0));
                    }
                };
                //#############################################################################
                //! The logical and function object.
                struct LogicalAnd
                {
                    enum { InitialValue = 1u};

                    ALPAKA_NO_HOST_ACC_WARNING
                    template<
                        typename T>
                    ALPAKA_FN_HOST_ACC auto operator()(
                        T const & currentResult,
                        T const & value) const
                    -> T
                    {
                        return static_cast<T>(currentResult && (value != static_cast<T>(0)));
                    }
                };
                //#############################################################################
                //! The logical or function object.
                struct LogicalOr
                {
                    enum { InitialValue = 0u};

                    ALPAKA_NO_HOST_ACC_WARNING
                    template<
                        typename T>
                    ALPAKA_FN_HOST_ACC auto operator()(
                        T const & currentResult,
                        T const & value) const
                    -> T
                    {
                        return static_cast<T>(currentResult || (value != static_cast<T>(0)));
                    }
                };
            }

            //-----------------------------------------------------------------------------
            //! Synchronizes all threads within the current block (independently for all blocks),
            //! evaluates the predicate for all threads and returns the combination of all the results
            //! computed via TOp.
            //!
            //! \tparam TOp The operation used to combine the predicate values of all threads.
            //! \tparam TBlockSync The block synchronization implementation type.
            //! \param blockSync The block synchronization implementation.
            //! \param predicate The predicate value of the current thread.
            ALPAKA_NO_HOST_ACC_WARNING
            template<
                typename TOp,
                typename TBlockSync>
            ALPAKA_FN_ACC auto syncBlockThreadsPredicate(
                TBlockSync const & blockSync,
                int predicate)
            -> int
            {
                return
                    traits::SyncBlockThreadsPredicate<
                        TOp,
                        TBlockSync>
                    ::syncBlockThreadsPredicate(
                        blockSync,
                        predicate);
            }

            namespace traits
            {
                //#############################################################################
                //! The AllocVar trait specialization for classes with BlockSyncBase member type.
                template<
                    typename TOp,
                    typename TBlockSync>
                struct SyncBlockThreadsPredicate<
                    TOp,
                    TBlockSync,
                    typename std::enable_if<
                        meta::IsStrictBase<
                            typename TBlockSync::BlockSyncBase,
                            TBlockSync
                        >::value
                    >::type>
                {
                    //-----------------------------------------------------------------------------
                    ALPAKA_NO_HOST_ACC_WARNING
                    ALPAKA_FN_ACC static auto syncBlockThreadsPredicate(
                        TBlockSync const & blockSync,
                        int predicate)
                    -> int
                    {
                        // Delegate the call to the base class.
                        return
                            block::sync::syncBlockThreadsPredicate<
                                TOp>(
                                    static_cast<typename TBlockSync::BlockSyncBase const &>(blockSync),
                                    predicate);
                    }
                };
            }
        }
    }
}
