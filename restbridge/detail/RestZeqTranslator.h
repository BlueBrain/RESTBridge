/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef RESTCONNECTOR_RESTZEQTRANSLATOR_H
#define RESTCONNECTOR_RESTZEQTRANSLATOR_H

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
        RestZeqTranslatorException( const std::string& message)
            : std::runtime_error( message ) {}

        virtual ~RestZeqTranslatorException() = 0;
    };
    class CommandNotFound : public RestZeqTranslatorException
    {
    public:
        CommandNotFound( const std::string& message)
            : RestZeqTranslatorException( message ) {}
    };
    class InvalidRequest : public RestZeqTranslatorException
    {
    public:
        InvalidRequest( const std::string& message)
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
     * Retreive the REST command from the http request.
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

        zeqEventDescriptor( lunchbox::uint128_t eventType, std::string eventSchema  )
            : eventType_( eventType )
            , eventSchema_( eventSchema )
        {}

        lunchbox::uint128_t eventType_;
        std::string eventSchema_;
    };

    typedef std::map< std::string, zeqEventDescriptor > VocabularyMap;

    VocabularyMap vocabularyPublished_;
    VocabularyMap vocabularySubscribed_;

    std::string command_;

};

}
}

#endif // RESTCONNECTOR_RESTZEQTRANSLATOR_H
