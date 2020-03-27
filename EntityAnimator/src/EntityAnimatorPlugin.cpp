#include <EntityAnimatorPlugin.hpp>

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <Engine/RadiumEngine.hpp>

#include <EntityAnimatorSystem.hpp>

namespace EntityAnimatorPlugin {

EntityAnimatorPluginC::~EntityAnimatorPluginC() {}

void EntityAnimatorPluginC::registerPlugin( const Ra::Plugins::Context& context ) {
    EntityAnimatorSystem* system = new EntityAnimatorSystem;
    system->setTimeline( context.m_timeline );
    context.m_engine->registerSystem( "EntityAnimatorSystem", system );
}

bool EntityAnimatorPluginC::doAddWidget( QString& /*name*/ ) {
    return false;
}

QWidget* EntityAnimatorPluginC::getWidget() {
    return nullptr;
}

bool EntityAnimatorPluginC::doAddMenu() {
    return false;
}

QMenu* EntityAnimatorPluginC::getMenu() {
    return nullptr;
}

bool EntityAnimatorPluginC::doAddAction( int& /*nb*/ ) {
    return false;
}

QAction* EntityAnimatorPluginC::getAction( int /*id*/ ) {
    return nullptr;
}

} // namespace EntityAnimatorPlugin
