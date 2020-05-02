cmake_minimum_required(VERSION 3.6)
#------------------------------------------------------------------------------
# Policies and global parameters for CMake
if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif ()
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
#------------------------------------------------------------------------------
# Project definition
project(ShaderFakeToon VERSION 1.0.0 LANGUAGES CXX)

# Qt stuff
find_package(Radium REQUIRED Core Engine PluginBase)
find_package(Qt5 COMPONENTS Core REQUIRED)
set(Qt5_LIBRARIES Qt5::Core)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(fktn_sources
        src/FakeToonMaterial.cpp
        src/plugin.cpp
        src/system.cpp
        )

set(fktn_headers
        src/FakeToonMaterial.hpp
        src/plugin.hpp
        src/system.hpp
        )

set(fktn_markdowns
        README.md
        )

set(fktn_resources
        Resources/Shaders
        )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Our library project uses these sources and headers.
add_library(
        ${PROJECT_NAME} SHARED
        ${fktn_sources}
        ${fktn_headers}
        ${fktn_inlines}
        ${fktn_markdowns}
        ${fktn_uis}
        ${fktn_resources}
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${PROJECT_NAME} PRIVATE "-D${PROJECT_NAME}_EXPORTS")

target_link_libraries(${PROJECT_NAME} PUBLIC
        Radium::Core
        Radium::Engine
        Radium::PluginBase
        ${Qt5_LIBRARIES}
        )

configure_radium_plugin(
    NAME ${PROJECT_NAME}
    RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders
)
