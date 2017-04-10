cmake_minimum_required(VERSION 3.0)
project(Radium-PluginExample)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package( Qt5Core    REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Radium REQUIRED)


# use Radium's common compilation options
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${RADIUM_ROOT_DIR}/cmake)
include (CompileFlags)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RADIUM_PLUGIN_OUTPUT_PATH} )

if ( NOT CMAKE_PREFIX_PATH )
  set( CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
endif()

# Win32 stuff
if (MSVC OR MSVC_IDE)
    # Copy libs / targets in the correct directories
    if ("${CMAKE_GENERATOR}" STREQUAL "NMake Makefiles")
        set(PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    else()
        foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
            set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RADIUM_PLUGIN_OUTPUT_PATH})
            set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RADIUM_PLUGIN_OUTPUT_PATH})
            set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RADIUM_PLUGIN_OUTPUT_PATH})
        endforeach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    endif()
endif(MSVC OR MSVC_IDE)


file( GLOB_RECURSE sources src/*.cpp )
file( GLOB_RECURSE headers src/*.hpp )
file( GLOB_RECURSE inlines src/*.inl )
file( GLOB_RECURSE markdowns *.md src/*.md )

file(GLOB_RECURSE app_uis  src/*.ui)
qt5_wrap_ui(app_uis_moc ${app_uis})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_definitions( ${Qt5Core_DEFINITIONS} )
add_definitions( ${Qt5Widgets_DEFINITIONS} )

# Include look up directories
include_directories(
    src/
    ${CMAKE_CURRENT_BINARY_DIR} # MOC
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${RADIUM_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIR}
    )

# Our library project uses these sources and headers.
add_library(
    PluginExample MODULE
    ${sources}
    ${headers}
    ${inlines}
    ${markdowns}
    ${app_uis})

target_compile_definitions(PluginExample PRIVATE "-DPluginExample_EXPORTS")

target_link_libraries(
                   PluginExample
                   ${RADIUM_LIBRARIES}              # Radium libs
                   ${GLBINDING_LIBRARIES}           # Radium dep
                   ${Qt5_LIBRARIES}                 # the Qt beast
                )

