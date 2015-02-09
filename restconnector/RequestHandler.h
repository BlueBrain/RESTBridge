/* Copyright (c) 2014-2015, Human Brain Project
 *                          Cyrille Favreau <cyrille.favreau@epfl.ch>
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef _RequestHandler_h_
#define _RequestHandler_h_

#include <restconnector/types.h>
#include <restconnector/RestZeqTranslator.h>

namespace restconnector
{

class RequestHandler
{
public:

    RequestHandler();

    ~RequestHandler();

    /*<< This is the function that handles the incoming request. >>*/
    void operator() ( const server::request &request, server::response &response );

    void unlock();

    /*<< It's necessary to define a log function, but it's ignored in
         this example. >>*/
    void log(...) {
        // do nothing
    }

private:

    void onImageRawRGBA8Event_( const ::zeq::Event& event );

    void processPUT_( const ::zeq::Event& event );
    void processGET_( const ::zeq::Event& event );
    void processPOST_( const ::zeq::Event& event );

    boost::shared_ptr<boost::condition_variable> condition_;
    int counter_;

    boost::shared_ptr< ::zeq::Subscriber > subscriber_;
    boost::shared_ptr< ::zeq::Publisher > publisher_;

    bool blocked_;
    server::request request_;
    server::response response_;
};

}
#endif // _RequestHandler_h_
