# Radium-PluginExample

Radium is a research 3D Engine for rendering, animation and processing.
It is developed and maintained by the [STORM research group](https://www.irit.fr/STORM/site/).

This repository holds examples of Plugins based on Radium Libraries.
To compile theses examples, you first need to compile the 
[Radium libraries](https://github.com/STORM-IRIT/Radium-Engine) and a plugin compatible application based on Radium, 
for instance [Main Application](https://github.com/STORM-IRIT/Radium-Apps).

Six examples of plugins are provided in this project:

-   LaplacianSmoothing: illustrates how to process a mesh using OpenMesh and Radium (using `Ra::Core::TopologicalMesh`). 

    The mesh is selected by the user using a combobox added into the plugins UI area in Radium Main-App.
    
-   FakeToonMaterial: illustrates how to add a new material in Radium. 

    All the models loaded in MainApplication when this plugin is enabled are assigned with a toon shader.
    
-   CameraManip: illustrates how to create cameras in Radium and switch from one to another.
-   Dummy: illustrates how to create a new system to add components to entities at model load, and tasks to the engine 
at each frame.
-   MeshFeatureTracking: illustrates how to pick mesh features (vertex, edge, triangle) and display data about these, 
updated at each frame.
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

By default the install target installs the plugin into the directory `installed-<Compiler_id>` in the plugin buildtree. 
Optional cmake options `-DCMAKE_INSTALL_PREFIX=pathToInstallPlugins`  or `-DINSTALL_IN_RADIUM_BUNDLE=ON` can be 
provided to install the plugins and resources in specified directory or in the Radium-Engine installation directory. 
Plugins installed into the Radium-Engine installation directory are automatically loaded by compatible applications 
while other installation directory must explicitly be added to the plugin directories of the applications.

For the provided plugins, the cmake option `INSTALL_IN_RADIUM` allows to add the parameter `INSTALL_IN_RADIUM_BUNDLE`
to each plugin CMakeLists.txt so that the Example plugins will be installed into the Radium Bundle
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
