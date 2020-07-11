
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
project(MeshAnimator VERSION 1.0.0 LANGUAGES CXX)

# Qt stuff
find_package(Radium REQUIRED Core Engine PluginBase)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(Qt5_LIBRARIES Qt5::Core Qt5::Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(ma_sources
	    src/MeshAnimatorPlugin.cpp
	    src/MeshAnimatorSystem.cpp
	    src/UI/MeshAnimatorUI.cpp
    )

set(ma_headers
	    src/MeshAnimatorPlugin.hpp
	    src/MeshAnimatorPluginMacros.hpp
	    src/MeshAnimatorSystem.hpp
	    src/UI/MeshAnimatorUI.h
    )

set(ma_markdowns
        README.md
    )

set(ma_uis
	    src/UI/MeshAnimatorUI.ui
    )
qt5_wrap_ui(ma_moc ${ma_uis})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR} # Moc
)

# Our library project uses these sources and headers.
add_library(
        ${PROJECT_NAME} SHARED
        ${ma_sources}
        ${ma_headers}
        ${ma_inlines}
        ${ma_markdowns}
        ${ma_uis}
        ${ma_resources}
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
        RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders
	"${ARGS}"
)
