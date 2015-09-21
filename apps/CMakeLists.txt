# Copyright (c) 2014-2015, Human Brain Project
#                          Grigori.Chevtchenko@epfl.ch
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
# All rights reserved. Do not distribute without further notice.

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(RESTBRIDGESERVER_SOURCES RESTBridge.cpp)
set(RESTBRIDGESERVER_LINK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY} RESTBridge)

common_application(restBridgeServer)
