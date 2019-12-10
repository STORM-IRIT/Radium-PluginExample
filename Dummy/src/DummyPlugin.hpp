#ifndef DUMMYPLUGIN_DUMMYPLUGIN_HPP
#define DUMMYPLUGIN_DUMMYPLUGIN_HPP

#include <DummyPluginMacros.hpp>

#include <QObject>
#include <QtPlugin>

#include <PluginBase/RadiumPluginInterface.hpp>

class QLabel;

namespace Ra {
namespace Engine {
class RadiumEngine;
}
} // namespace Ra

namespace DummyPlugin {
class DummySystem;

class DummyPlugin : public QObject, Ra::Plugins::RadiumPluginInterface
{
    Q_OBJECT
    Q_RADIUM_PLUGIN_METADATA
    Q_INTERFACES( Ra::Plugins::RadiumPluginInterface )

  public:
    DummyPlugin() = default;
    ~DummyPlugin();

    void registerPlugin( const Ra::Plugins::Context& context ) override;

    bool doAddWidget( QString& name ) override;
    QWidget* getWidget() override;

    bool doAddMenu() override;
    QMenu* getMenu() override;

    bool doAddAction( int& nb ) override;
    QAction* getAction( int id ) override;

  private slots:
    void sayHello();

  private:
    DummySystem* m_system{nullptr};
    QLabel* m_label{nullptr};
};

} // namespace DummyPlugin

#endif // FANCYMESHPLUGIN_HPP_
