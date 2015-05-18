# Copyright (c) BBP/EPFL 2011-2015
#                        Grigori.Chevtchenko@epfl.ch
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

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(RESTBRIDGE_SOURCES RESTBridge.cpp)
set(RESTBRIDGE_LINK_LIBRARIES RESTBridge)

common_application(restBridge)
