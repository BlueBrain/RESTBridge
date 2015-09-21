/* Copyright (c) 2014-2015, Human Brain Project
 *                          Cyrille Favreau <cyrille.favreau@epfl.ch>
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

#include "RequestHandler.h"
#include "restbridge/log.h"
#include <zeq/zeq.h>

#include <zeq/hbp/vocabulary.h>

#include <servus/uri.h>

namespace restbridge
{
namespace detail
{

RequestHandler::RequestHandler( const std::string& publisherSchema,
                                const std::string& subscriberSchema )
    : subscriber_( new zeq::Subscriber( servus::URI( subscriberSchema ) ) )
    , publisher_( new zeq::Publisher( servus::URI( publisherSchema ) ) )
    , listening_( true )
    , listeningThread_( new boost::thread( boost::bind( &RequestHandler::listen_, this ) ) )
{
    subscriber_->registerHandler( zeq::vocabulary::EVENT_HEARTBEAT,
                                  boost::bind( &RequestHandler::onStartupHeartbeatEvent_, this ));
    subscriber_->registerHandler( zeq::vocabulary::EVENT_VOCABULARY,
                                  boost::bind( &RequestHandler::onVocabularyEvent_, this, _1 ));
    requestLock_.lock();
}

RequestHandler::~RequestHandler()
{
    listening_ = false;
    listeningThread_->join();
    requestLock_.unlock();
}

void RequestHandler::listen_()
{
    while( listening_ )
        while( subscriber_->receive( 200 ) ){}
}

void RequestHandler::operator() ( const server::request &request, server::response &response )
{
    const std::string& method = request.method;

    try
    {
        if( restZeqTranslator_.getCommand( request.destination ) == INTERNAL_CMD_VOCABULARY )
        {
            const std::string& vocabulary = restZeqTranslator_.getVocabulary();
            response_ = server::response::stock_reply( server::response::ok, vocabulary );
        }
        else if( ( method == REST_VERB_PUT ) || ( method == REST_VERB_POST ) )
        {
            const zeq::Event& event = restZeqTranslator_.translate( request.destination,
                                                                    request.body );
            processPUT_( event );
        }
        else if( method == REST_VERB_GET )
        {
            const zeq::Event& event = restZeqTranslator_.translate( request.destination );
            processGET_( event );
        }
        response = response_;
    }
    catch( const std::runtime_error& e )
    {
        response = server::response::stock_reply( server::response::bad_request,
                                                  e.what() );
    }
}

void RequestHandler::processPUT_( const zeq::Event& event )
{
    publisher_->publish( event );
    std::ostringstream data;
    response_ = server::response::stock_reply( server::response::ok, data.str() );
}

void RequestHandler::processGET_( const zeq::Event& event )
{
    publisher_->publish( event );

    //We lock here because we need to wait for the response before new requests are processed.
    //This is unlocked when the response for the published event is received.
    requestLock_.lock();
}

void RequestHandler::onEvent_( const zeq::Event& event )
{
    response_ = server::response::stock_reply( server::response::ok,
                                               zeq::vocabulary::deserializeJSON( event) );
    requestLock_.unlock();
}

void RequestHandler::onStartupHeartbeatEvent_()
{
    const zeq::Event& zeqEvent =
            zeq::vocabulary::serializeRequest( zeq::vocabulary::EVENT_VOCABULARY );
    publisher_->publish( zeqEvent );
}

void RequestHandler::onHeartbeatEvent_()
{
}

void RequestHandler::onVocabularyEvent_( const zeq::Event& event )
{
    subscriber_->deregisterHandler( zeq::vocabulary::EVENT_HEARTBEAT );
    subscriber_->registerHandler( zeq::vocabulary::EVENT_HEARTBEAT,
                                  boost::bind( &RequestHandler::onHeartbeatEvent_, this ) );
    zeq::EventDescriptors eventDescriptors = zeq::vocabulary::deserializeVocabulary( event );

    BOOST_FOREACH( const zeq::EventDescriptor& eventDescriptor, eventDescriptors )
    {
        addEventDescriptor_( eventDescriptor );
    }
}

void RequestHandler::addEventDescriptor_( const zeq::EventDescriptor& eventDescriptor )
{
    zeq::vocabulary::registerEvent( eventDescriptor.getEventType(), eventDescriptor.getSchema() );
    if( ( eventDescriptor.getEventDirection() == zeq::PUBLISHER ) ||
        ( eventDescriptor.getEventDirection() == zeq::BIDIRECTIONAL ) )
    {
        restZeqTranslator_.addPublishedEvent( eventDescriptor );
        subscriber_->registerHandler( eventDescriptor.getEventType(),
                                      boost::bind( &RequestHandler::onEvent_, this, _1 ) );
    }
    if( ( eventDescriptor.getEventDirection() == zeq::SUBSCRIBER ) ||
        ( eventDescriptor.getEventDirection() == zeq::BIDIRECTIONAL ) )
    {
        restZeqTranslator_.addSubscribedEvent( eventDescriptor );
    }
}

void RequestHandler::log( server::string_type const &info )
{
    RBERROR << info << std::endl;
}

}
}
