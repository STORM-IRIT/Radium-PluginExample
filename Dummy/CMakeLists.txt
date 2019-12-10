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
project(Dummy VERSION 1.0.0 LANGUAGES CXX)

# Qt stuff
find_package(Radium REQUIRED Core Engine PluginBase)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(Qt5_LIBRARIES Qt5::Core Qt5::Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(dm_sources
	src/DummyComponent.cpp
	src/DummySystem.cpp
	src/DummyPlugin.cpp
	src/DummyTask.cpp
        )

set(dm_headers
	src/DummyComponent.hpp
	src/DummySystem.hpp
	src/DummyPlugin.hpp
	src/DummyPluginMacros.hpp
	src/DummyTask.hpp
        )

set(dm_markdowns
        README.md
        )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Our library project uses these sources and headers.
add_library(
        ${PROJECT_NAME} SHARED
        ${dm_sources}
        ${dm_headers}
        ${dm_inlines}
        ${dm_markdowns}
        ${dm_uis}
        ${dm_resources}
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(${PROJECT_NAME} PRIVATE "-D${PROJECT_NAME}_EXPORTS")

target_link_libraries(${PROJECT_NAME} PUBLIC
        Radium::Core
        Radium::Engine
        Radium::PluginBase
        ${Qt5_LIBRARIES}
        )

configure_radium_plugin_install(
        NAME ${PROJECT_NAME}
        RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders
)
