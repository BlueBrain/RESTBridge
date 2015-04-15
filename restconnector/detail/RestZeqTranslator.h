/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef RESTCONNECTOR_RESTZEQTRANSLATOR_H
#define RESTCONNECTOR_RESTZEQTRANSLATOR_H

#include <zeq/event.h>

#include <map>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// CAUTION FOR REVIEWERS: This class is under development. Expect serious
// changes in the next review.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

namespace restconnector
{
namespace detail
{

typedef std::map< std::string, lunchbox::uint128_t > VocabularyMap;

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
     * @param restName The string used for REST command
     * @param event The zeq event's uint128
     */
    void addPublishedEvent( const std::string& restName, const lunchbox::uint128_t& event );

    /**
     * Add an event of type SUBSCRIBER into RestZeqTranslator known events map.
     * @param restName The string used for REST command
     * @param event The zeq event's uint128
     */
    void addSubscribedEvent( const std::string& restName, const lunchbox::uint128_t& event );

private:

    VocabularyMap vocabularyPublished_;
    VocabularyMap vocabularySubscribed_;

    std::string getCommand_( const std::string& request ) const;
    std::string command_;

};

}
}

#endif // RESTCONNECTOR_RESTZEQTRANSLATOR_H
