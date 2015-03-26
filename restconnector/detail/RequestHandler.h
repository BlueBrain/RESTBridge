/* Copyright (c) 2014-2015, Human Brain Project
 *                          Cyrille Favreau <cyrille.favreau@epfl.ch>
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef RESTCONNECTOR_REQUESTHANDLER_H
#define RESTCONNECTOR_REQUESTHANDLER_H

#include <restconnector/types.h>
#include <zeq/types.h>

#include <boost/network/protocol/http/server.hpp>
#undef ECHO // On RedHat, ECHO is already defined and conflicts with the ECHO ZEq event

namespace restconnector
{
namespace detail
{

class RequestHandler;
typedef boost::network::http::server<RequestHandler> server;

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
    void processPOST_( const zeq::Event& event );
    void listen_();

    boost::scoped_ptr< zeq::Subscriber > subscriber_;
    boost::scoped_ptr< zeq::Publisher > publisher_;

    boost::mutex requestLock_;

    bool listening_;
    boost::scoped_ptr< boost::thread > listeningThread_;

    server::request request_;
    server::response response_;

    std::map< std::string, lunchbox::uint128_t > vocabulary_;
};

}
}
#endif // RESTCONNECTOR_REQUESTHANDLER_H
