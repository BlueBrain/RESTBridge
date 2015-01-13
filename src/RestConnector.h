/**
 * @file    RestConnector.h
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2015-01-13
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#ifndef _RestConnector_h_
#define _RestConnector_h_

#include <src/types.h>
#include <src/RequestHandler.h>

//class MyThread most probably will be removed...
class MyThread
{
public:
    static void run( RequestHandler* handler )
    {
        while(true)
        {
            sleep( 10 );
            handler->unlock();
        }
    }

};

class RestConnector
{
public:

    RestConnector( const float timeOut, const std::string& address, const std::string& port );

    ~RestConnector();

    void run();

private:

    float timeOut_;
    std::string address_;
    std::string port_;
};

#endif // _RequestHandler_h_
