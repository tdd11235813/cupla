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

#include <boost/config.hpp>

#include <type_traits>

namespace alpaka
{
    namespace math
    {
        namespace traits
        {
            //#############################################################################
            //! The cos trait.
            template<
                typename T,
                typename TArg,
                typename TSfinae = void>
            struct Cos;
        }

        //-----------------------------------------------------------------------------
        //! Computes the cosine (measured in radians).
        //!
        //! \tparam T The type of the object specializing Cos.
        //! \tparam TArg The arg type.
        //! \param cos_ctx The object specializing Cos.
        //! \param arg The arg.
        ALPAKA_NO_HOST_ACC_WARNING
        template<
            typename T,
            typename TArg>
        ALPAKA_FN_HOST_ACC auto cos(
            T const & cos_ctx,
            TArg const & arg)
#ifdef BOOST_NO_CXX14_RETURN_TYPE_DEDUCTION
        -> decltype(
            traits::Cos<
                T,
                TArg>
            ::cos(
                cos_ctx,
                arg))
#endif
        {
            return
                traits::Cos<
                    T,
                    TArg>
                ::cos(
                    cos_ctx,
                    arg);
        }

        namespace traits
        {
            //#############################################################################
            //! The Cos specialization for classes with CosBase member type.
            template<
                typename T,
                typename TArg>
            struct Cos<
                T,
                TArg,
                typename std::enable_if<
                    meta::IsStrictBase<
                        typename T::CosBase,
                        T
                    >::value
                >::type>
            {
                //-----------------------------------------------------------------------------
                ALPAKA_NO_HOST_ACC_WARNING
                ALPAKA_FN_HOST_ACC static auto cos(
                    T const & cos_ctx,
                    TArg const & arg)
#ifdef BOOST_NO_CXX14_RETURN_TYPE_DEDUCTION
                -> decltype(
                    math::cos(
                        static_cast<typename T::CosBase const &>(cos_ctx),
                        arg))
#endif
                {
                    // Delegate the call to the base class.
                    return
                        math::cos(
                            static_cast<typename T::CosBase const &>(cos_ctx),
                            arg);
                }
            };
        }
    }
}
