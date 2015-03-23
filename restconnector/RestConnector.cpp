/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include <restconnector/RestConnector.h>
#include <lunchbox/log.h>
#include <lunchbox/debug.h>

namespace restconnector
{

static const std::string PUBLISHER_SCHEMA_SUFFIX = "cmd://";
static const std::string SUBSCRIBER_SCHEMA_SUFFIX = "resp://";

RestConnector::RestConnector( const std::string& hostname, const uint16_t port )
    : hostname_( hostname )
    , port_( port )
{
    LBASSERT( !hostname.empty() );
}

RestConnector::~RestConnector(){}

void RestConnector::RestConnector::run( const std::string& schema ) const
{
    boost::thread main( boost::bind( &RestConnector::run_, this, schema ) );
    main.join();
}

void RestConnector::RestConnector::run_( const std::string& schema ) const
{
    const std::string publisherSchema = schema + PUBLISHER_SCHEMA_SUFFIX;
    const std::string subscriberSchema = schema + SUBSCRIBER_SCHEMA_SUFFIX;
    RequestHandler handler( publisherSchema, subscriberSchema );

    server::options options( handler );
    std::stringstream port;
    port << port_;
    server httpServer( options.address( hostname_ ).port( port.str() ) );

    LBINFO << "HTTP Server runnning on " << hostname_ << ":" << port_ << std::endl;
    LBINFO << "ZeroEq Publisher Schema : " << publisherSchema << std::endl;
    LBINFO << "ZeroEq Subscriber Schema: " << subscriberSchema << std::endl;
    httpServer.run();
}

}
