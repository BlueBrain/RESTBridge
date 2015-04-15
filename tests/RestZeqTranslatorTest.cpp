
/* Copyright (c) 2014-2015, Human Brain Project
 *                     Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// CAUTION FOR REVIEWERS: This test is !COMPLETELY! fake for now !!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define BOOST_TEST_MODULE restZeqTranslator

#include <boost/test/unit_test.hpp>
#include <restconnector/detail/RestZeqTranslator.h>

#include <zeq/zeq.h>
#include <zeq/hbp/hbp.h>

BOOST_AUTO_TEST_CASE( test_EventSUBSCRIBED )
{
    restconnector::detail::RestZeqTranslator restZeqTranslator;

    zeq::EventDescriptor eventDescpritor( zeq::hbp::CAMERA, zeq::hbp::EVENT_CAMERA,
                                          zeq::hbp::SCHEMA_CAMERA, zeq::BIDIRECTIONAL );

    restZeqTranslator.addSubscribedEvent( eventDescpritor );

    std::vector< float > matrixData = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    const std::string& bodyJSON = { "{\"matrix\": \[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]}" } ;
    const std::string commandString = "CAMERA";

    const zeq::Event& zeqEvent = restZeqTranslator.translate( commandString, bodyJSON );

    const std::vector< float >& deserialized = zeq::hbp::deserializeCamera( zeqEvent );

    BOOST_CHECK_EQUAL_COLLECTIONS( matrixData.begin(), matrixData.end(),
                                   deserialized.begin(), deserialized.end( ) );
}

BOOST_AUTO_TEST_CASE( test_EventPUBLISHED )
{
    restconnector::detail::RestZeqTranslator restZeqTranslator;

    zeq::EventDescriptor eventDescpritor( zeq::hbp::IMAGEJPEG, zeq::hbp::EVENT_IMAGEJPEG,
                                          zeq::hbp::SCHEMA_IMAGEJPEG, zeq::PUBLISHER );

    restZeqTranslator.addPublishedEvent( eventDescpritor );

    const std::string commandString = "IMAGEJPEG";

    const zeq::Event& zeqEvent = restZeqTranslator.translate( commandString );

    BOOST_CHECK_EQUAL( zeq::hbp::EVENT_IMAGEJPEG,
                       zeq::vocabulary::deserializeRequest( zeqEvent ) );
}

BOOST_AUTO_TEST_CASE( test_unknownEvent )
{
    restconnector::detail::RestZeqTranslator restZeqTranslator;

    zeq::EventDescriptor eventDescpritor( zeq::hbp::CAMERA, zeq::hbp::EVENT_CAMERA,
                                          zeq::hbp::SCHEMA_CAMERA, zeq::BIDIRECTIONAL );

    restZeqTranslator.addSubscribedEvent( eventDescpritor );

    const std::string commandString = "IDDQD";

    bool test;

    try
    {
        restZeqTranslator.translate( commandString );
        test = true;
    }
    catch( const restconnector::detail::RestZeqTranslator::RestZeqTranslatorException& e )
    {
        test = false;
    }
    BOOST_CHECK_EQUAL( test, false );
}
