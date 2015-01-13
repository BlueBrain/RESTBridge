# Copyright (c) BBP/EPFL 2011-2014
#                        Grigori.Chevtchenko@epfl.ch
# All rights reserved. Do not distribute without further notice.

source_group(\\ FILES CMakeLists.txt)

set(RESTCONNECTOR_PUBLIC_HEADERS ${COMMON_INCLUDES} RestParser.h
                                                    RestZeqTranslator.h)
set(RESTCONNECTOR_HEADERS)
set(RESTCONNECTOR_SOURCES ${COMMON_SOURCES} main.cpp
                                            RestParser.cpp
                                            RestZeqTranslator.cpp)

set(RESTCONNECTOR_LINK_LIBRARIES ${Boost_LIBRARIES} ${LUNCHBOX_LIBRARIES} ${ZEQ_LIBRARIES} ${CPPNETLIB_LIBRARIES} pthread)

common_application(RestConnector)

