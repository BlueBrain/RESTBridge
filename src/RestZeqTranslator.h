/**
 * @file    RestZeqTranslator.h
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2014-27-11
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#ifndef _RestZeqTranslator_h_
#define _RestZeqTranslator_h_

#include <vector>
#include <string>
#include <zeq/event.h>

class RestZeqTranslator
{
public:
    RestZeqTranslator(){}
    virtual ~RestZeqTranslator(){}
    virtual zeq::Event translate( const std::string& command, const std::vector< std::string >& params,
                           const std::vector< std::string >& values) = 0;
};

#endif // _RestZeqTranslator_h_

