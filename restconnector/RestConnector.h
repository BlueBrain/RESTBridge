/**
 * @file    RestConnector.h
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2015-01-13
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#ifndef _RestConnector_h_
#define _RestConnector_h_

#include <restconnector/types.h>
#include <restconnector/RequestHandler.h>

namespace restconnector
{

class RestConnector
{
public:

    RestConnector( const std::string& address, const std::string& port );

    ~RestConnector();

    void run( RestZeqTranslatorPtr restZeqTranslator );

private:

    void run_( RestZeqTranslatorPtr restZeqTranslator );
    std::string address_;
    std::string port_;
};

}
#endif // _RequestHandler_h_
