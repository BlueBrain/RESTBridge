
/* Copyright (c) 2015, Human Brain Project
 *                     Daniel Nachbaur <daniel.nachbaur@epfl.ch>
 */

#define BOOST_TEST_MODULE restZeqTranslator

#include <boost/test/unit_test.hpp>
#include <restconnector/RestConnector.h>

#include <lunchbox/rng.h>


BOOST_AUTO_TEST_CASE( test_construction )
{
    BOOST_CHECK_NO_THROW( restconnector::RestConnector( "localhost", 12345 ));
}

BOOST_AUTO_TEST_CASE( test_invalid_run )
{
    const unsigned short port = (lunchbox::RNG().get<uint16_t>() % 60000) + 1024;
    restconnector::RestConnector connector( "localhost", port );
    connector.run( "bla" );
    BOOST_CHECK_THROW( connector.run( "blub" ), std::runtime_error );
}

BOOST_AUTO_TEST_CASE( test_invalid_stop )
{
    const unsigned short port = (lunchbox::RNG().get<uint16_t>() % 60000) + 1024;
    restconnector::RestConnector connector( "localhost", port );
    BOOST_CHECK_THROW( connector.stop(), std::runtime_error );
}

BOOST_AUTO_TEST_CASE( test_run_stop )
{
    const unsigned short port = (lunchbox::RNG().get<uint16_t>() % 60000) + 1024;
    restconnector::RestConnector connector( "localhost", port );
    connector.run( "bla" );
    connector.stop();
}
