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
#include <zeq/types.h>
#undef ECHO // On RedHat, ECHO is already defined and conflicts with the ECHO ZEq event

namespace restconnector
{

//classes
class RequestHandler;
class RestZeqTranslator;

//typedefs
typedef boost::network::http::server<RequestHandler> server;

}

#endif // _types_h_
