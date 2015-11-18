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

#include <zeq/uri.h>

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
const std::string DEFAULT_PORT = "4020";
bool _isParameter( const int i, const std::string& arg,
                   const int argc, char* argv[] )
{
    return ( i < argc && arg == argv[i] && argv[i+1] && argv[i+1][0] != '-' );
}

}

namespace detail
{
class RestBridge
{
public:
    RestBridge( const int argc, char* argv[], const zeq::URI& uri )
        : pubURI( uri )
    {
        for( int i = 0; i < argc; ++i  )
        {
            if( _isParameter( i, "--rest", argc, argv ))
            {
                std::string http( argv[i+1] );
                const size_t colon = http.find( ':' );
                if( colon == std::string::npos )
                    httpHost = http;
                else
                {
                    httpHost = http.substr( 0, colon );
                    httpPort = http.substr( colon + 1 );
                }
            }
            else if( _isParameter( i, "--zeq-publisher", argc, argv ))
                pubURI = zeq::URI( argv[ i + 1 ] );
            else if( _isParameter( i, "--zeq-subscriber", argc, argv ))
                subURI = zeq::URI( argv[ i + 1 ] );
        }
        if( httpPort.empty( ))
            httpPort = DEFAULT_PORT;
        if( httpHost.empty( ))
            httpHost = "localhost";

        // Request handler
        //   pub and sub are from the app POV, so need to be swapped here
        _handler.reset( new RequestHandler( subURI, pubURI ));

        // http server
        Server::options options( *_handler );
        _server.reset( new Server( options.address( httpHost ).port( httpPort ).
                                   reuse_address( true )));
        _server->listen();

        RBDEBUG << "RestBridge running on http://" << httpHost << ":"
                << httpPort << " with ZeroEQ published on " << subURI
                << " subscribed to " << pubURI << std::endl;

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

    std::string httpHost;
    std::string httpPort;
    zeq::URI pubURI;
    zeq::URI subURI;
    std::unique_ptr< boost::thread > thread;

private:
    std::unique_ptr< Server > _server;
    std::unique_ptr< RequestHandler > _handler;
};
}

namespace
{
static bool _hasRestParameter( const int argc, char* argv[] )
{
    for( int i = 0; i < argc; ++i  )
        if( std::string( argv[i] ) == "--rest" )
            return true;
    return false;
}

}

std::unique_ptr< RestBridge > RestBridge::parse(
    const zeq::Publisher& publisher, int argc, char* argv[] )
{
    if( _hasRestParameter( argc, argv ))
        return std::unique_ptr< RestBridge >( new RestBridge( argc, argv,
                                                          publisher.getURI( )));
    return nullptr;
}

std::unique_ptr< RestBridge > RestBridge::parse( const int argc, char* argv[] )
{
    if( _hasRestParameter( argc, argv ))
        return std::unique_ptr< RestBridge >( new RestBridge( argc, argv,
                                                              zeq::URI( )));
    return nullptr;
}

std::unique_ptr< RestBridge > RestBridge::create( const int argc, char* argv[] )
{
    return std::unique_ptr< RestBridge >( new RestBridge( argc, argv,
                                                          zeq::URI( )));
}

std::string RestBridge::getHelp()
{
    return std::string(
        " --rest [host][:port]: Enable the REST bridge. Optional parameters\n"
        "        configure the web server, running by default on :" +
          DEFAULT_PORT + "\n" +
        " --zeq-publisher: URI where the application publishes ZeroEQ events\n"+
        " --zeq-subscriber: URI to where the application subscribes to\n" );
}

RestBridge::RestBridge( const int argc, char* argv[], const zeq::URI& uri )
    : _impl( new detail::RestBridge( argc, argv, uri ))
{
}

RestBridge::~RestBridge()
{
    delete _impl;
}

zeq::URI RestBridge::getPublisherURI() const
{
    return _impl->pubURI;
}

zeq::URI RestBridge::getSubscriberURI() const
{
    return _impl->subURI;
}

bool RestBridge::isRunning() const
{
    return _impl->thread != nullptr;
}

}
