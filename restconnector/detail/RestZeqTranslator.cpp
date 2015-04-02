/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include "RestZeqTranslator.h"
#include <zeq/hbp/vocabulary.h>

#include <boost/algorithm/string.hpp>
#include <lunchbox/log.h>

#include <iostream>

namespace restconnector
{
namespace detail
{

RestZeqTranslator::RestZeqTranslatorException::~RestZeqTranslatorException() {}

RestZeqTranslator::RestZeqTranslator()
{
}

RestZeqTranslator::~RestZeqTranslator()
{
}

zeq::Event RestZeqTranslator::translate ( const std::string& request, const std::string& body ) const
{
    const std::string& command = getCommand_( request );

    VocabularyMap::const_iterator it = vocabularySubscribed_.find( command );
    if( it == vocabularySubscribed_.end() )
        LBTHROW(CommandNotFound( command ) );

    return zeq::vocabulary::serializeJSON( it->second, body );
}

zeq::Event RestZeqTranslator::translate( const std::string& request ) const
{
    const std::string& command = getCommand_( request );

    VocabularyMap::const_iterator it = vocabularyPublished_.find( command );
    if( it == vocabularyPublished_.end() )
        LBTHROW(CommandNotFound( "Error: " + command ) );

    return zeq::vocabulary::serializeRequest( it->second );
}

std::string RestZeqTranslator::getCommand_( const std::string& request ) const
{
    if( request.empty() )
        LBTHROW(InvalidRequest("Empty request") );

    lunchbox::Strings dataParts;

    boost::split( dataParts, request, boost::is_any_of( "/" ) );
    lunchbox::Strings commandAndPayload;
    boost::split( commandAndPayload, dataParts.back(), boost::is_any_of( "?" ) );

    if( ( commandAndPayload.empty() ) || commandAndPayload.front().empty() )
        LBTHROW(InvalidRequest("Empty command string") );

    //We insure that REST command is lowercase because urls are not case sensitive
    std::transform( commandAndPayload.front().begin(), commandAndPayload.front().end(),
                    commandAndPayload.front().begin(), ::tolower );

    return commandAndPayload.front();
}

void RestZeqTranslator::addPublishedEvent( const std::string& restName,
                                           const lunchbox::uint128_t& event )
{
    //We insure that restName is lowercase because urls are not case sensitive
    std::string lowercaseRestName = restName;
    std::transform( lowercaseRestName.begin(), lowercaseRestName.end(),
                    lowercaseRestName.begin(), ::tolower );
    vocabularyPublished_[ lowercaseRestName ] = event;
}

void RestZeqTranslator::addSubscribedEvent( const std::string& restName,
                                            const lunchbox::uint128_t& event )
{
    //We insure that restName is lowercase because urls are not case sensitive
    std::string lowercaseRestName = restName;
    std::transform( lowercaseRestName.begin(), lowercaseRestName.end(),
                    lowercaseRestName.begin(), ::tolower );
    vocabularySubscribed_[ lowercaseRestName ] = event;
}

}
}
