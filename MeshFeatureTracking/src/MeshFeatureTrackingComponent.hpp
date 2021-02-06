#ifndef MESHFEATURETRACKINGPLUGIN_MESHFEATURETRACKING_COMPONENT_HPP_
#define MESHFEATURETRACKINGPLUGIN_MESHFEATURETRACKING_COMPONENT_HPP_

#include <MeshFeatureTrackingPluginMacros.hpp>

#include <Engine/Scene/Component.hpp>
#include <Gui/Utils/PickingManager.hpp>

#include <UI/MeshFeatureTrackingUI.h>

namespace Ra {
namespace Engine {
namespace Data{
class Mesh;
}
}
} // namespace Ra

namespace MeshFeatureTrackingPlugin {
class MeshFeatureTrackingComponent : public Ra::Engine::Scene::Component
{
  public:
    MeshFeatureTrackingComponent( const std::string& name );

    ~MeshFeatureTrackingComponent() override;

    virtual void initialize() override;

    void setData( const Ra::Engine::Rendering::Renderer::PickingResult& data );

    void setVertexIdx( uint idx );
    void setTriangleIdx( uint idx );

    void update();

    int getMaxV() const;
    int getMaxT() const;

    FeatureData getFeatureData() const;
    Scalar getFeatureScale() const;
    Ra::Core::Vector3 getFeatureVector() const;
    Ra::Core::Vector3 getFeaturePosition() const;

    /// Registers the new center for the sphere.
    void setPosition( Ra::Core::Vector3 position );
    /// Registers the new scale for the sphere.
    void setScale( Scalar scale );

    FeatureData m_data;
    Ra::Core::Utils::Index m_pickedRoIdx;
    Ra::Engine::Data::Mesh* m_pickedMesh;
    Ra::Engine::Rendering::RenderObject* m_RO;
};
} // namespace MeshFeatureTrackingPlugin

#endif // MESHFEATURETRACKINGPLUGIN_MESHFEATURETRACKING_COMPONENT_HPP_
