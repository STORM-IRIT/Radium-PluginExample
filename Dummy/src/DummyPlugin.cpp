#include "DummyPlugin.hpp"

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <Engine/RadiumEngine.hpp>

#include "DummySystem.hpp"

namespace DummyPlugin {

DummyPluginC::~DummyPluginC() {}

void DummyPluginC::registerPlugin( const Ra::Plugins::Context& context ) {
    DummySystem* system = new DummySystem;
    context.m_engine->registerSystem( "DummySystem", system );
}

bool DummyPluginC::doAddWidget( QString& name ) {
    name = "Dummy Mesh";
    return true;
}

QWidget* DummyPluginC::getWidget() {
    QWidget* widget = new QWidget;

    m_label             = new QLabel( "I am a dummy label", widget );
    QPushButton* button = new QPushButton( "And I am a dummy button", widget );

    QVBoxLayout* layout = new QVBoxLayout( widget );

    layout->addWidget( m_label );
    layout->addWidget( button );

    connect( button, &QPushButton::pressed, this, &DummyPluginC::sayHello );

    return widget;
}

bool DummyPluginC::doAddMenu() {
    return true;
}

QMenu* DummyPluginC::getMenu() {
    QMenu* menu = new QMenu( "Dummy" );

    QAction* a1 = menu->addAction( "Hello there" );
    QAction* a2 = menu->addAction( "And there" );

    CORE_UNUSED( a1 );
    CORE_UNUSED( a2 );

    return menu;
}

bool DummyPluginC::doAddAction( int& /*nb*/ ) {
    return false;
}

QAction* DummyPluginC::getAction( int /*id*/ ) {
    return nullptr;
}

void DummyPluginC::sayHello() {
    m_label->setText( "I have been clicked at least once !" );
    QMessageBox::warning( nullptr, "Dummy Plugin", "Hello, Radium World !" );
}
} // namespace DummyPlugin
