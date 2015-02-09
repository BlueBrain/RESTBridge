/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include <restconnector/RestConnector.h>

namespace restconnector
{

RestConnector::RestConnector( const std::string& address, const std::string& port )
    : address_( address )
    , port_( port )
{}

RestConnector::~RestConnector(){}

void RestConnector::RestConnector::run()
{
    boost::thread main( boost::bind( &RestConnector::run_, this ) );
    main.join();
}

void RestConnector::RestConnector::run_()
{
    RequestHandler handler;

    server::options options( handler );
    server server_( options.address( address_ ).port( port_ ) );

    server_.run();
}

}
