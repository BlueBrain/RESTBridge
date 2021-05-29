/* Copyright (c) 2013-2021, EPFL/Blue Brain Project
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
#include <zeroeq/zeroeq.h>

#include <zeroeq/hbp/vocabulary.h>

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
RequestHandler::RequestHandler( zeroeq::URI& publisherURI,
                                const zeroeq::URI& subscriberURI )
    : _subscriber( subscriberURI, zeroeq::DEFAULT_SESSION )
    , _publisher( publisherURI, zeroeq::DEFAULT_SESSION )
    , _listening( true )
    , _thread( boost::bind( &RequestHandler::listen_, this ))
{
    publisherURI = _publisher.getURI();
    _subscriber.registerHandler( zeroeq::vocabulary::EVENT_HEARTBEAT,
                                  boost::bind( &RequestHandler::onStartupHeartbeatEvent_, this ));
    _subscriber.registerHandler( zeroeq::vocabulary::EVENT_VOCABULARY,
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
            const zeroeq::Event& event =
                _translator.translate( request.destination, request.body );
            response = processPUT_( event );
            return;
        }
        if( method == REST_VERB_GET )
        {
            const zeroeq::Event& event =
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

Server::response RequestHandler::processPUT_( const zeroeq::Event& event )
{
    _publisher.publish( event );
    return Server::response::stock_reply( Server::response::ok, std::string( ));
}

Server::response RequestHandler::processGET_( const zeroeq::Event& event )
{
    const auto& type = ::zeroeq::vocabulary::deserializeRequest( event );
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

void RequestHandler::onEvent_( const zeroeq::Event& event,
                               const zeroeq::uint128_t& expected,
                               Server::response& response )
{
    if( event.getType() != expected )
        return;

    response = Server::response::stock_reply( Server::response::ok,
                                     zeroeq::vocabulary::deserializeJSON( event ));
    _requestLock.unlock();
}

void RequestHandler::onStartupHeartbeatEvent_()
{
    const zeroeq::Event& zeroeqEvent =
            zeroeq::vocabulary::serializeRequest( zeroeq::vocabulary::EVENT_VOCABULARY );
    _publisher.publish( zeroeqEvent );
}

void RequestHandler::onHeartbeatEvent_()
{
}

void RequestHandler::onVocabularyEvent_( const zeroeq::Event& event )
{
    _subscriber.deregisterHandler( zeroeq::vocabulary::EVENT_VOCABULARY );
    _subscriber.deregisterHandler( zeroeq::vocabulary::EVENT_HEARTBEAT );
    _subscriber.registerHandler( zeroeq::vocabulary::EVENT_HEARTBEAT,
                                  boost::bind( &RequestHandler::onHeartbeatEvent_, this ) );
    const zeroeq::EventDescriptors& eventDescriptors =
        zeroeq::vocabulary::deserializeVocabulary( event );

    for( const zeroeq::EventDescriptor& eventDescriptor : eventDescriptors )
        addEventDescriptor_( eventDescriptor );
}

void RequestHandler::addEventDescriptor_(
    const zeroeq::EventDescriptor& descriptor )
{
    zeroeq::vocabulary::registerEvent( descriptor.getEventType(),
                                    descriptor.getSchema( ));

    if( ( descriptor.getEventDirection() == zeroeq::PUBLISHER ) ||
        ( descriptor.getEventDirection() == zeroeq::BIDIRECTIONAL ) )
    {
        _translator.addPublishedEvent( descriptor );
    }
    if( ( descriptor.getEventDirection() == zeroeq::SUBSCRIBER ) ||
        ( descriptor.getEventDirection() == zeroeq::BIDIRECTIONAL ) )
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
