/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef _RestConnector_h_
#define _RestConnector_h_

#include <restconnector/types.h>
#include <restconnector/RequestHandler.h>

namespace restconnector
{

/**
 * The RestConnector class is responsible starting a HTTP server
 * in a dedicated thread, according to a given hostname and port.
 * HTTP requests are forwarded to an implicitely registered handler.
 */
class RestConnector
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
     */
    void run( const std::string& schema ) const;

private:

    void run_( const std::string& schema ) const;
    std::string hostname_;
    uint16_t port_;
};

}
#endif // _RequestHandler_h_
