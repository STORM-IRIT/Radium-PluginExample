#ifndef MESHPAINTPLUGIN_MESHPAINTSYSTEM_HPP
#define MESHPAINTPLUGIN_MESHPAINTSYSTEM_HPP

#include <MeshPaintPluginMacros.hpp>

#include <Core/Utils/Color.hpp>

#include <Engine/Rendering/Renderer.hpp>
#include <Engine/Scene/System.hpp>

namespace Ra {
namespace Engine {
class Entity;
class RenderTechnique;
class Component;
} // namespace Engine
} // namespace Ra

namespace MeshPaintPlugin {
class MeshPaintComponent;
}

namespace MeshPaintPlugin {
class MESH_PAINT_PLUGIN_API MeshPaintSystem : public Ra::Engine::Scene::System
{
  public:
    MeshPaintSystem();
    virtual ~MeshPaintSystem();

    virtual void handleAssetLoading( Ra::Engine::Scene::Entity* entity,
                                     const Ra::Core::Asset::FileData* fileData ) override;

    virtual void generateTasks( Ra::Core::TaskQueue* taskQueue,
                                const Ra::Engine::FrameInfo& frameInfo ) override;

    void startPaintMesh( bool start );

    void paintMesh( const Ra::Engine::Rendering::Renderer::PickingResult& picking,
                    const Ra::Core::Utils::Color& color );

    void bakeToDiffuse();
};

} // namespace MeshPaintPlugin

#endif // MESHPAINTPLUGIN_MESHPAINTSYSTEM_HPP
