/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
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

    void run();

private:

    void run_();
    std::string address_;
    std::string port_;
};

}
#endif // _RequestHandler_h_
