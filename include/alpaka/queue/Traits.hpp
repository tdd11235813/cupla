/* Copyright 2019 Benjamin Worpitz
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <alpaka/wait/Traits.hpp>

#include <alpaka/core/Common.hpp>

#include <type_traits>
#include <utility>

namespace alpaka
{
    //-----------------------------------------------------------------------------
    //! The queue specifics.
    namespace queue
    {
        //-----------------------------------------------------------------------------
        //! The queue traits.
        namespace traits
        {
            //#############################################################################
            //! The queue enqueue trait.
            template<
                typename TQueue,
                typename TTask,
                typename TSfinae = void>
            struct Enqueue;

            //#############################################################################
            //! The queue empty trait.
            template<
                typename TQueue,
                typename TSfinae = void>
            struct Empty;
        }

        //-----------------------------------------------------------------------------
        //! Queues the given task in the given queue.
        //!
        //! Special Handling for events:
        //!   If the event has previously been queued, then this call will overwrite any existing state of the event.
        //!   Any subsequent calls which examine the status of event will only examine the completion of this most recent call to enqueue.
        template<
            typename TQueue,
            typename TTask>
        ALPAKA_FN_HOST auto enqueue(
            TQueue & queue,
            TTask && task)
        -> void
        {
            traits::Enqueue<
                TQueue,
                typename std::decay<TTask>::type>
            ::enqueue(
                queue,
                std::forward<TTask>(task));
        }

        //-----------------------------------------------------------------------------
        //! Tests if the queue is empty (all ops in the given queue have been completed).
        template<
            typename TQueue>
        ALPAKA_FN_HOST auto empty(
            TQueue const & queue)
        -> bool
        {
            return
                traits::Empty<
                    TQueue>
                ::empty(
                    queue);
        }
    }
}
