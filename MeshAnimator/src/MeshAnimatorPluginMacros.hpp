#ifndef MESHANIMATORPLUGINMACROS_HPP_
#define MESHANIMATORPLUGINMACROS_HPP_

#include <Core/CoreMacros.hpp>

/// Defines the correct macro to export dll symbols.
#if defined MeshAnimator_EXPORTS
#    define MESH_ANIMATOR_PLUGIN_API DLL_EXPORT
#else
#    define MESH_ANIMATOR_PLUGIN_API DLL_IMPORT
#endif

#endif // MESHANIMATORPLUGINMACROS_HPP_
