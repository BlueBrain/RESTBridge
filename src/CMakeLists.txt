# Copyright (c) BBP/EPFL 2011-2014
#                        Stefan.Eilemann@epfl.ch
# All rights reserved. Do not distribute without further notice.

source_group(\\ FILES CMakeLists.txt)

set(VWSSERVER_PUBLIC_HEADERS ${COMMON_INCLUDES} RestParser.h
                                                RestZeqTranslator.h)
set(VWSSERVER_HEADERS)
set(VWSSERVER_SOURCES ${COMMON_SOURCES} main.cpp
                                        RestParser.cpp
                                        RestZeqTranslator.cpp)
set(VWSSERVER_LINK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})

common_library(VWSServer)
