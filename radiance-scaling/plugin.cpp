#include <plugin.hpp>

#include <Engine/RadiumEngine.hpp>

#include <system.hpp>

namespace MyPluginExample
{

    MyPluginC::MyPluginC()
        : _system (nullptr)
    {
    }

    MyPluginC::~MyPluginC()
    {
    }

    void MyPluginC::registerPlugin( const Ra::PluginContext& context )
    {
        delete (_system);
        _system = new MySystem;
        context.m_engine->registerSystem( "RadianceScalingSystem", _system );
    }

    bool MyPluginC::doAddWidget( QString &name )
    {
        return false;
    }

    QWidget* MyPluginC::getWidget()
    {
        return nullptr;
    }

    bool MyPluginC::doAddMenu()
    {
        return false;
    }

    QMenu* MyPluginC::getMenu()
    {
        return nullptr;
    }

    bool MyPluginC::doAddAction(int &/*nb*/)
    {
        return false;
    }

    QAction *MyPluginC::getAction(int /*id*/)
    {
        return nullptr;
    }
}
