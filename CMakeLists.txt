# Copyright (c) 2014-2015, Human Brain Project
#                          Stefan Eilemann <Stefan.Eilemann@epfl.ch>
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

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(RESTBridge)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)
include(GitExternal)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "2")
set(VERSION_PATCH "0")
set(VERSION_ABI 1)

set(RESTBRIDGE_DESCRIPTION "A library that provides a simple way to expose a 
                            REST interface in a C++ application")
set(RESTBRIDGE_ISSUES_URL "https://github.com/BlueBrain/RESTBridge/issues")
set(RESTBRIDGE_MAINTAINER
  "BBP Visualization Team <bbp-open-source@googlegroups.com>")
set(RESTBRIDGE_LICENSE LGPL)

include(Common)

set(COMMON_PROJECT_DOMAIN ch.epfl.bluebrain)
option(CPP-NETLIB_BUILD_SHARED_LIBS "Build cpp-netlib as shared libraries." ON)
option(CPP-NETLIB_BUILD_EXAMPLES "Build the cpp-netlib project examples." OFF)

if(APPLE)
  # WAR otherwise system OpenSSL is preferred
  list(INSERT CMAKE_PREFIX_PATH 0 /opt/local /usr/local)
endif()

common_package(Boost REQUIRED COMPONENTS unit_test_framework system thread)
common_package(cppnetlib SYSTEM REQUIRED)
common_package(FlatBuffers REQUIRED)
common_package(ZeroEQ REQUIRED)
common_package_post()

add_subdirectory(restbridge)
add_subdirectory(apps)
add_subdirectory(tests)

include(CPackConfig)
include(DoxygenRule)

SET(DOXYGEN_MAINPAGE_MD README.md)
SET(DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md)
