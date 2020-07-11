cmake_minimum_required(VERSION 3.6)
#------------------------------------------------------------------------------
# Policies and global parameters for CMake
if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif ()
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
# Be nice to visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#------------------------------------------------------------------------------
# Project definition
project(LaplacianSmoothing VERSION 1.0.0 LANGUAGES CXX)

# Qt stuff
find_package(Radium REQUIRED Core Engine PluginBase)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(Qt5_LIBRARIES Qt5::Core Qt5::Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(lplsmt_sources
        src/plugin.cpp
        src/system.cpp
        src/widget.cpp
        )

set(lplsmt_headers
        src/plugin.hpp
        src/system.hpp
        src/widget.hpp
        )

set(lplsmt_markdowns
        README.md
        )

set(lplsmt_uis
        ui/widget.ui
        )
qt5_wrap_ui(lplsmt_moc ${lplsmt_uis})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR} # Moc
)

# Our library project uses these sources and headers.
add_library(
        ${PROJECT_NAME} SHARED
        ${lplsmt_sources}
        ${lplsmt_headers}
        ${lplsmt_inlines}
        ${lplsmt_markdowns}
        ${lplsmt_uis}
        ${lplsmt_resources}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${PROJECT_NAME} PRIVATE "-D${PROJECT_NAME}_EXPORTS")

target_link_libraries(${PROJECT_NAME} PUBLIC
        Radium::Core
        Radium::Engine
        Radium::PluginBase
        ${Qt5_LIBRARIES}
        )

set(ARGS "")
if(INSTALL_IN_RADIUM_BUNDLE)
    set(ARGS "INSTALL_IN_RADIUM_BUNDLE")
endif(INSTALL_IN_RADIUM_BUNDLE)

configure_radium_plugin(
        NAME ${PROJECT_NAME}
	"${ARGS}"
)
