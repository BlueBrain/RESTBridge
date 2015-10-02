# Copyright (c) 2014-2015, Human Brain Project
#                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
#
# This file is part of RESTBridge <https://github.com/BlueBrain/RESTBridge>
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License version 3.0 as published
# by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

source_group(\\ FILES CMakeLists.txt)

set(RESTBRIDGE_PUBLIC_HEADERS
  RestBridge.h
  types.h)
set(RESTBRIDGE_HEADERS
  detail/RequestHandler.h
  detail/RestZeqTranslator.h)
set(RESTBRIDGE_SOURCES
  RestBridge.cpp
  detail/RequestHandler.cpp
  detail/RestZeqTranslator.cpp)

set(RESTBRIDGE_LINK_LIBRARIES PUBLIC zeq
  PRIVATE cppnetlib-client-connections ${Boost_LIBRARIES} pthread)
set(RESTBRIDGE_OMIT_LIBRARY_HEADER ON)

common_library(RESTBridge)
