# Copyright (c) BBP/EPFL 2011-2014
#                        Grigori.Chevtchenko@epfl.ch
# All rights reserved. Do not distribute without further notice.

source_group(\\ FILES CMakeLists.txt)

set(RESTCONNECTOR_PUBLIC_HEADERS
  ${COMMON_INCLUDES}
  RestConnector.h
  types.h)
set(RESTCONNECTOR_HEADERS
  detail/RequestHandler.h
  detail/RestZeqTranslator.h)
set(RESTCONNECTOR_SOURCES
  ${COMMON_SOURCES}
  RestConnector.cpp
  detail/RequestHandler.cpp
  detail/RestZeqTranslator.cpp)

set(RESTCONNECTOR_LINK_LIBRARIES ${Boost_LIBRARIES} ${LUNCHBOX_LIBRARIES}
  ${ZEQ_LIBRARIES} ${CPPNETLIB_LIBRARIES} pthread)
set(RESTCONNECTOR_OMIT_PROJECT_HEADER ON)

common_library(RESTConnector)
