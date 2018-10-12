
set (PLUGIN_NAME "RadianceScaling")

# Qt stuff
find_package( Qt5Core    REQUIRED )
add_definitions( ${Qt5Core_DEFINITIONS} )
set(Qt5_LIBRARIES ${Qt5Core_LIBRARIES})

file( GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
file( GLOB_RECURSE headers ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp )
file( GLOB_RECURSE inlines ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file( GLOB_RECURSE markdowns ${CMAKE_CURRENT_SOURCE_DIR}/*.md )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Include look up directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_BINARY_DIR} # MOC
    ${Qt5Core_INCLUDE_DIRS}
    ${RADIUM_INCLUDE_DIRS}
    )

# Our library project uses these sources and headers.
add_library(
    ${PLUGIN_NAME} SHARED
    ${sources}
    ${headers}
    ${inlines}
    ${markdowns})


target_compile_definitions(${PLUGIN_NAME} PRIVATE "-D${PLUGIN_NAME}_EXPORTS")


target_link_libraries(
                   ${PLUGIN_NAME}
                   ${RADIUM_LIBRARIES}              # Radium libs
                   ${GLBINDING_LIBRARIES}           # Radium dep
                   ${Qt5_LIBRARIES}                 # the Qt beast
                   ${OPENMESH_LIBRARIES}            # OpenMesh
                )

