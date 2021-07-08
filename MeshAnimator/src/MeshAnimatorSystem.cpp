#include <MeshAnimatorSystem.hpp>

#include <MeshAnimatorPlugin.hpp>

namespace MeshAnimatorPlugin {

MeshAnimatorSystem::MeshAnimatorSystem( MeshAnimatorPluginC* plugin ) :
    Ra::Engine::System(),
    m_plugin( plugin ) {}

void MeshAnimatorSystem::generateTasks( Ra::Core::TaskQueue* taskQueue,
                                        const Ra::Engine::FrameInfo& frameInfo ) {
    if ( m_plugin ) { m_plugin->update(); }
}

void MeshAnimatorSystem::handleAssetLoading( Ra::Engine::Entity* entity,
                                             const Ra::Core::Asset::FileData* fileData ) {
    ;
}

} // namespace MeshAnimatorPlugin
