
set (PLUGIN_NAME "ShaderFakeToon")

# Qt stuff
find_package( Qt5Core    REQUIRED )
add_definitions( ${Qt5Core_DEFINITIONS} )
set(Qt5_LIBRARIES ${Qt5Core_LIBRARIES})

file( GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )
file( GLOB_RECURSE headers ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp )
file( GLOB_RECURSE inlines ${CMAKE_CURRENT_SOURCE_DIR}/*.inl )
file( GLOB_RECURSE shaders ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*_SHADER* )

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
    ${shaders})

set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/")
if(MSVC OR MSVC_IDE OR MINGW)
    add_custom_target(copyShaders
       #${CMAKE_COMMAND} -E make_directory ${RADIUM_DOC_DIRECTORY}
       COMMAND ${CMAKE_COMMAND} -E copy_directory
         ${SHADER_DIR}
         ${RADIUM_PLUGIN_OUTPUT_PATH}/FakeToon/Shaders
       DEPENDS
         ${SHADER_DIR}/FRAGMENT_SHADER.frag
         ${SHADER_DIR}/VERTEX_SHADER.vert
       WORKING_DIRECTORY ${RADIUM_BUNDLE_DIRECTORY}
       COMMENT "Copying shaders to ${RADIUM_PLUGIN_OUTPUT_PATH}/FakeToon/Shaders" VERBATIM
    )
else()
    add_custom_target(copyShaders
       ${CMAKE_COMMAND} -E make_directory ${RADIUM_PLUGIN_OUTPUT_PATH}/FakeToon
       COMMAND ${CMAKE_COMMAND} -E create_symlink
         ${SHADER_DIR}
         ${RADIUM_PLUGIN_OUTPUT_PATH}/FakeToon/Shaders
       WORKING_DIRECTORY ${RADIUM_BUNDLE_DIRECTORY}
       COMMENT "Symlink shaders to ${RADIUM_PLUGIN_OUTPUT_PATH}/FakeToon/Shaders" VERBATIM
    )
endif()


target_compile_definitions(${PLUGIN_NAME} PRIVATE "-D${PLUGIN_NAME}_EXPORTS")
add_dependencies(${PLUGIN_NAME} copyShaders)


target_link_libraries(
                   ${PLUGIN_NAME}
                   ${RADIUM_LIBRARIES}              # Radium libs
                   ${GLBINDING_LIBRARIES}           # Radium dep
                   ${Qt5_LIBRARIES}                 # the Qt beast
                   ${OPENMESH_LIBRARIES}            # OpenMesh
                )

