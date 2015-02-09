/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef _RestZeqTranslator_h_
#define _RestZeqTranslator_h_

#include <vector>
#include <string>
#include <zeq/event.h>

namespace restconnector
{

class RestZeqTranslator
{
public:
    RestZeqTranslator();
    ~RestZeqTranslator();
    ::zeq::Event translate();
    bool findCommand( const std::string& data );

private:
    bool checkCommandValidity_( const std::string& command );
    std::string command_;
};

}

#endif // _RestZeqTranslator_h_

