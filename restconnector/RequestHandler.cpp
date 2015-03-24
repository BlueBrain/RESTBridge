/* Copyright (c) 2014-2015, Human Brain Project
 *                          Cyrille Favreau <cyrille.favreau@epfl.ch>
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include <restconnector/RequestHandler.h>
#include <restconnector/RestZeqTranslator.h>

#include <zeq/zeq.h>

#include <zeq/hbp/vocabulary.h>

#include <lunchbox/uri.h>
#include <lunchbox/log.h>

namespace restconnector
{

RequestHandler::RequestHandler( const std::string& publisherSchema,
                                const std::string& subscriberSchema )
    : subscriber_( new zeq::Subscriber( lunchbox::URI( subscriberSchema ) ) )
    , publisher_( new zeq::Publisher( lunchbox::URI( publisherSchema ) ) )
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
}

void RequestHandler::listen_()
{
    while( listening_ )
        while( subscriber_->receive( 200 ) ){}
}

void RequestHandler::operator() ( const server::request &request, server::response &response )
{
    server::string_type ip = source( request );
    RestZeqTranslator restZeqTranslator;

    if ( !restZeqTranslator.isCommandSupported( request.destination ) )
    {
        response = server::response::stock_reply( server::response::bad_request,
                                                  "Command not found." );
    }
    else
    {
        const std::string& method = request.method;
        try
        {
            const ::zeq::Event zeqEvent = restZeqTranslator.translate( request.body );

            if( method == "PUT" )
                processPUT_( zeqEvent );
            else if( method == "GET" )
                processGET_( zeqEvent );
            else if( method == "POST" )
                processPOST_( zeqEvent );

            response = response_;
        }
        catch( const std::runtime_error& e )
        {
            response = server::response::stock_reply( server::response::bad_request,
                                                      e.what() );
        }
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

void RequestHandler::processPOST_( const zeq::Event& event )
{
    publisher_->publish( event  );
    std::ostringstream data;
    response_ = server::response::stock_reply( server::response::ok, data.str() );
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
    LBVERB << "Heartbeat event received." << std::endl;
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
    vocabulary_[ eventDescriptor.getRestName() ] = eventDescriptor.getEventType();
    subscriber_->registerHandler( eventDescriptor.getEventType(),
                                  boost::bind( &RequestHandler::onEvent_, this, _1 ) );
}

void RequestHandler::log( server::string_type const &info )
{
    LBERROR << info << std::endl;
}

}
