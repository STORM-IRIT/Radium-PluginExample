#include <system.hpp>

#include <Core/String/StringUtils.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>

#include <Engine/RadiumEngine.hpp>
#include <Engine/Entity/Entity.hpp>
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
        QString entityName = QString::fromStdString(entity->getName());

        LOG(logINFO) << "New entity: " << qPrintable(entityName)
                     << " with " << geomData.size()
                     << " geometry "
                     << (geomData.size() <= 1 ? " component." : "components.");

        // Register the geometry data associated to the loaded entities
        for ( const auto& geometry : geomData )
        {
            emit newInputModelRegistered(entityName,
                                         QString::fromStdString(geometry->getName()));
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

        LOG(logINFO) << "Example Plugin System: computation requested.";

        // Get the triangle mesh associated to the selected point
        TriangleMesh* mesh = nullptr;

        {
            auto em =  Ra::Engine::RadiumEngine::getInstance()->getEntityManager();
            Ra::Engine::Entity* e = em->entityExists(p.entityName.toStdString()) ?
                        em->getEntity(p.entityName.toStdString()):
                        nullptr;

            if( e!= nullptr ){
                // here we check if any component exports the a rw access to the
                // triangle mesh.
                // In practice, the plugin FancyMesh embedded in Radium does it.
                // @see FancyMeshComponent.cpp:156
                bool hasMesh = ComponentMessenger::getInstance()->
                        canRw<TriangleMesh>( e, p.dataId.toStdString() );

                if (hasMesh){
                    mesh = ComponentMessenger::getInstance()->
                            rwCallback<TriangleMesh>( e, p.dataId.toStdString() )();
                }
            }
        }

        // process geometry
        if (mesh != nullptr) {
            LOG(logINFO) << "Example Plugin System: processing starts...";

            // TODO
        }
    }

} // namespace MyPlugin
