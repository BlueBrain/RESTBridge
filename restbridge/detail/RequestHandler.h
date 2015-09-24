/* Copyright (c) 2014-2015, Human Brain Project
 *                          Cyrille Favreau <cyrille.favreau@epfl.ch>
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

#ifndef RESTBRIDGE_REQUESTHANDLER_H
#define RESTBRIDGE_REQUESTHANDLER_H

#include <restbridge/types.h>
#include <restbridge/detail/RestZeqTranslator.h>
#include <zeq/publisher.h>
#include <zeq/subscriber.h>
#include <boost/network/protocol/http/server.hpp>
#include <mutex>
#ifdef ECHO
#  undef ECHO // Global namespace pollution conflicts with the ECHO ZEq event
#endif

namespace restbridge
{
namespace detail
{

class RequestHandler;
typedef boost::network::http::server< RequestHandler > Server;

/**
 * The RequestHandler class handles incoming HTTP requests and
 * transforms them into corresponding zeq events based on registered
 * vocabularies.
 */
class RequestHandler
{
public:

    /**
     * Constructor
     * @param publisherSchema Schema on which zeq events will be published
     * @param subscriberSchema Schema on which zeq events will be received
     */
    RequestHandler( const std::string& publisherSchema,
                    const std::string& subscriberSchema );

    ~RequestHandler();

    /**
     * This operator in invoked whenever an HTTP request is accepted by the server.
     * It is responsible for processing the request and generating the response.
     * @param request Incoming HTTP request
     * @param response Response to be sent back
     */
    void operator() ( const Server::request& request,
                      Server::response& response );

    /**
     * Callback method used for logging internal errors raised by the underlying
     * cppnetlib library.
     * @param info String containing the error message
     */
    void log( const Server::string_type& info );

private:
    void onStartupHeartbeatEvent_();
    void onHeartbeatEvent_();
    void onVocabularyEvent_( const zeq::Event& event );
    void onEvent_( const zeq::Event& event );
    void addEventDescriptor_( const zeq::EventDescriptor& eventDescriptor );

    void processPUT_( const zeq::Event& event );
    void processGET_( const zeq::Event& event );
    void listen_();

    zeq::Subscriber subscriber_;
    zeq::Publisher publisher_;

    std::mutex requestLock_;

    bool listening_;
    boost::thread listeningThread_;

    Server::request request_;
    Server::response response_;

    RestZeqTranslator restZeqTranslator_;
};

}
}
#endif // RESTBRIDGE_REQUESTHANDLER_H
