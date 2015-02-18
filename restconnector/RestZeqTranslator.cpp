/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include <restconnector/RestZeqTranslator.h>
#include <zeq/hbp/vocabulary.h>

#include <boost/algorithm/string.hpp>
#include <lunchbox/log.h>

#include <iostream>

namespace restconnector
{

RestZeqTranslator::RestZeqTranslator()
{
}

RestZeqTranslator::~RestZeqTranslator()
{
}

::zeq::Event RestZeqTranslator::translate( const std::string& body ) const
{
    //Here we should use the rest2Zeq mapper that need to be created in zeq::vocabulary
    if( command_ == "setCamera" )
        // This must be in the vocabulary
        // The vacabulary is responsible for returning the
        // zeq event
        return ::zeq::vocabulary::serializeJSON( ::zeq::hbp::EVENT_CAMERA, body );
    else if( command_ == "request" )
        //  This must be in the vocabulary
        return ::zeq::hbp::serializeRequest( ::zeq::hbp::EVENT_IMAGEJPEG );
    return ::zeq::vocabulary::serializeEcho("Unknown REST command");
}

bool RestZeqTranslator::isCommandSupported( const std::string& request )
{
    lunchbox::Strings dataParts;

    if( request.empty() )
    {
        LBINFO << "Error: The request string is empty." << std::endl;
        return false;
    }
    else
        boost::split( dataParts, request, boost::is_any_of( "/" ) );

    std::vector<std::string> commandAndPayload;
    boost::split( commandAndPayload, dataParts.back(), boost::is_any_of( "?" ) );

    if( commandAndPayload.empty() )
        return false;

    if( commandAndPayload[0] == "")
        return false;

    command_ = commandAndPayload[0];
    return checkCommandValidity_( command_ );
}

bool RestZeqTranslator::checkCommandValidity_( const std::string& command ) const
{
    //Here we should use the rest2Zeq mapper that need to be created in zeq::vocabulary
    if( command == "setCamera" )
        return true;
    else if( command == "request" )
        return true;

    LBINFO << "Unknown REST command " << command << std::endl;
    return false;
}

}
