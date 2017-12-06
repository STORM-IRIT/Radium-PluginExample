#ifndef _PLUGIN_EXAMPLE_HPP_
#define _PLUGIN_EXAMPLE_HPP_

#include <Core/CoreMacros.hpp>
/// Defines the correct macro to export dll symbols.
#if defined PluginExample_EXPORTS
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
        QString entityName;
        QString dataId;
    };

// Du to an ambigous name while compiling with Clang, must differentiate plugin claas from plugin namespace
    /*!
     * \brief Plugin class of this tutorial, wiring the UI and the System in Radium
     *
     * The main objective of this class are:
     *  - storing the plugin UI and widget
     *  - registering the plugin and its system in Radium: #registerPlugin
     *  - add the widget in the UI: #doAddWidget and #getWidget.
     *
     */
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

        virtual bool doAddAction( int &nb ) override;
        virtual QAction* getAction( int id ) override;


    private:
        MySystem* _system;
        MyWidget* _widget;
    };

} // namespace

#endif // _PLUGIN_EXAMPLE_HPP_
