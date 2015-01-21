/**
 * @file    RestConnector.cpp
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2015-01-13
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <restconnector/RestConnector.h>

namespace restconnector
{

RestConnector::RestConnector( const std::string& address, const std::string& port )
    : address_( address )
    , port_( port )
{}

RestConnector::~RestConnector(){}

void RestConnector::RestConnector::run( RestZeqTranslatorPtr restZeqTranslator )
{
    boost::thread main( boost::bind( &RestConnector::run_, this, restZeqTranslator ) );
    //main.join()
}

void RestConnector::RestConnector::run_( RestZeqTranslatorPtr restZeqTranslator )
{
    RequestHandler handler( restZeqTranslator );

    server::options options( handler );
    server server_( options.address( address_ ).port( port_ ) );

    server_.run();
}

}
