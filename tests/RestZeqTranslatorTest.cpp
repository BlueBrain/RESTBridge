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
#include <restbridge/detail/RestZeqTranslator.h>

#include <zeq/zeq.h>
#include <zeq/hbp/hbp.h>

BOOST_AUTO_TEST_CASE( test_EventSUBSCRIBED )
{
    restbridge::detail::RestZeqTranslator restZeqTranslator;

    zeq::EventDescriptor eventDescpritor( zeq::hbp::CAMERA, zeq::hbp::EVENT_CAMERA,
                                          zeq::hbp::SCHEMA_CAMERA, zeq::BIDIRECTIONAL );

    restZeqTranslator.addSubscribedEvent( eventDescpritor );

    std::vector< float > matrixData = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    const std::string& bodyJSON = { "{\"matrix\": [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]}" } ;
    const std::string commandString = "CAMERA";

    const zeq::Event& zeqEvent = restZeqTranslator.translate( commandString, bodyJSON );

    const std::vector< float >& deserialized = zeq::hbp::deserializeCamera( zeqEvent );

    BOOST_CHECK_EQUAL_COLLECTIONS( matrixData.begin(), matrixData.end(),
                                   deserialized.begin(), deserialized.end( ) );
}

BOOST_AUTO_TEST_CASE( test_EventPUBLISHED )
{
    restbridge::detail::RestZeqTranslator restZeqTranslator;

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
    restbridge::detail::RestZeqTranslator restZeqTranslator;

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
    catch( const restbridge::detail::RestZeqTranslator::Exception& )
    {
        test = false;
    }
    BOOST_CHECK_EQUAL( test, false );
}
