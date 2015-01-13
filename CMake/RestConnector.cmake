
set(RESTCONNECTOR_VERSION 0.1)
set(RESTCONNECTOR_REPO_URL ssh://bbpcode.epfl.ch/viz/RestConnector)
set(RESTCONNECTOR_DEPENDS REQUIRED Boost cppnetlib Lunchbox zeq
                          OPTIONAL BBPDocumentation)
set(RESTCONNECTOR_BOOST_COMPONENTS "unit_test_framework program_options system thread")
set(RESTCONNECTOR_DEB_DEPENDS libboost-test-dev libboost-program-options-dev)

if(CI_BUILD_COMMIT)
  set(RESTCONNECTOR_REPO_TAG ${CI_BUILD_COMMIT})
else()
  set(RESTCONNECTOR_REPO_TAG master)
endif()
set(RESTCONNECTOR_FORCE_BUILD ON)
set(RESTCONNECTOR_SOURCE ${CMAKE_SOURCE_DIR})