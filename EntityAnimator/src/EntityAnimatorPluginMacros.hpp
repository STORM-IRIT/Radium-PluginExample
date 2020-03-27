#ifndef ENTITYANIMATORPLUGINMACROS_HPP
#define ENTITYANIMATORPLUGINMACROS_HPP

#include <Core/CoreMacros.hpp>

/// Defines the correct macro to export dll symbols.
#if defined EntityAnimator_EXPORTS
#    define ENTITY_ANIMATOR_PLUGIN_API DLL_EXPORT
#else
#    define ENTITY_ANIMATOR_PLUGIN_API DLL_IMPORT
#endif

#endif // ENTITYANIMATORPLUGINMACROS_HPP
