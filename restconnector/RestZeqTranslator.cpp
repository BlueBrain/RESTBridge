/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include <restconnector/RestZeqTranslator.h>
#include <zeq/hbp/vocabulary.h>

#include <boost/algorithm/string.hpp>

namespace restconnector
{

RestZeqTranslator::RestZeqTranslator(){}

RestZeqTranslator::~RestZeqTranslator(){}

::zeq::Event RestZeqTranslator::translate()
{
    //Here we should use the rest2Zeq mapper that need to be created in zeq::vocabulary
    lunchbox::uint128_t test;
    ::zeq::Event event = zeq::hbp::serializeRequest( test );
    return event;
}

bool RestZeqTranslator::findCommand( const std::string& data )
{
    std::vector<std::string> dataParts;

    if( !data.empty() )
    {
        boost::split( dataParts, data, boost::is_any_of( "/" ) );
    }
    else
    {
        std::cout << "Error: The request string is empty." << std::endl;
        return false;
    }

    std::vector<std::string> commandAndPayload;
    boost::split( commandAndPayload, dataParts.back(), boost::is_any_of( "?" ) );

    if( commandAndPayload.empty() )
        return false;

    if( commandAndPayload[0] == "")
        return false;

    command_ = commandAndPayload[0];
    return checkCommandValidity_( command_ );
}

bool RestZeqTranslator::checkCommandValidity_( const std::string& command )
{
    //Here we should use the rest2Zeq mapper that need to be created in zeq::vocabulary
    if( command == "setCamera" )
        return true;
    else if( command == "request" )
        return true;

    return false;
}

}
