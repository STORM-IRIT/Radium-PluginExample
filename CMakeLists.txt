
cmake_minimum_required(VERSION 3.6)
project(Radium-PluginsExample)

# set the installation directory for all the apps at once. This will make the same definition ignore on all the
# included applications
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/installed-${CMAKE_CXX_COMPILER_ID}" CACHE PATH
	"Install path prefix, prepended onto install directories." FORCE)
    message("Set install prefix to ${CMAKE_INSTALL_PREFIX}")
    set(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT False)
endif ()

option(INSTALL_IN_RADIUM_BUNDLE "Install plugins in Radium-Bundle dir" OFF)

add_subdirectory(fake-toon)
add_subdirectory(laplacian-smoothing)
add_subdirectory(CameraManip)
add_subdirectory(Dummy)
add_subdirectory(MeshFeatureTracking)
add_subdirectory(MeshPaint)
