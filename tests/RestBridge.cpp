/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 *
 * This file is part of RESTBridge <https://github.com/BlueBrain/RESTBridge>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define BOOST_TEST_MODULE restZeqTranslator

#include <boost/test/unit_test.hpp>
#include <restbridge/RestBridge.h>

#include <lunchbox/rng.h>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<unsigned long long> dis(0,60000);

BOOST_AUTO_TEST_CASE( test_construction )
{
    BOOST_CHECK_NO_THROW( restbridge::RestBridge( "localhost", 12345 ));
}

BOOST_AUTO_TEST_CASE( test_invalid_run )
{
    const unsigned short port =  dis(gen) + 1024;
    restbridge::RestBridge bridge( "localhost", port );
    bridge.run( "bla" );
    BOOST_CHECK_THROW( bridge.run( "blub" ), std::runtime_error );
}

BOOST_AUTO_TEST_CASE( test_invalid_stop )
{
    const unsigned short port =  dis(gen) + 1024;
    restbridge::RestBridge bridge( "localhost", port );
    BOOST_CHECK_THROW( bridge.stop(), std::runtime_error );
}

BOOST_AUTO_TEST_CASE( test_run_stop )
{
    const unsigned short port =  dis(gen) + 1024;
    restbridge::RestBridge bridge( "localhost", port );
    bridge.run( "bla" );
    bridge.stop();
}
