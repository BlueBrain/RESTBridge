/**
 * @file    types.h
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2015-13-01
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#ifndef _types_h_
#define _types_h_

#include <boost/network/protocol/http/server.hpp>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>

namespace zeq
{
    class Publisher;
    class Subscriber;
    class Event;
}

namespace restconnector
{

//classes
class RequestHandler;
class RestZeqTranslator;

//typedefs
typedef boost::network::http::server<RequestHandler> server;

}

#endif // _types_h_
