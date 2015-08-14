# Copyright (c) HBP 2015 Grigori.Chevtchenko@epfl.ch
# Change this number when adding tests to force a CMake run: 4

set(TEST_LIBRARIES RESTBridge zeqHBP ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
include(CommonCTest)
