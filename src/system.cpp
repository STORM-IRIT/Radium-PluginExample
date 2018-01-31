#include <system.hpp>

#include <Core/String/StringUtils.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>
#include <Core/Mesh/TriangleMesh.hpp>
#include <Engine/Renderer/Mesh/Mesh.hpp>

#include <Engine/RadiumEngine.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Core/File/FileData.hpp>
#include <Core/File/GeometryData.hpp>
#include <Engine/Managers/ComponentMessenger/ComponentMessenger.hpp>
#include <Engine/Managers/EntityManager/EntityManager.hpp>

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
        const auto& components = entity->getComponents();
        QString entityName = QString::fromStdString(entity->getName());

        LOG(logINFO) << "New entity: " << qPrintable(entityName)
                     << " with " << components.size()
                     << " geometry"
                     << (components.size() <= 1 ? " component." : "components.");

        // Register the geometry data associated to the loaded entities
        for ( const auto& comp : components )
        {
            emit newInputModelRegistered(entityName,
                                         QString::fromStdString(comp->getName()));
        }
    }

    void MySystem::generateTasks( Ra::Core::TaskQueue* /*taskQueue*/,
                                  const Ra::Engine::FrameInfo& /*frameInfo*/ )
    {
    }

    /*!
     * \brief MySystem::compute Called from the UI, smooths the triangle mesh
     * \param p Parameter set from the ui.
     */
    void MySystem::compute(Param p)
    {
        using Ra::Engine::ComponentMessenger;
        using Ra::Core::TriangleMesh;
	using Ra::Engine::Mesh;

        LOG(logINFO) << "Example Plugin System: computation requested.";

	Mesh* mesh = Ra::Engine::RadiumEngine::getInstance()->getMesh(p.entityName.toStdString(),
								      p.dataId.toStdString() );

        // process geometry
        if (mesh != nullptr) {
            LOG(logINFO) << "Example Plugin System: processing starts...";

            TriangleMesh& trmesh = mesh->getGeometry();
            // TODO
        }
    }

} // namespace MyPlugin
