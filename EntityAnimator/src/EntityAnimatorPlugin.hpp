#ifndef ENTITYANIMATORPLUGIN_HPP
#define ENTITYANIMATORPLUGIN_HPP

#include <EntityAnimatorPluginMacros.hpp>

#include <QObject>
#include <QtPlugin>

#include <PluginBase/RadiumPluginInterface.hpp>

namespace EntityAnimatorPlugin {
class EntityAnimatorSystem;

class ENTITY_ANIMATOR_PLUGIN_API EntityAnimatorPluginC : public QObject, Ra::Plugins::RadiumPluginInterface
{
    Q_OBJECT
    Q_RADIUM_PLUGIN_METADATA
    Q_INTERFACES( Ra::Plugins::RadiumPluginInterface )

  public:
    EntityAnimatorPluginC() = default;
    ~EntityAnimatorPluginC() override;

    void registerPlugin( const Ra::Plugins::Context& context ) override;

    bool doAddWidget( QString& name ) override;
    QWidget* getWidget() override;

    bool doAddMenu() override;
    QMenu* getMenu() override;

    bool doAddAction( int& nb ) override;
    QAction* getAction( int id ) override;
};

} // namespace EntityAnimatorPlugin

#endif // ENTITYANIMATORPLUGIN_HPP
