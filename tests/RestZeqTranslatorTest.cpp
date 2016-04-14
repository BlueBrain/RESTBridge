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

#include <zeroeq/zeroeq.h>
#include <zeroeq/hbp/hbp.h>

BOOST_AUTO_TEST_CASE( test_EventSUBSCRIBED )
{
    restbridge::detail::RestZeqTranslator restZeqTranslator;

    zeroeq::EventDescriptor eventDescpritor( zeroeq::hbp::CAMERA, zeroeq::hbp::EVENT_CAMERA,
                                          zeroeq::hbp::SCHEMA_CAMERA, zeroeq::BIDIRECTIONAL );

    restZeqTranslator.addSubscribedEvent( eventDescpritor );

    std::vector< float > matrixData = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    const std::string& bodyJSON = { "{\"matrix\": [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]}" } ;
    const std::string commandString = "CAMERA";

    const zeroeq::Event& zeroeqEvent = restZeqTranslator.translate( commandString, bodyJSON );

    const std::vector< float >& deserialized = zeroeq::hbp::deserializeCamera( zeroeqEvent );

    BOOST_CHECK_EQUAL_COLLECTIONS( matrixData.begin(), matrixData.end(),
                                   deserialized.begin(), deserialized.end( ) );
}

BOOST_AUTO_TEST_CASE( test_EventPUBLISHED )
{
    restbridge::detail::RestZeqTranslator restZeqTranslator;

    zeroeq::EventDescriptor eventDescpritor( zeroeq::hbp::IMAGEJPEG, zeroeq::hbp::EVENT_IMAGEJPEG,
                                          zeroeq::hbp::SCHEMA_IMAGEJPEG, zeroeq::PUBLISHER );

    restZeqTranslator.addPublishedEvent( eventDescpritor );

    const std::string commandString = "IMAGEJPEG";

    const zeroeq::Event& zeroeqEvent = restZeqTranslator.translate( commandString );

    BOOST_CHECK_EQUAL( zeroeq::hbp::EVENT_IMAGEJPEG,
                       zeroeq::vocabulary::deserializeRequest( zeroeqEvent ) );
}

BOOST_AUTO_TEST_CASE( test_unknownEvent )
{
    restbridge::detail::RestZeqTranslator restZeqTranslator;

    zeroeq::EventDescriptor eventDescpritor( zeroeq::hbp::CAMERA, zeroeq::hbp::EVENT_CAMERA,
                                          zeroeq::hbp::SCHEMA_CAMERA, zeroeq::BIDIRECTIONAL );

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
