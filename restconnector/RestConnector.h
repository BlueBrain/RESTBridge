/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef RESTCONNECTOR_RESTCONNECTOR_H
#define RESTCONNECTOR_RESTCONNECTOR_H

#include <restconnector/types.h>
#include <boost/noncopyable.hpp>
#include <string>

namespace restconnector
{

namespace detail { class RestConnector; }

/**
 * The RestConnector class is responsible starting a HTTP server
 * in a dedicated thread, according to a given hostname and port.
 * HTTP requests are forwarded to an implicitely registered handler.
 */
class RestConnector : public boost::noncopyable
{
public:
    /**
     * Default constructor.
     * @param hostname Hostname or IP address
     * @param port Listening port
     */
    RestConnector( const std::string& hostname, const uint16_t port );

    ~RestConnector();

    /**
     * Listening to HTTP requests and forwarding them to the handler in
     * a dedicated thread.
     * @param schema Schema prefix used by zeq publisher and subscriber
     * @throw std::runtime_error if HTTP server is already running or could not be started
     */
    void run( const std::string& schema );

    /**
     * Stop the HTTP server thread.
     * @throw std::runtime_error if HTTP server is not running
     */
    void stop();

private:
    detail::RestConnector* const _impl;
};

}
#endif // RESTCONNECTOR_RESTCONNECTOR_H
