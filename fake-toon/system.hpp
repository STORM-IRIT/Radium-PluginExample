#ifndef MyPLUGIN_MyPLUGIN_HPP_
#define MyPLUGIN_MyPLUGIN_HPP_

#include "plugin.hpp"

#include <Engine/System/System.hpp>


namespace FakeToonPluginExample
{
    /*!
     * \brief System class of the plugin tutorial
     *
     * The system links the plugin to Radium using callbacks (#handleAssetLoading)
     * and use the #ComponentMessenger to access and modify the 3d model geometry.
     *
     */
    class FAKE_TOON_PLUGIN_EXAMPLE_API MySystem : public QObject, public Ra::Engine::System
    {
        Q_OBJECT
    public:
        MySystem();
        virtual ~MySystem();

        virtual void handleAssetLoading( Ra::Engine::Entity* entity,
                                         const Ra::Asset::FileData* fileData ) override;

        virtual void generateTasks( Ra::Core::TaskQueue* taskQueue,
                                    const Ra::Engine::FrameInfo& frameInfo ) override;
    };

} // namespace MyPlugin

#endif // MyPLUGIN_MyPLUGIN_HPP_
