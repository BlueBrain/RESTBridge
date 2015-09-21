/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
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

#ifndef RESTBRIDGE_RESTBRIDGE_H
#define RESTBRIDGE_RESTBRIDGE_H

#include <restbridge/types.h>
#include <string>
#include <cstdint>

namespace restbridge
{

namespace detail { class RestBridge; }

/**
 * The RestBridge class is responsible starting a HTTP server
 * in a dedicated thread, according to a given hostname and port.
 * HTTP requests are forwarded to an implicitely registered handler.
 */
class RestBridge
{
public:
    /**
     * Default constructor.
     * @param hostname Hostname or IP address
     * @param port Listening port
     */
    RestBridge( const std::string& hostname, const uint16_t port );

    /**
     * This constructor extract the hostname, the port and the schema from the
     * command line arguments char array.
     * The format is: --restbridge-zeq schema://hostname:port
     * @param argc the command line argument count.
     * @param argv the command line argument values.
     */
    RestBridge( const int argc, const char** argv );

    ~RestBridge();

    /**
     * Listening to HTTP requests and forwarding them to the handler in
     * a dedicated thread.
     * @param schema Optional schema prefix used by zeq publisher and subscriber
     * @throw std::runtime_error if HTTP server is already running or could not be started
     */
    void run( const std::string& schema = "" );

    /**
     * Stop the HTTP server thread.
     * @throw std::runtime_error if HTTP server is not running
     */
    void stop();

private:
    RestBridge( const RestBridge& ) = delete;
    RestBridge& operator=( const RestBridge& ) = delete;
    detail::RestBridge* const _impl;
};

}
#endif // RESTBRIDGE_RESTBRIDGE_H
