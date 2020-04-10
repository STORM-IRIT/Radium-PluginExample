# Radium-PluginExample

Radium is a research 3D Engine for rendering, animation and processing.
It is developed and maintained by the [STORM research group](https://www.irit.fr/STORM/site/).

This repository holds examples of Plugins based on Radium Libraries.
To compile theses examples, you first need to compile the [Radium libraries](https://github.com/STORM-IRIT/Radium-Engine) and a plugin compatible application based on Radium, for instance [Main Application](https://github.com/STORM-IRIT/Radium-Apps).

Six examples of plugins are provided in this project:

-   LaplacianSmoothing: illustrates how to process a mesh using OpenMesh and Radium (using `Ra::Core::TopologicalMesh`). The mesh is selected by the user using a combobox added into the plugins UI area in Radium Main-App.
-   FakeToonMaterial: illustrates how to add a new material in Radium. All the models loaded in MainApplication when this plugin is enabled are assigned with a toon shader.
-   CameraManip: illustrates how to create cameras in Radium and switch from one to another.
-   Dummy: illustrates how to create a new system to add components to entities at model load, and tasks to the engine at each frame.
-   MeshFeatureTracking: illustrates how to pick mesh features (vertex, edge, triangle) and display data about these, updated at each frame.
-   MeshPaint: illustrates how to paint on a mesh, here painting the per-vertex color.

## Quick instruction to build and install the examples

```Shell
mkdir build-release && cd build-release
cmake ../ -DRadium_DIR=pathToRadiumInstallation/lib/cmake/Radium -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

When configuring the examples, the cmake command takes the following options :

-   `Radium_DIR` must be set to `pathToRadiumInstallation/lib/cmake/Radium`
-   `INSTALL_IN_RADIUM_BUNDLE`, Off by default.

By default the install target installs the plugin into the directory `installed-<Compiler_id>` in the plugin buildtree. Optional cmake options `-DCMAKE_INSTALL_PREFIX=pathToInstallPlugins`  or `-DINSTALL_IN_RADIUM_BUNDLE=ON` can be provided to install the plugins and resources in specified directory or in the Radium-Engine installation directory. Plugins installed into the Radium-Engine installation directory are automatically loaded by compatible applications while other installation directory must explicitly be added to the plugin directories of the applications.
More details [below](#configure-the-plugin)

## How to write your own plugin

Custom plugins can be easily add to Radium ecosystem.
In addition to the Radium and Ra::Plugins::RadiumPluginInterface API,
Radium provides a set of tools simplifying the deployment of plugins, which among
other things take care of the portability and relocatability of the
binaries.

To use these tools, you need to follow theses instructions to configure, conmpile and use your plugin.

### Setting a CMakeLists.txt to compile and install a plugin

To compile and use a plugin, either from the build tree
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

 # Use installed Radium environment
 find_package(Radium REQUIRED Core Engine PluginBase)

 # Find and configure Qt environment (Radium requires Qt >= 5.1)
 find_package(Qt5 COMPONENTS Core REQUIRED)
 set(Qt5_LIBRARIES Qt5::Core)
 set(CMAKE_AUTOMOC ON)
 set(CMAKE_AUTORCC ON)

 # list all the sources and headers of the Plugin
 set(sources /*... */)
 set(headers /*... */)

 set(CMAKE_INCLUDE_CURRENT_DIR ON)

 # A Plugin is a library that uses these sources and headers.
 add_library( ${PROJECT_NAME} SHARED ${sources} ${headers} )

 target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
 target_compile_definitions(${PROJECT_NAME} PRIVATE "-D${PROJECT_NAME}_EXPORTS")

 target_link_libraries(${PROJECT_NAME} PUBLIC
                       Radium::Core Radium::Engine Radium::PluginBase
                       ${Qt5_LIBRARIES} )

 # Configure the plugin deployement using the Radium configuration tool
 configure_radium_plugin_install( NAME ${PROJECT_NAME} )
```

Such a CMakeLists.txt is as ususal as others. The only command that is specific to Radium plugins is the last line
`configure_radium_plugin_install( NAME ${PROJECT_NAME} )` that aims at configuring the plugin installation along with 
its associated resources. 

The command `configure_radium_plugin_install`takes several parameters.
The full usage of this command is :

```cmake
configure_radium_plugin_install( 
    NAME nameOfTheTargetOtInstall      # mandatory
    RESOURCES ListOfResourcesDirectory # optional
    INSTALL_IN_RADIUM_BUNDLE           # optional
     )
```

-   `NAME nameOfTheTargetOtInstall`: this parameter, mandatory, will configure how to install the target
    `nameOfTheTargetOtInstall`. This target must correspond to a configured plugin target such the one obtained by
    `target_link_libraries(nameOfTheTargetOtInstall PUBLIC Radium::Core Radium::Engine Radium::PluginBase ${Qt5_LIBRARIES} )`

-   `RESOURCES ListOfResourcesDirectory` : this parameter, optional, will install several resources alongside the plugin 
    so that the Radium resource locator system will be able to find these resources. 
       Resources could be shader source files, images and data files or whatever document is needed for the plugin to be 
       operational.

-   `INSTALL_IN_RADIUM_BUNDLE` : this parameter, optional, will install the plugin and its associated resources directly 
    into the Radium Bundle installation directory. This will allow to have Plugins relocatable at the same time than 
    the Radium Bundle. If this parameter is given, it will superseed to install location configuration that uses
     the cmake standard `CMAKE_INSTALL_PREFIX`

Note that, as it is the case for applications, Plugins does not need to be installed to be used by any applications. 
Plugins could be loaded and used from their build-trees.

### Configure the plugin

Configuring the plugin, and mainly the installation of the plugin, depends on several use cases that might evolve 
along the life-time of plugin development.

There are three main uses cases we can identify in the life-time of a Plugin development :

1.  You are developing/testing your plugin. There is no need to install the plugin.
    Any plugin compatible application might find and use it from its build tree (e.g `build-release/Plugins`). 
    For this, you have to configure once your application by adding the build path of the plugin (`build-dir/Plugins`) 
    to the plugin search path of the application. Each time you will launch this application, it will search in all its 
    registered search paths for compatible plugins.
    The advantage of not installing plugins while developping/testing is a fast startup of the application 
    and efficient update of the Plugin resources as Resources directories are linked (symbolic links on systems that 
    support them) into the build tree.

2.  You have developed several plugins, and will only occasionally recompile them, for your own usage and you want your 
    plugin compatible applications to find and load them quickly.
    In this case, you might install your plugin in one unique location and tell the applications to load plugins from 
    there. To do so, use the option `-DCMAKE_INSTALL_PREFIX=thePathToTheInstalledPlugins` and do a `make install`
    You can then configure your applications so that they will search plugins from the `thePathToTheInstalledPlugins` directory.
    Each time the application will start, all the plugins from this `thePathToTheInstalledPlugins` will be 
    loaded, even those installed here after the configuration of the plugin location in the application.

3.  You want to distribute plugins binaries alongside the Radium bundle.
    In this case, you have to configure the plugin with the`INSTALL_IN_RADIUM_BUNDLE`option and do a `make install` to
    install the plugins in the Radium Bundle. Note that this requires write access to the Radium Bundle.

The use case 1 above is efficient when you develop/test plugins with resources. The use case 2 is efficient when you 
have mature plugins you want to stack together. The use case 3 is fine when you want to distribute radium as binaries.

Once you have choosen the adequate use case, you can configure the plugin using cmake.

As we disable "in source" build in the CMakeLists.txt example above, we need to create a build directory:

```Shell
mkdir build-release && cd build-release
```

With Radium-Engine installed in the directory `pathToRadiumInstallation`,
configure and compile the project with :

```Shell
cmake ../ -DRadium_DIR=pathToRadiumInstallation/lib/cmake/Radium
cmake --build .
```

If you just want to use the plugin directly from its build-tree, the resulting dynamic library will be found in the 
`build-release/Plugins/lib` directory and the associated resources will be linked (or copied on systems that do not 
support symbolic links) in the directory `build-release/Plugins/Resources`. In this case, note that the plugin from the 
buildtree could generally not be moved as it is linked to used libraries using relative paths.

If you install the plugin, by default, it will be installed in a directory
called `installed-<Compiler_id>` in the plugin buildtree. 
This installed plugin is then relocatable only on the system where the plugin was built.  
You can change this default behavior by setting `CMAKE_INSTALL_PREFIX` to any directory, e.g `pathToInstallPlugins`.
The dynamic library will then be copied into  `pathToInstallPlugins/lib` and the Resources will be copied into 
`pathToInstallPlugins/Resources`. Later, you can move `pathToInstallPlugins` wherever you need and the plugin will still 
be functional.

To install the plugin in `pathToRadiumInstallation/Plugins/lib/`, which is relocatable alongside the Radium bundle and 
automatically loaded by any plugin compatible applications (such as `main-app`), pass the parameter 
`INSTALL_IN_RADIUM_BUNDLE` to the `configure_radium_plugin_install` of each plugin's `CMakeLists.txt`. 

Remember that resources associated with the plugin using the option `RESOURCES ListOfResourcesDirectory` of the 
`configure_radium_plugin_install` command are _linked_ into the buildtree (on systems supporting symbolic links)
and _copied_ into the installed locations. 

### Using the Plugin

Any application that inherits from `Ra::GuiBase::BaseApplication` could use any Plugin developped using the
Radium Engine. 
As plugins are Qt objects that implement specific interface, it is also permitted to have any Qt
Application to use plugins. Meanwhile, the following documentation is related to the Radium BaseApplication 
derived applications.

If the plugin is installed in the Radium Bundle directory, it will be automatically loaded by any plugin-aware 
application. 
But, when developping a plugin, the installation in the Radium bundle directory is not always permitted nor efficient. 

It could then be useful to use the Plugin from its own installation directory or directly from its build-tree.
To do that, the application could register the plugin directory location by calling the 
`Ra::GuiBase::BaseApplication::addPluginDirectory( const std::string& pluginDir );` method that will record, in the 
QSettings file associated with the application, the directory in which the Radium Engine will look for plugins.
Once this directory is registered, any plugin that will be found in this directory will be loaded at each application 
startup if the build type of the plugin is compatible with the build type of the application (release, debug, ...). 
If the plugin location that is registered correspond to the build tree location of the plugin, there is no need to 
instal it.

Note that the main Radium demonstration application (`main-app`) offer a menu entry 
(`File/Plugins/Add plugin path)` that call the registration method once the user has selected a plugin location.

## Description of the example plugins

For the prodvided plugins, the cmake option `INSTALL_IN_RADIUM` allows to add the parameter `INSTALL_IN_RADIUM_BUNDLE`
to each plugin CMakeLists.txt so that the Example plugins will be isntalled into the Radium Bundle
(i.e. use `cmake -DINSTALL_IN_RADIUM=On`). Note that this option requires to have write access to the Radium Bundle 
installation directory.

### Laplacian Smoothing

This plugin is composed by 3 elements:

-   Plugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which widget and system need to be used in this plugin.

-   Widget: Inherit `QWidget`, and defines the UI to be added to Radium Main application. In this plugin, it contains a `QCombobox` listing the 3D models, and a process button to start computation.

-   System: Implements `Ra::Engine::System`. In this plugin, the system does the following:
    -   each time a model is loaded in Radium Main Application, the system registers the 3D models for further processing and update the UI accordingly
    -   when a processing request is emitted by the Widget, the System process the 3D model selected by the user.

### FakeToon Material

This plugin is composed by 3 elements:

-   Plugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which system needs to be used in this plugin. It also initialize OpenGL elements when the application starts,
-   Material: Implements `Ra::Engine::Material`, and defines the `FakeToon` `Ra::Engine::RenderTechnique` and OpenGL binding operations,
-   System: Implements `Ra::Engine::System`. In this plugin, the system set the `FakeToon` RenderTechnique to the `Ra::Engine::RenderObject` associated to any asset loaded.

### Camera Manip

This plugin is composed by 2 elements:

-   CameraManipPlugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which widget and system need to be used in this plugin.

-   CameraManipUI: Inherit `QWidget`, and defines the UI to be added to Radium Main application. In this plugin, it contains three `QPressButton`s,
    one to create a new camera object at the current camera position, one to make the selected camera the one used by the application, and one to save the selected camera to a file.

### Dummy

This plugin is composed by 4 elements:

-   DummyPlugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which widget and system need to be used in this plugin.

-   DummyComponent: Implements `Ra::Engine::Component`. In this plugin, the component is empty and serves no specific purpose.

-   DummyTask: Implements `Ra::Core::Task`.

-   DummySystem: Implements `Ra::Engine::System`. In this plugin, the system does the following:
    -   each time a model is loaded in Radium Main Application, the system adds a DummyComponent to the new entity.
    -   at each frame, the system adds two different tasks displaying info to the output.

### Mesh Feature Tracking

This plugin is composed by 6 elements:

-   MeshFeatureTrackingPlugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which widget and system need to be used in this plugin.

-   MeshFeatureTrackingUI: Inherit `QWidget`, and defines the UI to be added to Radium Main application. In this plugin, it contains three `QGroupBox`es,
    one to display vertices data (index, position and normal), one to display edge data (vertices indices) and one to display triangle data (vertices indices).

-   MeshFeatureTrackingComponent: Implements `Ra::Engine::Component`. In this plugin, the component is part of the `SystemEntity` and does the following:
    -   it stores the selected feature data, if any.
    -   it displays a green sphere located at the selected feature, if any.

### Mesh Paint

This plugin is composed by 4 elements:

-   MeshPaintPlugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which widget and system need to be used in this plugin.

-   MeshPaintUI: Inherit `QWidget`, and defines the UI to be added to Radium Main application. In this plugin, it contains one `QRadioButton` to activate/desactivate mesh painting,
    and two `QPushButton`s, one to choose the painted color, the other to save the painted colors into the per-vertex colors.

-   MeshPaintComponent: Implements `Ra::Engine::Component`. In this plugin, the component does the following:
    -   it stores the painted colors.
    -   it processes paint events.

-   MeshPaintSystem: Implements `Ra::Engine::System`. In this plugin, the system adds a `MeshPaintComponent` to each loaded model.
