#ifndef RESTBRIDGE_LOG_H
#define RESTBRIDGE_LOG_H

/* Copyright (c) 2014-2015, Human Brain Project
 *                          Cyrille Favreau <cyrille.favreau@epfl.ch>
 */

#include <iostream>

#ifdef NDEBUG
#  define RBINFO if( false ) std::cout
#  define RBDEBUG if( false ) std::cout
#else
#  define RBINFO std::cout
#  define RBDEBUG std::cout
#endif
#define RBERROR std::cerr
#define RBWARN std::cerr

#define RBTHROW(e)\
    {\
        RBERROR << e.what() << std::endl;\
        throw e;\
    }

#endif // RESTBRIDGE_LOG_H
