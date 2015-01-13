/**
 * @file    RequestHandler.cpp
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2014-26-11
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <src/RequestHandler.h>
#include <src/RestParser.h>

RequestHandler::RequestHandler()
    : condition_( new boost::condition_variable ),
      counter_(0)
{}

RequestHandler::~RequestHandler(){}

void RequestHandler::operator() ( const server::request &request, server::response &response )
{
    server::string_type ip = source( request );
    unsigned int port = request.source_port;

    boost::mutex io_mutex;
    boost::mutex::scoped_lock lock( io_mutex );

    std::cout << "method :" << request.method << std::endl;
    //std::cout << "source :" << request.source << std::endl;
    //std::cout << "source_port :" << request.source_port << std::endl;
    //std::cout << "destination :" << request.destination << std::endl;
    //std::cout << "http_version_major :" << (uint)request.http_version_major << std::endl;
    //std::cout << "http_version_minor :" << (uint)request.http_version_minor << std::endl;
    //std::cout << request.headers << std::endl;
    //std::cout << "body :" << request.body << std::endl;
    RestParser parser;
    parser.parse( request.destination );

    //condition_->wait(lock);

    std::ostringstream data;
    data << "I am the mighty Server :) --> " << ip << ':' << port << "! Command: " << request.destination ;
    response = server::response::stock_reply( server::response::ok, data.str() );
}

void RequestHandler::unlock()
{
    condition_->notify_one();
}
