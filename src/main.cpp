// Copyright 2009 (c) Tarro, Inc.
// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

//[ RequestHandler_server_main
/*`
  This is a part of the 'Hello World' example. It's used to
  demonstrate how easy it is to set up an HTTP server.  All we do in
  this example is create a request handler and run the server.
 */
#include <boost/network/protocol/http/server.hpp>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>

#include <zeq/event.h>
#include <zeq/subscriber.h>
#include <zeq/vocabulary.h>
#include <zeq/publisher.h>

#include <src/RestParser.h>

namespace http = boost::network::http;

const int nbMaxLocks=100;
float timeOut=0;

/*<< Defines the server. >>*/
class RequestHandler;
typedef http::server<RequestHandler> server;


/*<< Defines the request handler.  It's a class that defines two
     functions, `operator()` and `log()` >>*/
class RequestHandler
{
public:

    RequestHandler(): condition_(new boost::condition_variable)
    {
        counter_ = 0;
    }

    /*<< This is the function that handles the incoming request. >>*/
    void operator() ( const server::request &request, server::response &response )
    {
        //std::cout << "RequestHandler::operator() [" << counter_++ << "]" << std::endl;
        server::string_type ip = source(request);
        unsigned int port = request.source_port;

        boost::mutex io_mutex;
        boost::mutex::scoped_lock lock(io_mutex);

        //std::cout << "method :" << request.method << std::endl;
        //std::cout << "source :" << request.source << std::endl;
        //std::cout << "source_port :" << request.source_port << std::endl;
        //std::cout << "destination :" << request.destination << std::endl;
        //std::cout << "http_version_major :" << (uint)request.http_version_major << std::endl;
        //std::cout << "http_version_minor :" << (uint)request.http_version_minor << std::endl;
        //std::cout << request.headers << std::endl;
        //std::cout << "body :" << request.body << std::endl;
        RestParser parser;
        parser.parse( request.destination );

        //std::cout << "RequestHandler::operator() -> Waiting on lock ..." << std::endl;
        condition_->wait(lock);

        /*if( request.destination == "/1")
        {
            std::cout<<"************** A long one **************"<<std::endl;
            sleep(10);
        }*/

        //std::cout << "RequestHandler::operator() -> Lock released..." << std::endl;

        std::ostringstream data;
        data << "I am the mighty Server :) --> " << ip << ':' << port << "! Command: " << request.destination ;
        response = server::response::stock_reply( server::response::ok, data.str() );
    }

    void unlock( )
    {
        //std::cout << "RequestHandler::unlock -> Releasing ..." << std::endl;
        condition_->notify_one();
    }

    /*<< It's necessary to define a log function, but it's ignored in
         this example. >>*/
    void log(...) {
        // do nothing
    }

private:

    boost::shared_ptr<boost::condition_variable> condition_;
    int counter_;

};

/*
 *
 */
class MyThread
{
public:
    static void run( RequestHandler* handler )
    {
        std::cout << "Run with Handler: " << handler << std::endl;
        while(true)
        {
            //std::cout << "MyThread::run -> Sleeping..." << std::endl;
            sleep( timeOut );
            handler->unlock();
            //std::cout << "MyThread::run -> Slept enough" << std::endl;
        }
    }

};

/*
 *
 */
int main(int argc, char * argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " address port timeout" << std::endl;
        return 1;
    }

    try {
        timeOut =  boost::lexical_cast<float>( argv[3] );
        /*<< Creates the request handler. >>*/
        RequestHandler handler;
        std::cout << "Handler: " << &handler << std::endl;
        /*<< Creates the server. >>*/
        server::options options(handler);
        server server_(options.address(argv[1]).port(argv[2]));

        boost::thread t(&MyThread::run, &handler);

        /*<< Runs the server. >>*/
        server_.run();

        t.join();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
//]
