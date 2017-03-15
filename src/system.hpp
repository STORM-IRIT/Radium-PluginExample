#ifndef MyPLUGIN_MyPLUGIN_HPP_
#define MyPLUGIN_MyPLUGIN_HPP_

#include "plugin.hpp"

#include <Engine/System/System.hpp>

namespace Ra
{
    namespace Core
    {
        struct TriangleMesh;
    }
}

namespace Ra
{
    namespace Engine
    {
        class Entity;
        struct RenderTechnique;
        class Component;
    }
}

namespace MyPluginExample
{
    /*!
     * \brief System class of the plugin tutorial
     *
     * The system links the plugin to Radium using callbacks (#handleAssetLoading)
     * and use the #ComponentMessenger to access and modify the 3d model geometry.
     *
     */
    class PLUGIN_EXAMPLE_API MySystem : public QObject, public Ra::Engine::System
    {
        Q_OBJECT
    public:
        MySystem();
        virtual ~MySystem();

        virtual void handleAssetLoading( Ra::Engine::Entity* entity,
                                         const Ra::Asset::FileData* fileData ) override;

        virtual void generateTasks( Ra::Core::TaskQueue* taskQueue,
                                    const Ra::Engine::FrameInfo& frameInfo ) override;

    public slots:
        void compute(Param p);

    signals:
        void newInputModelRegistered( const QString& entityName,
                                      const QString& componentName );
    };

} // namespace MyPlugin

#endif // MyPLUGIN_MyPLUGIN_HPP_
