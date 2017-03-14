#ifndef _PLUGIN_HPP_
#define _PLUGIN_HPP_

#include <Core/CoreMacros.hpp>
/// Defines the correct macro to export dll symbols.
#if defined  DPluginExample_EXPORTS
    #define PLUGIN_EXAMPLE_API DLL_EXPORT
#else
    #define PLUGIN_EXAMPLE_API DLL_IMPORT
#endif

#include <QObject>
#include <QtPlugin>

#include <PluginBase/RadiumPluginInterface.hpp>

namespace Ra
{
    namespace Engine
    {
        class RadiumEngine;
    }
}

namespace MyPluginExample
{
    class MySystem;
    class MyWidget;

    struct Param {
        std::string entityName;
    };

// Du to an ambigous name while compiling with Clang, must differentiate plugin claas from plugin namespace
    class MyPluginC : public QObject, Ra::Plugins::RadiumPluginInterface
    {
        Q_OBJECT
        Q_PLUGIN_METADATA( IID "RadiumEngine.PluginInterface" )
        Q_INTERFACES( Ra::Plugins::RadiumPluginInterface )

    public:
        MyPluginC();
        virtual ~MyPluginC();

        virtual void registerPlugin( const Ra::PluginContext& context ) override;

        virtual bool doAddWidget( QString& name ) override;
        virtual QWidget* getWidget() override;

        virtual bool doAddMenu() override;
        virtual QMenu* getMenu() override;

    private slots:
        void registerInputModel( const std::string& name);

    private:
        MySystem* _system;
        MyWidget* _widget;
    };

} // namespace

#endif // MyPLUGIN_HPP_
