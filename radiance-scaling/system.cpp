#include <system.hpp>

#include <Core/Log/Log.hpp>

namespace MyPluginExample
{

    MySystem::MySystem()
        : Ra::Engine::System()
    {
        LOG(logINFO) << "Radiance Scaling Plugin System created.";
    }

    MySystem::~MySystem()
    {
        LOG(logINFO) << "Radiance Scaling Plugin System destroyed.";
    }

    void MySystem::handleAssetLoading( Ra::Engine::Entity* /*entity*/,
                                       const Ra::Asset::FileData* /*fileData*/ )
    {
    }

    void MySystem::generateTasks( Ra::Core::TaskQueue* /*taskQueue*/,
                                  const Ra::Engine::FrameInfo& /*frameInfo*/ )
    {
    }

} // namespace MyPlugin
