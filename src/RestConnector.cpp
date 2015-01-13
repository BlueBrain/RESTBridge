/**
 * @file    RestConnector.cpp
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2015-01-13
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <src/RestConnector.h>

RestConnector::RestConnector( const float timeOut, const std::string& address,
                              const std::string& port )
    : timeOut_( timeOut ),
      address_( address ),
      port_( port )
{}

RestConnector::~RestConnector(){}

void RestConnector::RestConnector::run()
{
    RequestHandler handler;

    server::options options( handler );
    server server_( options.address( address_ ).port( port_ ) );

    //boost::thread t( &MyThread::run, &handler );

    server_.run();

    //t.join();
}
