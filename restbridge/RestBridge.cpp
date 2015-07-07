/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include "RestBridge.h"
#include "detail/RequestHandler.h"

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
        { // scope is for automic unlocking of the mutex_ when the http server is stopped
            std::unique_lock<std::mutex> lock(mutex_);
            const std::string publisherSchema = schema + PUBLISHER_SCHEMA_SUFFIX;
            const std::string subscriberSchema = schema + SUBSCRIBER_SCHEMA_SUFFIX;
            // Create request handler
            RequestHandler handler( publisherSchema, subscriberSchema );

            server::options options( handler );
            std::stringstream port;
            port << port_;
            // Create http server
            httpServer_.reset( new server( options.address( hostname_ ).port( port.str( ))));

            std::cout << "HTTP Server runnning on " << hostname_ << ":" << port_ << std::endl;
            std::cout << "ZeroEq Publisher Schema : " << publisherSchema << std::endl;
            std::cout << "ZeroEq Subscriber Schema: " << subscriberSchema << std::endl;
            serverRunning_ = true;
            // Start processing http requests
            httpServer_->run();
        }
    }

    void stop()
    {
        if( !thread_ || !serverRunning_ )
            throw( std::runtime_error( "HTTP server is not running, cannot stop it" ));
        // Stop http server hosted by thread_
        httpServer_->stop();

        // Wait until the http server is stopped
        std::unique_lock<std::mutex> lock(mutex_);
        // Stop listening thread
        thread_->join();
        serverRunning_ = false;
        std::cout << "HTTP Server on " << hostname_ << ":" << port_
                  << " is stopped" << std::endl;
    }

    boost::scoped_ptr< boost::thread > thread_;
    boost::scoped_ptr< server > httpServer_;
    std::mutex mutex_;
    std::atomic<bool> serverRunning_;
    std::string hostname_;
    uint16_t port_;
};
}

RestBridge::RestBridge( const std::string& hostname, const uint16_t port )
    : _impl( new detail::RestBridge( hostname, port ))
{
}

RestBridge::~RestBridge()
{
    delete _impl;
}

void RestBridge::run( const std::string& schema )
{
    if( _impl->thread_ )
        throw( std::runtime_error( "HTTP server is already running" ));
    _impl->thread_.reset( new boost::thread(
        boost::bind( &detail::RestBridge::run, _impl, schema )));
    if( !_impl->waitForRunningState(2000) )
        throw( std::runtime_error( "HTTP server could not be started" ));
}

void RestBridge::stop()
{
    _impl->stop();
}

}
