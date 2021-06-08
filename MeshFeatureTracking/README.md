# Mesh Feature Tracking Radium plugin example

This plugin is composed by 6 elements:

-   MeshFeatureTrackingPlugin: Implements `Ra::Plugins::RadiumPluginInterface`, and defines which widget and system need to be used in this plugin.

-   MeshFeatureTrackingUI: Inherit `QWidget`, and defines the UI to be added to Radium Main application. In this plugin, it contains three `QGroupBox`es,
    one to display vertices data (index, position and normal), one to display edge data (vertices indices) and one to display triangle data (vertices indices).

-   MeshFeatureTrackingComponent: Implements `Ra::Engine::Component`. In this plugin, the component is part of the `SystemEntity` and does the following:
    -   it stores the selected feature data, if any.
    -   it displays a green sphere located at the selected feature, if any.
