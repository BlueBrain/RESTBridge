/* Copyright (c) 2014-2015, Human Brain Project
 *                          Cyrille Favreau <cyrille.favreau@epfl.ch>
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include <restconnector/RequestHandler.h>
#include <restconnector/RestZeqTranslator.h>

#include <zeq/event.h>
#include <zeq/subscriber.h>
#include <zeq/vocabulary.h>
#include <zeq/publisher.h>

#include <zeq/hbp/vocabulary.h>
#include <zeq/hbp/camera_generated.h>
#include <zeq/hbp/selections_generated.h>
#include <zeq/hbp/request_generated.h>
#include <zeq/hbp/imageRawRGBA8_generated.h>

#include <lunchbox/uri.h>

namespace restconnector
{

RequestHandler::RequestHandler()
    : condition_( new boost::condition_variable )
    , counter_(0)
    , blocked_( false )
{
    publisher_.reset( new ::zeq::Publisher( lunchbox::URI( "restCommand10025://" ) ) );
    subscriber_.reset( new ::zeq::Subscriber( lunchbox::URI( "restResponse10025://" ) ) );
    subscriber_->registerHandler( ::zeq::hbp::EVENT_IMAGERAWRGBA8,
                                  boost::bind( &RequestHandler::onImageRawRGBA8Event_, this, _1 ));
}

RequestHandler::~RequestHandler(){}

void RequestHandler::operator() ( const server::request &request, server::response &response )
{
    request_ = request;
    server::string_type ip = source( request );
    RestZeqTranslator restZeqTranslator;

    if ( restZeqTranslator.findCommand( request.destination ) )
    {
        std::string method = request.method;

        ::zeq::Event zeqEvent = restZeqTranslator.translate();

        if( method == "PUT" )
            processPUT_( zeqEvent );
        else if( method == "GET" )
            processGET_( zeqEvent );
        else if( method == "POST" )
            processPOST_( zeqEvent );

        response = response_;
    }
    else
    {
        response = server::response::stock_reply( server::response::bad_request,
                                                  "Command not found." );
    }

}

void RequestHandler::processPUT_( const ::zeq::Event& event )
{
    publisher_->publish( event );
    std::ostringstream data;
    data << "Detected a PUT." <<std::endl;
    response_ = server::response::stock_reply( server::response::ok, data.str() );
}

void RequestHandler::processGET_( const ::zeq::Event& event )
{
    blocked_ = true;
    publisher_->publish( event );

    while( blocked_ )
    {
        while( subscriber_->receive( 0 ) ){}
    }
}

void RequestHandler::processPOST_( const ::zeq::Event& event )
{
    publisher_->publish( event  );
    std::ostringstream data;
    data << "Detected a POST." <<std::endl;
    response_ = server::response::stock_reply( server::response::ok, data.str() );
}

void RequestHandler::onImageRawRGBA8Event_( const ::zeq::Event& event )
{
    ::zeq::hbp::data::ImageRawRGBA8 image( ::zeq::hbp::deserializeImageRawRGBA8( event ) );

    std::ostringstream data;
    response_ = server::response::stock_reply( server::response::ok,
                                               ::zeq::vocabulary::deserializeJSON( event) );
    blocked_ = false;
}

void RequestHandler::unlock()
{
    condition_->notify_one();
}

}
