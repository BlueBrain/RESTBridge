/**
 * @file    RestParser.h
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2014-26-11
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#ifndef _RestParser_h_
#define _RestParser_h_

#include <vector>
#include <string>

/*<< Defines the request handler.  It's a class that defines two
     functions, `operator()` and `log()` >>*/
class RestParser
{
public:
    RestParser();

    bool parse( const std::string& data );

    std::string getCommand() { return command_; }

    std::vector< std::string > getParams() { return params_; }

    std::vector< std::string > getValues() { return values_; }

private:

    bool parseRestRequest_( const std::string& restRequest );

    //the result of parsing
    std::string command_;
    std::vector< std::string > params_;
    std::vector< std::string > values_;
};


#endif // _RestParser_h_
