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

#ifndef RESTBRIDGE_RESTZEQTRANSLATOR_H
#define RESTBRIDGE_RESTZEQTRANSLATOR_H

#include <zeq/event.h>

#include <map>

namespace restbridge
{
namespace detail
{

static const std::string INTERNAL_CMD_VOCABULARY = "vocabulary";

/**
 * The RestZeqTranslator class is responsible translating REST commands
 * into zeq events. The translation is based on registered vocabularies.
 */
class RestZeqTranslator
{
public:

    class RestZeqTranslatorException : public std::runtime_error
    {
    public:
        explicit RestZeqTranslatorException( const std::string& message )
            : std::runtime_error( message ) {}

        virtual ~RestZeqTranslatorException() = 0;
    };
    class CommandNotFound : public RestZeqTranslatorException
    {
    public:
        explicit CommandNotFound( const std::string& message )
            : RestZeqTranslatorException( message ) {}
    };
    class InvalidRequest : public RestZeqTranslatorException
    {
    public:
        explicit InvalidRequest( const std::string& message )
            : RestZeqTranslatorException( message ) {}
    };

public:

    RestZeqTranslator();
    ~RestZeqTranslator();

    /**
     * Translate REST command into a zeq event. If the command is not supported
     * throw an exeption.
     * @param request String containing the HTTP request
     * @throw InvalidRequest if HTTP request is not REST-compliant
     * @throw CommandNotFound if REST command is not supported
     * @return the generated zeq event
     */
    zeq::Event translate( const std::string& request ) const;

    /**
     * Translate REST command into a zeq event. If the command is not supported
     * throw an exeption.
     * @param request String containing the HTTP request
     * @param body The payload of the REST command
     * @throw InvalidRequest if HTTP request is not REST-compliant
     * @throw CommandNotFound if REST command is not supported
     * @return the generated zeq event
     */
    zeq::Event translate( const std::string& request, const std::string& body ) const;

    /**
     * Add an event of type PUBLISHER into RestZeqTranslator known events map.
     * @param eventDescpriptor The desciptor of the event (restName, eventType, eventSchema )
     */
    void addPublishedEvent( const zeq::EventDescriptor& eventDescriptor );

    /**
     * Add an event of type SUBSCRIBER into RestZeqTranslator known events map.
     * @param eventDescpriptor The desciptor of the event (restName, eventType, eventSchema )
     */
    void addSubscribedEvent( const zeq::EventDescriptor& eventDescriptor );

    /**
     * Retrieve the REST command from the http request.
     * @param request A string containing the http request
     * @return A string containing the REST command
     */
    std::string getCommand( const std::string& request ) const;

    /**
     * Return a string with the known vocabulary.
     * @return The string with the known vocabulary
     */
    std::string getVocabulary() const;

private:

    struct zeqEventDescriptor
    {
        zeqEventDescriptor() {}

        zeqEventDescriptor( zeq::uint128_t eventType, std::string eventSchema  )
            : eventType_( eventType )
            , eventSchema_( eventSchema )
        {}

        zeq::uint128_t eventType_;
        std::string eventSchema_;
    };

    typedef std::map< std::string, zeqEventDescriptor > VocabularyMap;

    VocabularyMap vocabularyPublished_;
    VocabularyMap vocabularySubscribed_;

    std::string command_;

};

}
}

#endif // RESTBRIDGE_RESTZEQTRANSLATOR_H
