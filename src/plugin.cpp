#include <plugin.hpp>

#include <Engine/RadiumEngine.hpp>

#include <system.hpp>
#include <widget.hpp>

namespace MyPluginExample
{

    MyPluginC::MyPluginC()
        : _system (nullptr),
          _widget (nullptr)
    {
    }

    MyPluginC::~MyPluginC()
    {
    }

    void MyPluginC::registerPlugin( const Ra::PluginContext& context )
    {
        delete (_system);
        _system = new MySystem;
        context.m_engine->registerSystem( "MySystem", _system );
    }

    bool MyPluginC::doAddWidget( QString &name )
    {
        delete (_widget);
        _widget = new MyWidget();

        connect(_system, &MySystem::newInputModelRegistered,
                _widget, &MyWidget::addInputModel);
        connect(_widget, &MyWidget::computationRequested,
                _system, &MySystem::compute);

        name = "Plugin Example: Laplacian smoothing";
        return true;
    }

    QWidget* MyPluginC::getWidget()
    {
        return _widget;
    }

    bool MyPluginC::doAddMenu()
    {
        return false;
    }

    QMenu* MyPluginC::getMenu()
    {
        return nullptr;
    }

    void MyPluginC::registerInputModel(const std::string &name)
    {
        _widget->addInputModel(name);
    }
}
