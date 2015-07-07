
set(RESTBRIDGE_VERSION 0.1)
set(RESTBRIDGE_REPO_URL https://github.com/BlueBrain/RESTBridge.git)
set(RESTBRIDGE_DEPENDS REQUIRED Boost cppnetlib Servus zeq FlatBuffers
                          OPTIONAL bluebrain)
set(RESTBRIDGE_BOOST_COMPONENTS "unit_test_framework program_options system thread")
set(RESTBRIDGE_DEB_DEPENDS libboost-test-dev libboost-program-options-dev libboost-system-dev libboost-thread-dev)

if(CI_BUILD_COMMIT)
  set(RESTBRIDGE_REPO_TAG ${CI_BUILD_COMMIT})
else()
  set(RESTBRIDGE_REPO_TAG master)
endif()
set(RESTBRIDGE_FORCE_BUILD ON)
set(RESTBRIDGE_SOURCE ${CMAKE_SOURCE_DIR})
