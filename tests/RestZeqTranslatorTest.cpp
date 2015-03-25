
/* Copyright (c) 2014-2015, Human Brain Project
 *                     Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// CAUTION FOR REVIEWERS: This test is !COMPLETELY! fake for now !!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define BOOST_TEST_MODULE restZeqTranslator

#include <boost/test/unit_test.hpp>
#include <restconnector/detail/RestZeqTranslator.h>

#include <zeq/hbp/vocabulary.h>

BOOST_AUTO_TEST_CASE( test_cameraEvent )
{
    std::vector< float > matrixData = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    const std::string& bodyJSON = { "{\"matrix\": \[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]}" } ;

    restconnector::detail::RestZeqTranslator restzeqTranslator;
    const std::string commandString = "setCamera";

    const bool result = restzeqTranslator.isCommandSupported( commandString );
    BOOST_CHECK_EQUAL( result, true );

    const zeq::Event& zeqEvent = restzeqTranslator.translate( bodyJSON );

    const std::vector< float >& deserialized = zeq::hbp::deserializeCamera( zeqEvent );

    BOOST_CHECK_EQUAL_COLLECTIONS( matrixData.begin(), matrixData.end(),
                                   deserialized.begin(), deserialized.end( ));
}

BOOST_AUTO_TEST_CASE( test_requestEvent )
{
    restconnector::detail::RestZeqTranslator restzeqTranslator;
    const std::string commandString = "request";

    const bool result = restzeqTranslator.isCommandSupported( commandString );
    BOOST_CHECK_EQUAL( result, true );

    const zeq::Event& zeqEvent = restzeqTranslator.translate( "" );

    BOOST_CHECK_EQUAL( zeq::hbp::EVENT_IMAGEJPEG,
                       zeq::vocabulary::deserializeRequest( zeqEvent ) );
}

BOOST_AUTO_TEST_CASE( test_unknownEvent )
{
    restconnector::detail::RestZeqTranslator restzeqTranslator;
    const std::string commandString = "IDDQD";

    const bool result = restzeqTranslator.isCommandSupported( commandString );
    BOOST_CHECK_EQUAL( result, false );
}
