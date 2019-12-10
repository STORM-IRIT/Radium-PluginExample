#ifndef DUMMYPLUGIN_DUMMYSYSTEM_HPP
#define DUMMYPLUGIN_DUMMYSYSTEM_HPP

#include <DummyPluginMacros.hpp>

#include <Engine/System/System.hpp>

namespace Ra {
namespace Core {
struct TriangleMesh;
}
} // namespace Ra

namespace Ra {
namespace Engine {
class Entity;
struct RenderTechnique;
class Component;
} // namespace Engine
} // namespace Ra

namespace DummyPlugin {
class DummyComponent;

struct DummyData {
    int foo;
    int bar;
};

class DummySystem : public Ra::Engine::System
{
  public:
    DummySystem();
    ~DummySystem();

    void handleAssetLoading( Ra::Engine::Entity* entity,
                             const Ra::Core::Asset::FileData* data ) override;

    void generateTasks( Ra::Core::TaskQueue* taskQueue,
                        const Ra::Engine::FrameInfo& frameInfo ) override;

  private:
    DummyData* m_data;
};

} // namespace DummyPlugin

#endif // DUMMYPLUGIN_DUMMYSYSTEM_HPP
