/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 *
 * This file is part of RESTBridge <https://github.com/BlueBrain/RESTBridge>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "RestZeqTranslator.h"
#include "restbridge/log.h"

#include <zeq/zeq.h>
#include <zeq/hbp/vocabulary.h>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>

static const std::string JSON_COMMAND = "command";
static const std::string JSON_SCHEMA = "schema";
static const std::string JSON_SUBSCRIBER = "subscribed";
static const std::string JSON_PUBLISHED = "published";

namespace restbridge
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

zeq::Event RestZeqTranslator::translate ( const std::string& request,
                                          const std::string& body ) const
{
    const std::string& command = getCommand( request );

    VocabularyMap::const_iterator it = vocabularySubscribed_.find( command );
    if( it == vocabularySubscribed_.end() )
        RBTHROW(CommandNotFound( command ) );

    return zeq::vocabulary::serializeJSON( it->second.eventType_, body );
}

zeq::Event RestZeqTranslator::translate( const std::string& request ) const
{
    const std::string& command = getCommand( request );

    VocabularyMap::const_iterator it = vocabularyPublished_.find( command );
    if( it == vocabularyPublished_.end() )
        RBTHROW(CommandNotFound( command ) );

    return zeq::vocabulary::serializeRequest( it->second.eventType_ );
}

std::string RestZeqTranslator::getCommand( const std::string& request ) const
{
    if( request.empty() )
        RBTHROW(InvalidRequest("Empty request") );

    servus::Strings dataParts;

    boost::split( dataParts, request, boost::is_any_of( "/" ) );
    servus::Strings commandAndPayload;
    boost::split( commandAndPayload, dataParts.back(), boost::is_any_of( "?" ) );

    if( ( commandAndPayload.empty() ) || commandAndPayload.front().empty() )
        RBTHROW(InvalidRequest("Empty command string") );

    //We insure that REST command is lowercase because urls are not case sensitive
    std::transform( commandAndPayload.front().begin(), commandAndPayload.front().end(),
                    commandAndPayload.front().begin(), ::tolower );

    return commandAndPayload.front();
}

void RestZeqTranslator::addPublishedEvent( const zeq::EventDescriptor& eventDescriptor )
{
    //We insure that restName is lowercase because urls are not case sensitive
    std::string lowercaseRestName = eventDescriptor.getRestName();
    std::transform( lowercaseRestName.begin(), lowercaseRestName.end(),
                    lowercaseRestName.begin(), ::tolower );
    vocabularyPublished_[ lowercaseRestName ] = zeqEventDescriptor( eventDescriptor.getEventType(),
                                                                    eventDescriptor.getSchema() );
}

void RestZeqTranslator::addSubscribedEvent( const zeq::EventDescriptor& eventDescriptor )
{
    //We insure that restName is lowercase because urls are not case sensitive
    std::string lowercaseRestName = eventDescriptor.getRestName();
    std::transform( lowercaseRestName.begin(), lowercaseRestName.end(),
                    lowercaseRestName.begin(), ::tolower );
    vocabularySubscribed_[ lowercaseRestName ] = zeqEventDescriptor( eventDescriptor.getEventType(),
                                                                     eventDescriptor.getSchema() );
}

std::string RestZeqTranslator::getVocabulary() const
{
    boost::property_tree::ptree vocabulary;
    boost::property_tree::ptree subscribedVocabulary;

    for ( VocabularyMap::const_iterator it = vocabularySubscribed_.begin();
          it != vocabularySubscribed_.end(); ++it )
    {
        boost::property_tree::ptree event;
        event.add( JSON_COMMAND, it->first );
        event.add( JSON_SCHEMA, it->second.eventSchema_ );
        subscribedVocabulary.add_child( it->second.eventType_.getString(),
                                        event );
    }

    vocabulary.add_child( JSON_SUBSCRIBER, subscribedVocabulary );
    boost::property_tree::ptree publishedVocabulary;

    for ( VocabularyMap::const_iterator it = vocabularyPublished_.begin();
          it != vocabularyPublished_.end(); ++it )
    {
        boost::property_tree::ptree event;
        event.add( JSON_COMMAND, it->first );
        event.add( JSON_SCHEMA, it->second.eventSchema_ );
        publishedVocabulary.add_child( it->second.eventType_.getString(),
                                       event );
    }

    vocabulary.add_child( JSON_PUBLISHED, publishedVocabulary );
    std::ostringstream json;
    boost::property_tree::write_json( json, vocabulary, false );
    return json.str();
}

}
}
