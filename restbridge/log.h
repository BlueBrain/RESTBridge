/* Copyright (c) 2013-2021, EPFL/Blue Brain Project
 *                         Cyrille Favreau <cyrille.favreau@epfl.ch>
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

#ifndef RESTBRIDGE_LOG_H
#define RESTBRIDGE_LOG_H

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
