/**
 * @file    RequestHandler.h
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2014-26-11
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#ifndef _RequestHandler_h_
#define _RequestHandler_h_

#include <src/types.h>

class RequestHandler
{
public:

    RequestHandler();

    ~RequestHandler();

    /*<< This is the function that handles the incoming request. >>*/
    void operator() ( const server::request &request, server::response &response );

    void unlock();

    /*<< It's necessary to define a log function, but it's ignored in
         this example. >>*/
    void log(...) {
        // do nothing
    }

private:

    boost::shared_ptr<boost::condition_variable> condition_;
    int counter_;

};

#endif // _RequestHandler_h_
