#include <plugin.hpp>

#include <Engine/RadiumEngine.hpp>

#include <system.hpp>
#include <widget.hpp>

namespace MyPluginExample {

MyPluginC::MyPluginC() : _system( nullptr ), _widget( nullptr ) {}

MyPluginC::~MyPluginC() {}

void MyPluginC::registerPlugin( const Ra::Plugins::Context& context ) {
    delete ( _system );
    _system = new MySystem;
    context.m_engine->registerSystem( "LaplacianSmoothingSystem", _system );
}

bool MyPluginC::doAddWidget( QString& name ) {
    delete ( _widget );
    _widget = new MyWidget();

    // Wire system and UI
    connect( _system, &MySystem::newInputModelRegistered, _widget, &MyWidget::addInputModel );
    connect( _widget, &MyWidget::computationRequested, _system, &MySystem::compute );

    name = "Plugin Example: Laplacian smoothing";
    return true;
}

QWidget* MyPluginC::getWidget() {
    return _widget;
}

bool MyPluginC::doAddMenu() {
    return false;
}

QMenu* MyPluginC::getMenu() {
    return nullptr;
}

bool MyPluginC::doAddAction( int& /*nb*/ ) {
    return false;
}

QAction* MyPluginC::getAction( int /*id*/ ) {
    return nullptr;
}
} // namespace MyPluginExample
