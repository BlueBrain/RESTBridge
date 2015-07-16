
/* Copyright (c) 2015, Human Brain Project
 *                     Daniel Nachbaur <daniel.nachbaur@epfl.ch>
 */

#define BOOST_TEST_MODULE restZeqTranslator

#include <boost/test/unit_test.hpp>
#include <restbridge/RestBridge.h>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<unsigned long long> dis(0,60000);

BOOST_AUTO_TEST_CASE( test_construction )
{
    BOOST_CHECK_NO_THROW( restbridge::RestBridge( "localhost", 12345 ));
}

BOOST_AUTO_TEST_CASE( test_construction_argc_argv )
{
    const char* argv[] = { "--chocolate", "is-good",
                           "--something-different...", "is-not-good",
                           "--restbridge-zeq", "zeq01://localhost:6666",
                           "--(some-well-known-OS)8", "is-not-really-chocolate" };
    const int argc = 8;
    BOOST_CHECK_NO_THROW( restbridge::RestBridge( argc, argv ));
}

BOOST_AUTO_TEST_CASE( test_construction_argc_argv_invalid_schema )
{
    const char* argv[] = { "--chocolate", "is-good",
                           "--restbridge-zeq", "://localhost:6666",
                           "--something-different...", "is-not-good",
                           "--(some-well-known-OS)8", "is-not-really-chocolate" };
    const int argc = 8;
    BOOST_CHECK_THROW( restbridge::RestBridge( argc, argv ), std::runtime_error );
}

BOOST_AUTO_TEST_CASE( test_construction_argc_argv_invalid_port )
{
    const char* argv[] = { "--(some-well-known-OS)8", "is ??",
                            "--restbridge-zeq", "zeq01://localhost:" };
    const int argc = 4;
    BOOST_CHECK_THROW( restbridge::RestBridge( argc, argv ), std::runtime_error );
}

BOOST_AUTO_TEST_CASE( test_construction_argc_argv_invalid_hostname )
{
    const char* argv[] = { "--compleeeete", "#$%&",
                            "--restbridge-zeq", "zeq01://:42" };
    const int argc = 4;
    BOOST_CHECK_THROW( restbridge::RestBridge( argc, argv ), std::runtime_error );
}


BOOST_AUTO_TEST_CASE( test_argc_argv_run )
{
    const char* argv[] = { "--chocolate", "is-good",
                           "--something-different...", "is-not-good",
                           "--(some-well-known-OS)8", "is-not-really-chocolate",
                           "--restbridge-zeq", "zeq02://localhost:6666" };
    const int argc = 8;
    restbridge::RestBridge bridge( argc, argv );
    BOOST_CHECK_NO_THROW( bridge.run());
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
