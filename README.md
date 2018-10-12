# Radium-PluginExample
Example of plugins for [Radium Engine](https://github.com/AGGA-IRIT/Radium-Engine), using an external repository to keep your code private (when you need it) and independent from the main repository.
This documentation is designed as a tutorial, introducing minimal requirements for Radium Engine plugins development.

Two exemples of plugins are provided in this project:
 - LaplacianSmoothing: which smooth a mesh in the scene using `Ra::Core::TopologicalMesh`. The mesh is selected by the user using a combobox added into the plugins UI area in Radium Main-App.
 - RadianceScalingMaterial: illustrate how to add a new material in Radium.

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
TODO
