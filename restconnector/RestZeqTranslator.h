/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef _RestZeqTranslator_h_
#define _RestZeqTranslator_h_

#include <zeq/event.h>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// CAUTION FOR REVIEWERS: This class is under development. Expect serious
// changes in the next review.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

namespace restconnector
{

/**
 * The RestZeqTranslator class is responsible translating REST commands
 * into zeq events. The translation is based on registered vocabularies.
 */
class RestZeqTranslator
{

public:

    RestZeqTranslator();
    ~RestZeqTranslator();

    /**
     * Translates a REST command into a zeq event.
     * @param body JSON formatted body of the REST request
     */
    ::zeq::Event translate( const std::string& body ) const;

    /**
     * Checks if the REST command is supported by the registered vocabularies.
     * @param request String containing the HTTP request
     */
    bool isCommandSupported( const std::string& request );

private:

    bool checkCommandValidity_( const std::string& command ) const;
    std::string command_;

};

}

#endif // _RestZeqTranslator_h_