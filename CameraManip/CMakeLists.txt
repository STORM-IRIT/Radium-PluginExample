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
project(CameraManip VERSION 1.0.0 LANGUAGES CXX)

# Qt stuff
find_package(Radium REQUIRED Core Engine PluginBase)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(Qt5_LIBRARIES Qt5::Core Qt5::Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(cm_sources
	src/CameraManipPlugin.cpp
	src/UI/CameraManipUI.cpp
        )

set(cm_headers
	src/CameraManipPlugin.hpp
	src/CameraManipPluginMacros.hpp
	src/UI/CameraManipUI.h
        )

set(cm_markdowns
        README.md
        )

set(cm_uis
	src/UI/CameraManipUI.ui
        )
qt5_wrap_ui(cm_moc ${cm_uis})


set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Our library project uses these sources and headers.
add_library(
        ${PROJECT_NAME} SHARED
        ${cm_sources}
        ${cm_headers}
        ${cm_inlines}
        ${cm_markdowns}
        ${cm_uis}
        ${cm_resources}
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
