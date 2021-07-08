#ifndef MESHANIMATORPLUGIN_HPP_
#define MESHANIMATORPLUGIN_HPP_

#include <Core/CoreMacros.hpp>
#include <PluginBase/RadiumPluginInterface.hpp>
#include <QAction>
#include <QObject>
#include <QtPlugin>

#include <Core/Animation/KeyFramedValue.hpp>

#include <UI/MeshAnimatorUI.h>

#include <MeshAnimatorPluginMacros.hpp>

namespace Ra {
namespace Engine {
class Component;
} // namespace Engine
} // namespace Ra

namespace MeshAnimatorPlugin {
class MeshAnimatorSystem;

// Due to an ambigous name while compiling with Clang, must differentiate plugin class from plugin
// namespace
class MESH_ANIMATOR_PLUGIN_API MeshAnimatorPluginC : public QObject, Ra::Plugins::RadiumPluginInterface
{
    Q_OBJECT
    Q_RADIUM_PLUGIN_METADATA
    Q_INTERFACES( Ra::Plugins::RadiumPluginInterface )

  public:
    MeshAnimatorPluginC();
    ~MeshAnimatorPluginC() override;

    void registerPlugin( const Ra::Plugins::Context& context ) override;

    bool doAddWidget( QString& name ) override;
    QWidget* getWidget() override;

    bool doAddMenu() override;
    QMenu* getMenu() override;

    bool doAddAction( int& nb ) override;
    QAction* getAction( int id ) override;

  public slots:
    void onCurrentChanged( const QModelIndex& current, const QModelIndex& prev );
    void update();
    void indexChanged( int );
    void animatePosition( bool );
    void animateNormal( bool );

  private:
    MeshAnimatorSystem* m_system {nullptr};
    MeshAnimatorUI* m_widget {nullptr};

    Ra::GuiBase::SelectionManager* m_selectionManager {nullptr};
    Ra::Gui::PickingManager* m_PickingManager {nullptr};
    Ra::GuiBase::Timeline* m_timeline {nullptr};

    Ra::Engine::Component* m_component {nullptr};
    Ra::Core::Utils::Index m_currentRO;
    int m_currentVertex {-1};

    using KeyFramedValue = Ra::Core::Animation::KeyFramedValue<Ra::Core::Vector3>;
    using KeyFrames      = std::map<int, std::pair<KeyFramedValue*, Ra::Core::Vector3>>;
    std::map<Ra::Core::Utils::Index, KeyFrames> m_positionKeyframes;
    std::map<Ra::Core::Utils::Index, KeyFrames> m_normalKeyframes;
};

} // namespace MeshAnimatorPlugin

#endif // MESHANIMATORPLUGIN_HPP_
