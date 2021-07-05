/* Copyright (c) 2013-2021, EPFL/Blue Brain Project
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
     * Create a new REST bridge when requested.
     *
     * The creation and parameters depend on the following command line
     * parameters:
     * * --rest [host][:port]: Enable the REST bridge. Optional parameters
     *   configure the web server, running by default on :4020
     * * --zeroeq-publisher: URI where the application publishes ZeroEQ events
     * * --zeroeq-subscriber: URI to where the application subscribes to
     *
     * @param publisher Existing publisher of the application
     * @param argc Argument count
     * @param argv Argument list
     * @return a configured and running REST bridge, or an invalid pointer.
     * @throw std::runtime_error if the HTTP server could not be started.
     */
    static std::unique_ptr< RestBridge > parse( const zeroeq::Publisher& publisher,
                                                int argc, char* argv[] );

    /** @sa parse() above. */
    static std::unique_ptr< RestBridge > parse( int argc, char* argv[] );

    /** Always create a new RESTBridge. @sa create() for parameters. */
    static std::unique_ptr< RestBridge > create( int argc, char* argv[] );

    ~RestBridge();

    /** @return a string describing the parsed command line parameters. */
    static std::string getHelp();

    /** @return the URI to which the application should publish. */
    zeroeq::URI getPublisherURI() const;

    /** @return the URI to which the application should subscribe. */
    zeroeq::URI getSubscriberURI() const;

    /** @return true if the rest bridge is running. */
    bool isRunning() const;

private:
    RestBridge( const RestBridge& ) = delete;
    RestBridge& operator=( const RestBridge& ) = delete;
    detail::RestBridge* const _impl;

    RestBridge( int argc, char* argv[], const zeroeq::URI& );
};

}
#endif // RESTBRIDGE_RESTBRIDGE_H
