# Radium-PluginExample
Example of plugins for [Radium Engine](https://github.com/AGGA-IRIT/Radium-Engine), using an external repository to keep your code private (when you need it) and independent from the main repository.
This documentation is designed as a tutorial, introducing minimal requirements for Radium Engine plugins development.

Two exemples of plugins are provided in this project:
 - LaplacianSmoothing: illustrates how to process a mesh using OpenMesh and Radium (using `Ra::Core::TopologicalMesh`) The mesh is selected by the user using a combobox added into the plugins UI area in Radium Main-App.
 - FakeToonMaterial: illustrates how to add a new material in Radium. All the models loaded in MainApplication when this plugin is enabled are assigned with a toon shader.

## Installation
### Requirements
We assume that Radium Engine is in your file system, properly compiled, and reachable by using `FindRadium.cmake`.

If you are not a Radium Engine expert, we recommend to use the following folder structure:

```
    devel/
    ├── Radium-Engine/
    └── Radium-AppExample/
```

Note:
> As in the [Main Application example](https://github.com/STORM-IRIT/Radium-AppExample), Radium can also be set as a submodule.

### Compilation

1. Compile Radium Engine in `Release` mode
2. Compile Plugin
  - From the folder `Radium-AppExample:`, run `mkdir build-rel && cd build-rel`
  - Compile with `cmake -DCMAKE_BUILD_TYPE=Release .. && make -j 4`
3. Go to Radium Engine binaries with `cd ../../Radium-Engine/Release/bin`, which should contain the plugin binary.
4. Run Radium

## Code structure
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

