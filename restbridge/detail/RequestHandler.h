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

#include <boost/network/protocol/http/server.hpp>
#undef ECHO // On RedHat, ECHO is already defined and conflicts with the ECHO ZEq event

namespace restbridge
{
namespace detail
{

class RequestHandler;
typedef boost::network::http::server<RequestHandler> server;

static const std::string REST_VERB_GET = "GET";
static const std::string REST_VERB_PUT = "PUT";
static const std::string REST_VERB_DELETE = "DELETE";
static const std::string REST_VERB_PATCH = "PATCH";
static const std::string REST_VERB_POST = "POST";

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
    void operator() ( const server::request &request, server::response &response );

    /**
     * Callback method used for logging internal errors raised by the underlying
     * cppnetlib library.
     * @param info String containing the error message
     */
    void log( server::string_type const &info );

private:
    void onStartupHeartbeatEvent_();
    void onHeartbeatEvent_();
    void onVocabularyEvent_( const zeq::Event& event );
    void onEvent_( const zeq::Event& event );
    void addEventDescriptor_( const zeq::EventDescriptor& eventDescriptor );

    void processPUT_( const zeq::Event& event );
    void processGET_( const zeq::Event& event );
    void listen_();

    boost::scoped_ptr< zeq::Subscriber > subscriber_;
    boost::scoped_ptr< zeq::Publisher > publisher_;

    boost::mutex requestLock_;

    bool listening_;
    boost::scoped_ptr< boost::thread > listeningThread_;

    server::request request_;
    server::response response_;

    RestZeqTranslator restZeqTranslator_;
};

}
}
#endif // RESTBRIDGE_REQUESTHANDLER_H
