# Copyright (c) BBP/EPFL 2011-2014
#                        Grigori.Chevtchenko@epfl.ch
# All rights reserved. Do not distribute without further notice.

source_group(\\ FILES CMakeLists.txt)

set(RESTBRIDGE_PUBLIC_HEADERS
  ${COMMON_INCLUDES}
  RestBridge.h
  types.h)
set(RESTBRIDGE_HEADERS
  detail/RequestHandler.h
  detail/RestZeqTranslator.h)
set(RESTBRIDGE_SOURCES
  ${COMMON_SOURCES}
  RestBridge.cpp
  detail/RequestHandler.cpp
  detail/RestZeqTranslator.cpp)

set(RESTBRIDGE_LINK_LIBRARIES ${Boost_LIBRARIES} ${LUNCHBOX_LIBRARIES}
  ${ZEQ_LIBRARIES} ${CPPNETLIB_LIBRARIES} pthread)

common_library(RESTBridge)
