#ifndef MESHANIMATORPLUGIN_MESHANIMATOR_SYSTEM_HPP_
#define MESHANIMATORPLUGIN_MESHANIMATOR_SYSTEM_HPP_

#include <MeshAnimatorPluginMacros.hpp>

#include <Engine/System/TimedSystem.hpp>

#include <UI/MeshAnimatorUI.h>

namespace MeshAnimatorPlugin {
class MeshAnimatorPluginC;

class MESH_ANIMATOR_PLUGIN_API MeshAnimatorSystem : public Ra::Engine::System
{
  public:
    MeshAnimatorSystem( MeshAnimatorPluginC* plugin );

    void generateTasks( Ra::Core::TaskQueue* taskQueue,
                        const Ra::Engine::FrameInfo& frameInfo ) override;

    void handleAssetLoading( Ra::Engine::Entity* entity,
                             const Ra::Core::Asset::FileData* fileData ) override;

  private:
    MeshAnimatorPluginC* m_plugin;
};

} // namespace MeshAnimatorPlugin

#endif // MESHANIMATORPLUGIN_MESHANIMATOR_SYSTEM_HPP_
