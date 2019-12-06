# Radium-PluginExample
Radium is a research 3D Engine for rendering, animation and processing.
It is developed and maintained by the [STORM research group](https://www.irit.fr/STORM/site/).

This repository holds examples of Plugins based on Radium Libraries.
To compile this repository, you first need to compile the [Radium libraries](https://github.com/STORM-IRIT/Radium-Engine) and an application based Radium, for instance[Main Application](https://github.com/STORM-IRIT/Radium-Apps).

Two examples of plugins are provided in this project:
 - LaplacianSmoothing: illustrates how to process a mesh using OpenMesh and Radium (using `Ra::Core::TopologicalMesh`) The mesh is selected by the user using a combobox added into the plugins UI area in Radium Main-App.
 - FakeToonMaterial: illustrates how to add a new material in Radium. All the models loaded in MainApplication when this plugin is enabled are assigned with a toon shader.

## How to write your own plugin
Custom plugins can be easily add to Radium ecosystem.
Radium provides a set of tools simplifying the deployment plugins, which among
other things take care of the portability and relocatability of the
binaries.

To use these tools, you need to follow theses instructions.
### Setting a CMakeLists.txt to compile and install a plugin
To compile and execute an application, either in the build tree
or after installation, the Radium environment must be configured,
compiled and installed beforehand.

The main CMakeLists.txt file for compiling a Radium plugin
could be as simple as
 ```cmake
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
  project(MyPlugin VERSION 1.0.0 LANGUAGES CXX)

  # Qt stuff
  find_package(Radium REQUIRED Core Engine PluginBase)
  find_package(Qt5 COMPONENTS Core REQUIRED)
  set(Qt5_LIBRARIES Qt5::Core)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)

  set(sources /*... */)
  set(headers /*... */)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  # Our library project uses these sources and headers.
  add_library( ${PROJECT_NAME} SHARED ${sources} ${headers} )

  target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_compile_definitions(${PROJECT_NAME} PRIVATE "-D${PROJECT_NAME}_EXPORTS")

  target_link_libraries(${PROJECT_NAME} PUBLIC
                        Radium::Core Radium::Engine Radium::PluginBase
                        ${Qt5_LIBRARIES} )

  configure_radium_plugin_install( NAME ${PROJECT_NAME} )
```
### Configure the plugin
A Radium Plugin shouldn't be compiled in its source tree.
You then must first create a build directory:

     mkdir build && cd build

If the Radium environment was installed in the directory `pathToRadiumInstallation`,
configure the project with :

     cmake -DRadium_DIR=pathToRadiumInstallation/lib/cmake/Radium


 By default, the plugin will be installed in a directory
 called `installed-<Compiler_id>` in the application buildtree. This installed
 plugin is then fully relocatable and distribuable.
 You can change this default behavior by setting `CMAKE_INSTALL_PREFIX`. To generate
 the application in `pathToRadiumInstallation/Plugins/lib/`, which is
 automatically loaded by `main-app`, set
 `-DCMAKE_INSTALL_PREFIX=pathToRadiumInstallation/Plugins`.


## Description of the example plugins
### Laplacian Smoothing
This plugin is composed by 3 elements:
* Plugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which widget and system need to be used in this plugin.
* Widget: Inherit `QWidget`, and defines the UI to be added to Radium Main application. In this plugin, it contains a `QCombobox` listing the 3D models, and a process button to start computation.
* System: Implements `Ra::Engine::System`. In this plugin, the system does the following:
  * each time a model is loaded in Radium Main Application, the system registers the 3D models for further processing and update the UI accordingly
  * when a processing request is emitted by the Widget, the System process the 3D model selected by the user.

### FakeToon Material
This plugin is composed by 3 elements:
* Plugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which system needs to be used in this plugin. It also initialize OpenGL elements when the application starts,
* Material: Implements `Ra::Engine::Material`, and defines the `FakeToon` `Ra::Engine::RenderTechnique` and OpenGL binding operations,
* System: Implements `Ra::Engine::System`. In this plugin, the system set the `FakeToon` RenderTechnique to the `Ra::Engine::RenderObject` associated to any asset loaded.
