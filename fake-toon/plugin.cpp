#include <plugin.hpp>

#include <Engine/RadiumEngine.hpp>

#include <system.hpp>
#include "FakeToonMaterial.hpp"

namespace FakeToonPluginExample
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
        context.m_engine->registerSystem( "FakeToonSystem", _system );
    }
    QWidget* MyPluginC::getWidget()
    {
        return nullptr;
    }

    QMenu* MyPluginC::getMenu()
    {
        return nullptr;
    }

    QAction *MyPluginC::getAction(int /*id*/)
    {
      return nullptr;
    }

    void MyPluginC::openGlInitialize(const Ra::PluginContext &/*context*/)
    {
      FakeToonMaterial::registerMaterial();
    }
}
