#ifndef CAMERAMANIPPLUGIN_HPP_
#define CAMERAMANIPPLUGIN_HPP_

#include <Core/Animation/KeyFramedValue.hpp>

#include <Core/CoreMacros.hpp>
#include <PluginBase/RadiumPluginInterface.hpp>
#include <QObject>

#include <UI/CameraManipUI.h>

#include <CameraManipPluginMacros.hpp>

namespace Ra {
namespace Engine {
class RadiumEngine;
class Entity;
} // namespace Engine
} // namespace Ra

namespace CameraManipPlugin {

// Due to an ambigous name while compiling with Clang, must differentiate plugin class from plugin
// namespace
/// The CameraManipPlugin allows to change the current camera used for display,
/// create a copy of the current camera and save the current camera data to a file.
class CameraManipPluginC : public QObject, Ra::Plugins::RadiumPluginInterface
{
    Q_OBJECT
    Q_RADIUM_PLUGIN_METADATA
    Q_INTERFACES( Ra::Plugins::RadiumPluginInterface )

  public:
    CameraManipPluginC();
    ~CameraManipPluginC() override;

    void registerPlugin( const Ra::Plugins::Context& context ) override;

    bool doAddWidget( QString& name ) override;
    QWidget* getWidget() override;

    bool doAddMenu() override;
    QMenu* getMenu() override;

    bool doAddAction( int& nb ) override;
    QAction* getAction( int id ) override;

  public slots:
    void useSelectedCamera();
    void saveCamera();
    void createCamera();
    void keyFrameCamera( bool on );
    void onCurrentChanged( const QModelIndex& current, const QModelIndex& prev );

  signals:
    void askForUpdate();

  private:
    CameraManipUI* m_widget;

    Ra::Engine::RadiumEngine* m_engine;
    Ra::GuiBase::SelectionManager* m_selectionManager;
    Ra::GuiBase::Timeline* m_timeline;
    Ra::Gui::Viewer* m_viewer;

    using CameraPath = Ra::Core::Animation::KeyFramedValue<Ra::Core::Transform>;
    std::map<Ra::Engine::Camera*, std::pair<CameraPath*, Ra::Core::Transform>> m_cameraPaths;
    Scalar m_currentTime;
};

} // namespace CameraManipPlugin

#endif // CAMERAMANIPPLUGIN_HPP_
