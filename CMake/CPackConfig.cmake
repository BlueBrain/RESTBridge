# Copyright (c) BBP/EPFL 2011-2014
#               Stefan.Eilemann@epfl.ch
# All rights reserved. Do not distribute without further notice.

# General CPack configuration
# Info: http://www.itk.org/Wiki/CMake:Component_Install_With_CPack

set(CPACK_PACKAGE_CONTACT "Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_LICENSE "Proprietary")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-test-dev")

include(CommonCPack)
include(GNUModules)
