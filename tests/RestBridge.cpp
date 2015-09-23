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
#include <random>

servus::Strings args = {{ "--rest" }, { "--chocolate" }, { "is-good" },
                        { "--something-different..." }, { "is-not-good" },
                        { "--(some-well-known-OS)8" },
                        { "is-not-really-chocolate" }};

BOOST_AUTO_TEST_CASE( construction_argc_argv )
{
    std::string uri = "zeq01://localhost:6666";
    char* argv[] = { &args[1][0], &args[2][0], &args[3][0], &args[4][0],
                     &args[0][0], &uri[0], &args[4][0], &args[5][0] };
    const int argc = 8;
    BOOST_CHECK_NO_THROW( restbridge::RestBridge::parse( argc, argv ));
}

BOOST_AUTO_TEST_CASE( construction_argc_argv_invalid_schema )
{
    std::string uri = "foo://localhost:murks";
    char* argv[] = { &args[1][0], &args[2][0], &args[3][0], &args[4][0],
                     &args[0][0], &uri[0], &args[4][0], &args[5][0] };
    const int argc = 8;
    BOOST_CHECK_THROW( restbridge::RestBridge::parse( argc, argv ),
                       std::exception );
}

BOOST_AUTO_TEST_CASE( argc_argv_run )
{
    std::string uri = "zeq02://localhost:6666";
    char* argv[] = { &args[1][0], &args[2][0], &args[3][0], &args[4][0],
                     &args[0][0], &uri[0], &args[4][0], &args[5][0] };
    const int argc = 8;
    std::unique_ptr< restbridge::RestBridge > bridge =
        restbridge::RestBridge::parse( argc, argv );
    BOOST_CHECK( bridge );
    BOOST_CHECK( bridge->isRunning( ));
}
