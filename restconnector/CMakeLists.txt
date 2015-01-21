# Copyright (c) BBP/EPFL 2011-2014
#                        Grigori.Chevtchenko@epfl.ch
# All rights reserved. Do not distribute without further notice.

source_group(\\ FILES CMakeLists.txt)

set(RESTCONNECTOR_PUBLIC_HEADERS ${COMMON_INCLUDES} RequestHandler.h
                                                    RestConnector.h
                                                    RestParser.h
                                                    RestZeqTranslator.h
                                                    types.h
                                                    ZeqEventTranslator.h
                                                    Lib/zeq/events.h)
set(RESTCONNECTOR_HEADERS)
set(RESTCONNECTOR_SOURCES ${COMMON_SOURCES} main.cpp
                                            RestConnector.cpp
                                            RequestHandler.cpp
                                            RestParser.cpp
                                            ZeqEventTranslator.cpp
                                            Lib/zeq/events.cpp)

flatbuffers_generate_c_headers(FBS Lib/zeq/response.fbs
                                   Lib/zeq/makeCurrent.fbs)

list(APPEND RESTCONNECTOR_HEADERS ${FBS_OUTPUTS})

set(RESTCONNECTOR_LINK_LIBRARIES ${Boost_LIBRARIES} ${LUNCHBOX_LIBRARIES} ${ZEQ_LIBRARIES} ${CPPNETLIB_LIBRARIES} pthread)

common_library(RestConnector)
#common_application(RestConnector)
