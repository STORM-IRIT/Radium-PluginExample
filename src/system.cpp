#include <system.hpp>

#include <Core/String/StringUtils.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>

#include <Engine/RadiumEngine.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/FrameInfo.hpp>
#include <Engine/Renderer/RenderTechnique/RenderTechnique.hpp>
#include <Engine/Assets/FileData.hpp>
#include <Engine/Assets/GeometryData.hpp>
#include <Engine/Managers/ComponentMessenger/ComponentMessenger.hpp>

namespace MyPluginExample
{

    MySystem::MySystem()
        : Ra::Engine::System()
    {
        LOG(logINFO) << "Example Plugin System created.";
    }

    MySystem::~MySystem()
    {
        LOG(logINFO) << "Example Plugin System destroyed.";
    }

    void MySystem::handleAssetLoading( Ra::Engine::Entity* entity,
                                       const Ra::Asset::FileData* fileData )
    {
        auto geomData = fileData->getGeometryData();

        uint id = 0;

        for ( const auto& data : geomData )
        {
            std::string componentName = "FMC_" + entity->getName() + std::to_string( id++ );
            LOG(logINFO) << "New entity: " << componentName.c_str();
            emit newInputModelRegistered(QString::fromStdString(componentName));
        }
    }

    void MySystem::generateTasks( Ra::Core::TaskQueue* /*taskQueue*/,
                                  const Ra::Engine::FrameInfo& /*frameInfo*/ )
    {
    }

    void MySystem::compute(Param /*p*/)
    {
        LOG(logINFO) << "Example Plugin System: computation requested.";
    }

} // namespace MyPlugin
