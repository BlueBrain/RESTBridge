/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 *                          Stefan.Eilemann@epfl.ch
 *
 * This file is part of RESTBridge <https://github.com/BlueBrain/RESTBridge>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "RestBridge.h"
#include "restbridge/log.h"
#include "detail/RequestHandler.h"

#include <servus/uri.h>

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <condition_variable>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>

namespace restbridge
{
namespace
{
static const std::string DEFAULT_SCHEME = "hbp";
static const uint16_t DEFAULT_PORT = 4020;
static const std::string PUBLISHER_SCHEME_SUFFIX = "req";
static const std::string SUBSCRIBER_SCHEME_SUFFIX = "rep";
}

namespace detail
{
class RestBridge
{
public:
    RestBridge( const int argc, char* argv[] )
    {
        for( int i = 0; i < argc; ++i  )
        {
            if( std::string( argv[i] ) == "--rest" &&
                i < argc && argv[i+1] && argv[i+1][0] != '-' )
            {
                uri = servus::URI( std::string( argv[i+1] ));
            }
        }

        if( uri.getScheme().empty( ))
            uri.setScheme( DEFAULT_SCHEME );
        if( uri.getPort() == 0 )
            uri.setPort( DEFAULT_PORT );
        if( uri.getHost().empty( ))
            uri.setHost( "localhost" );

        // Request handler
        const std::string pubScheme = uri.getScheme() + PUBLISHER_SCHEME_SUFFIX;
        const std::string subScheme = uri.getScheme() +SUBSCRIBER_SCHEME_SUFFIX;
        _handler.reset( new RequestHandler( pubScheme, subScheme ));

        // http server
        Server::options options( *_handler );
        _server.reset( new Server( options.address( uri.getHost( )).
                                   port( std::to_string( int( uri.getPort( )))).
                                   reuse_address( true )));

        RBDEBUG << "RestBridge running on http://" << uri.getHost() << ":"
                << uri.getPort() << std::endl;

        thread.reset( new boost::thread(
                          boost::bind( &detail::Server::run, _server.get( ))));
    }

    ~RestBridge()
    {
        if( _server )
            _server->stop();
        if( thread )
            thread->join();
        RBDEBUG << "HTTP Server stopped" << std::endl;
    }

    servus::URI uri;
    std::unique_ptr< boost::thread > thread;

private:
    std::unique_ptr< Server > _server;
    std::unique_ptr< RequestHandler > _handler;
};
}

std::unique_ptr< RestBridge > RestBridge::parse( const int argc, char* argv[] )
{
    for( int i = 0; i < argc; ++i  )
        if( std::string( argv[i] ) == "--rest" )
            return std::unique_ptr< RestBridge >( new RestBridge( argc, argv ));
    return nullptr;
}

std::unique_ptr< RestBridge > RestBridge::create( const int argc, char* argv[] )
{
    return std::unique_ptr< RestBridge >( new RestBridge( argc, argv ));
}

std::string RestBridge::getHelp()
{
    static const std::string help(
        "  --rest [host][:port]: Enable the REST bridge. Optional parameters\n"
        "         configure the web server, running by default on :" +
                                  std::to_string( int( DEFAULT_PORT )));
    return help;
}

RestBridge::RestBridge( const int argc, char* argv[] )
    : _impl( new detail::RestBridge( argc, argv ))
{
}

RestBridge::~RestBridge()
{
    delete _impl;
}

servus::URI RestBridge::getPublisherURI() const
{
    return servus::URI( _impl->uri.getScheme() + SUBSCRIBER_SCHEME_SUFFIX +
                        "://" );
}

servus::URI RestBridge::getSubscriberURI() const
{
    return servus::URI( _impl->uri.getScheme() + PUBLISHER_SCHEME_SUFFIX +
                        "://?subscribeSelf=true" );
}

bool RestBridge::isRunning() const
{
    return _impl->thread != nullptr;
}

}
