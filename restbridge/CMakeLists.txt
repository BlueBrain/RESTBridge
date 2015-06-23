# Copyright (c) BBP/EPFL 2011-2014
#                        Grigori.Chevtchenko@epfl.ch
# All rights reserved. Do not distribute without further notice.

source_group(\\ FILES CMakeLists.txt)

set(RESTBRIDGE_PUBLIC_HEADERS
  RestBridge.h
  types.h)
set(RESTBRIDGE_HEADERS
  detail/RequestHandler.h
  detail/RestZeqTranslator.h)
set(RESTBRIDGE_SOURCES
  RestBridge.cpp
  detail/RequestHandler.cpp
  detail/RestZeqTranslator.cpp)

set(MINIMAL_BOOST_LIBRARIES ${Boost_LIBRARIES})
list(REMOVE_ITEM MINIMAL_BOOST_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})
set(RESTBRIDGE_LINK_LIBRARIES ${MINIMAL_BOOST_LIBRARIES}
  ${ZEQ_LIBRARIES} ${CPPNETLIB_LIBRARIES} pthread)
set(RESTBRIDGE_OMIT_LIBRARY_HEADER ON)

common_library(RESTBridge)
