/* Copyright 2019 Axel Huebl, Benjamin Worpitz
 *
 * This file is part of Alpaka.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <alpaka/acc/Traits.hpp>

#include <alpaka/test/acc/TestAccs.hpp>

#include <catch2/catch.hpp>

#include <iostream>

//-----------------------------------------------------------------------------
struct TestTemplate
{
    template< typename TAcc >
    void operator()()
    {
        std::cout << alpaka::acc::getAccName<TAcc>() << std::endl;
    }
};

TEST_CASE( "getAccName", "[acc]")
{
    alpaka::meta::forEachType< alpaka::test::acc::TestAccs >( TestTemplate() );
}
