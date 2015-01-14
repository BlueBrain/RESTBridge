/**
 * @file    RequestHandler.cpp
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2014-26-11
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <src/RequestHandler.h>
#include <src/RestParser.h>

#include <zeq/event.h>
#include <zeq/subscriber.h>
#include <zeq/vocabulary.h>
#include <zeq/publisher.h>

#include <lunchbox/uri.h>

RequestHandler::RequestHandler( RestZeqTranslatorPtr restZeqTranslator )
    : condition_( new boost::condition_variable ),
      counter_(0),
      restZeqTranslator_( restZeqTranslator )
{
    publisher_.reset( new zeq::Publisher( lunchbox::URI( "camera://" ) ) );
}

RequestHandler::~RequestHandler(){}

void RequestHandler::operator() ( const server::request &request, server::response &response )
{
    server::string_type ip = source( request );
    unsigned int port = request.source_port;

    boost::mutex io_mutex;
    boost::mutex::scoped_lock lock( io_mutex );

    std::cout << "method :" << request.method << std::endl;

    RestParser parser;
    if( parser.parse( request.destination ) )
    {
        // Translate REST event into Zeq event
        zeq::Event zeqEvent = restZeqTranslator_->translate( parser.getCommand(), parser.getKeys(),
                                                             parser.getValues() );

        // Publish Zeq event
        publisher_->publish( zeqEvent );

        // Return response
        std::ostringstream data;
        data << "I am the mighty Server :) --> " << ip << ':' << port << "! Command: " << request.destination ;
        response = server::response::stock_reply( server::response::ok, data.str() );
    }
    else
    {
        std::ostringstream data;
        data << "Bad " << ip << ':' << port << "! Command: " << request.destination ;
        response = server::response::stock_reply( server::response::bad_request, data.str() );
    }

    //condition_->wait(lock);
}

void RequestHandler::unlock()
{
    condition_->notify_one();
}
