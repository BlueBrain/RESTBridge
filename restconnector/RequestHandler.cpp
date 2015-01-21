/**
 * @file    RequestHandler.cpp
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2014-26-11
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <restconnector/RequestHandler.h>
#include <restconnector/RestParser.h>
#include <restconnector/RestZeqTranslator.h>
#include <restconnector/Lib/zeq/events.h>

#include <zeq/event.h>
#include <zeq/subscriber.h>
#include <zeq/vocabulary.h>
#include <zeq/publisher.h>
#include <zeq/hbp/vocabulary.h>

#include <lunchbox/uri.h>

namespace restconnector
{

RequestHandler::RequestHandler( RestZeqTranslatorPtr restZeqTranslator )
    : condition_( new boost::condition_variable )
    , counter_(0)
    , restZeqTranslator_( restZeqTranslator )
    , blocked_( false )
{
    publisher_.reset( new ::zeq::Publisher( lunchbox::URI( "restCommand://" ) ) );
    subscriber_.reset( new ::zeq::Subscriber( lunchbox::URI( "restResponse://" ) ) );
    subscriber_->registerHandler( zeq::EVENT_GENERIC_RESPONSE,
                                  boost::bind( &RequestHandler::onGenericResponse_, this, _1 ) );
}

RequestHandler::~RequestHandler(){}

void RequestHandler::operator() ( const server::request &request, server::response &response )
{
    request_ = request;
    //response_ = &response;
    blocked_ = true;
    server::string_type ip = source( request );
    unsigned int port = request.source_port;

    //boost::mutex io_mutex;
    //boost::mutex::scoped_lock lock( io_mutex );

    std::cout << "method :" << request.method << std::endl;

    RestParser parser;
    if( parser.parse( request.destination ) )
    {
        // Translate REST event into Zeq event
        ::zeq::Event zeqEvent = restZeqTranslator_->translate( parser.getCommand(), parser.getKeys(),
                                                             parser.getValues() );
        // Publish Zeq event
        publisher_->publish( zeqEvent );

        while( blocked_ )
        {
            while( subscriber_->receive( 0 ) )
            {
                std::cout<<"Waiting..."<<std::endl;
            }
        }
        std::cout<<"Deblocked"<<std::endl;
        response = response_;
    }
    else
    {
        std::ostringstream data;
        data << "Bad " << ip << ':' << port << "! Command: " << request.destination ;
        response = server::response::stock_reply( server::response::bad_request, data.str() );
    }

    //condition_->wait(lock);
}

void RequestHandler::onGenericResponse_( const ::zeq::Event& )
{
    std::cout << "Got Generic Info ZEQ Response!!" << std::endl;

    // Return response
    std::ostringstream data;
    data << "I am the mighty Server :) --> " <<std::endl;
    response_ = server::response::stock_reply( server::response::ok, data.str() );
    blocked_ = false;
}

void RequestHandler::unlock()
{
    condition_->notify_one();
}

}
