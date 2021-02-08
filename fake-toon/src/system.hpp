#ifndef MyPLUGIN_MyPLUGIN_HPP_
#define MyPLUGIN_MyPLUGIN_HPP_

#include "plugin.hpp"

#include <Engine/Scene/System.hpp>

namespace FakeToonPluginExample {
/*!
 * \brief System class of the plugin tutorial
 *
 * The system links the plugin to Radium using callbacks (#handleAssetLoading)
 * and use the #ComponentMessenger to access and modify the 3d model geometry.
 *
 */
class FAKE_TOON_PLUGIN_EXAMPLE_API MySystem : public QObject, public Ra::Engine::Scene::System
{
    Q_OBJECT
  public:
    MySystem();
    virtual ~MySystem();

    virtual void handleAssetLoading( Ra::Engine::Scene::Entity* entity,
                                     const Ra::Core::Asset::FileData* fileData ) override;

    virtual void generateTasks( Ra::Core::TaskQueue* taskQueue,
                                const Ra::Engine::FrameInfo& frameInfo ) override;
};

} // namespace FakeToonPluginExample

#endif // MyPLUGIN_MyPLUGIN_HPP_
