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
namespace
{
static const std::string REST_VERB_GET = "GET";
static const std::string REST_VERB_PUT = "PUT";
static const std::string REST_VERB_DELETE = "DELETE";
static const std::string REST_VERB_PATCH = "PATCH";
static const std::string REST_VERB_POST = "POST";
}

namespace detail
{
RequestHandler::RequestHandler( zeq::URI& publisherURI,
                                const zeq::URI& subscriberURI )
    : _subscriber( subscriberURI )
    , _publisher( publisherURI )
    , _listening( true )
    , _thread( boost::bind( &RequestHandler::listen_, this ))
{
    publisherURI = _publisher.getURI();
    _subscriber.registerHandler( zeq::vocabulary::EVENT_HEARTBEAT,
                                  boost::bind( &RequestHandler::onStartupHeartbeatEvent_, this ));
    _subscriber.registerHandler( zeq::vocabulary::EVENT_VOCABULARY,
                                  boost::bind( &RequestHandler::onVocabularyEvent_, this, _1 ));
    _requestLock.lock();
}

RequestHandler::~RequestHandler()
{
    _listening = false;
    _thread.join();
    _requestLock.unlock();
}

void RequestHandler::listen_()
{
    while( _listening )
        while( _subscriber.receive( 200 ) ){}
}

void RequestHandler::operator() ( const Server::request& request,
                                  Server::response& response )
{
    const std::string& method = request.method;

    try
    {
        if( _translator.getCommand( request.destination ) ==
            INTERNAL_CMD_VOCABULARY )
        {
            const std::string& vocabulary = _translator.getVocabulary();
            response = Server::response::stock_reply( Server::response::ok,
                                                      vocabulary );
            return;
        }
        if( method == REST_VERB_PUT || method == REST_VERB_POST )
        {
            const zeq::Event& event =
                _translator.translate( request.destination, request.body );
            response = processPUT_( event );
            return;
        }
        if( method == REST_VERB_GET )
        {
            const zeq::Event& event =
                _translator.translate( request.destination );
            response = processGET_( event );
            return;
        }

        response = Server::response::stock_reply( Server::response::bad_request,
                                                  "Unsupported http method " +
                                                  method );
    }
    catch( const std::runtime_error& e )
    {
        response = Server::response::stock_reply( Server::response::bad_request,
                                                  e.what( ));
    }
}

Server::response RequestHandler::processPUT_( const zeq::Event& event )
{
    _publisher.publish( event );
    return Server::response::stock_reply( Server::response::ok, std::string( ));
}

Server::response RequestHandler::processGET_( const zeq::Event& event )
{
    const auto& type = ::zeq::vocabulary::deserializeRequest( event );
    Server::response response;
    _subscriber.registerHandler( type,
                                 boost::bind( &RequestHandler::onEvent_, this,
                                              _1, type,
                                              boost::ref( response )));
    _publisher.publish( event );

    // We lock here because we need to wait for the response before new requests
    // are processed. This is unlocked when the response for the published event
    // is received.
    _requestLock.lock();
    _subscriber.deregisterHandler( type );
    return response;
}

void RequestHandler::onEvent_( const zeq::Event& event,
                               const zeq::uint128_t& expected,
                               Server::response& response )
{
    if( event.getType() != expected )
        return;

    response = Server::response::stock_reply( Server::response::ok,
                                     zeq::vocabulary::deserializeJSON( event ));
    _requestLock.unlock();
}

void RequestHandler::onStartupHeartbeatEvent_()
{
    const zeq::Event& zeqEvent =
            zeq::vocabulary::serializeRequest( zeq::vocabulary::EVENT_VOCABULARY );
    _publisher.publish( zeqEvent );
}

void RequestHandler::onHeartbeatEvent_()
{
}

void RequestHandler::onVocabularyEvent_( const zeq::Event& event )
{
    _subscriber.deregisterHandler( zeq::vocabulary::EVENT_VOCABULARY );
    _subscriber.deregisterHandler( zeq::vocabulary::EVENT_HEARTBEAT );
    _subscriber.registerHandler( zeq::vocabulary::EVENT_HEARTBEAT,
                                  boost::bind( &RequestHandler::onHeartbeatEvent_, this ) );
    const zeq::EventDescriptors& eventDescriptors =
        zeq::vocabulary::deserializeVocabulary( event );

    for( const zeq::EventDescriptor& eventDescriptor : eventDescriptors )
        addEventDescriptor_( eventDescriptor );
}

void RequestHandler::addEventDescriptor_(
    const zeq::EventDescriptor& descriptor )
{
    zeq::vocabulary::registerEvent( descriptor.getEventType(),
                                    descriptor.getSchema( ));

    if( ( descriptor.getEventDirection() == zeq::PUBLISHER ) ||
        ( descriptor.getEventDirection() == zeq::BIDIRECTIONAL ) )
    {
        _translator.addPublishedEvent( descriptor );
    }
    if( ( descriptor.getEventDirection() == zeq::SUBSCRIBER ) ||
        ( descriptor.getEventDirection() == zeq::BIDIRECTIONAL ) )
    {
        _translator.addSubscribedEvent( descriptor );
    }
}

void RequestHandler::log( const Server::string_type& info )
{
    RBERROR << info << std::endl;
}

}
}
