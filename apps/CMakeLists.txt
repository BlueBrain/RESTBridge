# Copyright (c) BBP/EPFL 2011-2015
#                        Grigori.Chevtchenko@epfl.ch
# All rights reserved. Do not distribute without further notice.

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(RESTCONNECTOR_SOURCES RESTConnector.cpp)
set(RESTCONNECTOR_LINK_LIBRARIES RESTConnector)

common_application(restConnector)
