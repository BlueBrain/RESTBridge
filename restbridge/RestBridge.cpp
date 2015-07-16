/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include "RestBridge.h"
#include "restbridge/log.h"
#include "detail/RequestHandler.h"

#include <servus/uri.h>

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <condition_variable>
#include <mutex>
#include <atomic>

namespace restbridge
{

static const std::string PUBLISHER_SCHEMA_SUFFIX = "cmd://";
static const std::string SUBSCRIBER_SCHEMA_SUFFIX = "resp://";

namespace detail
{
class RestBridge
{
public:
    RestBridge( const std::string& hostname, const uint16_t port )
        : serverRunning_( false )
        , hostname_( hostname )
        , port_( port )
    {
    }

    RestBridge( const int argc, const char** argv )
        : serverRunning_( false )
    {
        for( int i = 0; i < argc; ++i  )
        {
            if( std::string( argv[i] ) == "--restbridge-zeq" )
            {
                try
                {
                     const servus::URI uri( std::string( argv[i+1] ));
                     if( uri.getScheme().empty() )
                         RBTHROW( std::runtime_error( "Empty zeq schema" ));

                     if( uri.getHost().empty() )
                         RBTHROW( std::runtime_error( "Empty host" ));

                     if( !uri.getPort())
                         RBTHROW( std::runtime_error( "Port is zero" ));

                     schema_ = uri.getScheme();
                     hostname_ = uri.getHost();
                     port_ = uri.getPort();
                }
                catch( ... )
                {
                    RBTHROW( std::runtime_error( "Servus uri parsing error" ));
                }
            }
        }
    }

    ~RestBridge()
    {
        if( serverRunning_ )
        {
            try
            {
                stop();
            }
            catch( ... ) {}
        }
    }

    bool waitForRunningState(const uint16_t milliseconds)
    {
        sleep(milliseconds/1000.f);
        return serverRunning_;
    }

    void run( const std::string& schema )
    {
        if( !schema.empty() )
            schema_ = schema;
        std::unique_lock<std::mutex> lock(mutex_);
        const std::string publisherSchema = schema_ + PUBLISHER_SCHEMA_SUFFIX;
        const std::string subscriberSchema = schema_ + SUBSCRIBER_SCHEMA_SUFFIX;
        // Create request handler
        RequestHandler handler( publisherSchema, subscriberSchema );

        server::options options( handler );
        std::stringstream port;
        port << port_;
        // Create http server
        httpServer_.reset( new server( options.address( hostname_ ).port( port.str( ))));

        RBDEBUG << "HTTP Server runnning on " << hostname_ << ":" << port_ << std::endl;
        RBDEBUG << "ZeroEq Publisher Schema : " << publisherSchema << std::endl;
        RBDEBUG << "ZeroEq Subscriber Schema: " << subscriberSchema << std::endl;
        serverRunning_ = true;
        // Start processing http requests
        httpServer_->run();
    }

    void stop()
    {
        if( !thread_ || !serverRunning_ )
            RBTHROW( std::runtime_error( "HTTP server is not running, cannot stop it" ));
        // Stop http server hosted by thread_
        httpServer_->stop();

        // Wait until the http server is stopped
        std::unique_lock<std::mutex> lock(mutex_);
        // Stop listening thread
        thread_->join();
        serverRunning_ = false;
        RBDEBUG << "HTTP Server on " << hostname_ << ":" << port_
                << " is stopped" << std::endl;
    }

    boost::scoped_ptr< boost::thread > thread_;
    boost::scoped_ptr< server > httpServer_;
    std::mutex mutex_;
    std::atomic<bool> serverRunning_;
    std::string hostname_;
    uint16_t port_;
    std::string schema_;
};
}

RestBridge::RestBridge( const std::string& hostname, const uint16_t port )
    : _impl( new detail::RestBridge( hostname, port ))
{
}

RestBridge::RestBridge( const int argc, const char** argv )
    : _impl( new detail::RestBridge( argc, argv ))
{
}

RestBridge::~RestBridge()
{
    delete _impl;
}

void RestBridge::run( const std::string& schema )
{
    if( _impl->thread_ )
        RBTHROW( std::runtime_error( "HTTP server is already running" ));
    _impl->thread_.reset( new boost::thread(
        boost::bind( &detail::RestBridge::run, _impl, schema )));
    if( !_impl->waitForRunningState(2000) )
        RBTHROW( std::runtime_error( "HTTP server could not be started" ));
}

void RestBridge::stop()
{
    _impl->stop();
}

}
