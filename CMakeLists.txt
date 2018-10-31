cmake_minimum_required(VERSION 2.8.11)
project(Radium-PluginExamples)


if (APPLE)
    cmake_policy(SET CMP0042 NEW)
endif(APPLE)

# Radium Stuff
find_package( Radium REQUIRED)

# use Radium's common compilation options
list(APPEND CMAKE_MODULE_PATH ${RADIUM_ROOT_DIR}/cmake)
# perhaps not a good idea of including all the stuff from CompileFlags
# FIXME : get the correct flags (openmp, float precision, ... instead of the default
include (CompileFlags)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RADIUM_PLUGIN_OUTPUT_PATH} )
message( "${CMAKE_PROJECT_NAME} output path : ${RADIUM_PLUGIN_OUTPUT_PATH}" )

if ( NOT CMAKE_PREFIX_PATH )
  set( CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
endif()


# Win32 stuff
if (MSVC OR MSVC_IDE)
    # Copy libs / targets in the correct directories
    if ("${CMAKE_GENERATOR}" STREQUAL "NMake Makefiles")
        set(PDB_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    else()
        foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
            set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
            set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
            set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
        endforeach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    endif()
endif(MSVC OR MSVC_IDE)


add_subdirectory( laplacian-smoothing )
add_subdirectory( fake-toon )
