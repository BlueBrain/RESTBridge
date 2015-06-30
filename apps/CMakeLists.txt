# Copyright (c) BBP/EPFL 2011-2015
#                        Grigori.Chevtchenko@epfl.ch
# All rights reserved. Do not distribute without further notice.

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(RESTBRIDGE_SOURCES RESTBridge.cpp)
set(RESTBRIDGE_LINK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY} RESTBridge)

common_application(restBridge)
