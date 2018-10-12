#include <system.hpp>

#include <Core/String/StringUtils.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>
#include <Core/Mesh/TriangleMesh.hpp>
#include <Engine/Renderer/Mesh/Mesh.hpp>
#include <Core/Mesh/HalfEdge.hpp>
#include <Core/Mesh/MeshUtils.hpp>
#include <Core/Mesh/TopologicalTriMesh/TopologicalMesh.hpp>

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
                     << (components.size() <= 1 ? " component." : " components.");

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
    using Ra::Core::TopologicalMesh;
    using Ra::Core::Triangle;
    using Ra::Core::Vector3;
    using Ra::Core::VertexIdx;
    using Ra::Engine::Mesh;

    LOG(logINFO) << "Example Plugin System: computation requested.";

    Mesh* mesh = Ra::Engine::RadiumEngine::getInstance()->getMesh(p.entityName.toStdString(),
                                                                  p.dataId.toStdString() );

    // process geometry
    if (mesh != nullptr) {
        LOG(logINFO) << "Example Plugin System: processing starts...";

        TopologicalMesh topologicalMesh (mesh->getGeometry());

        OpenMesh::VPropHandleT<Scalar> vertex_weights_;
        topologicalMesh.add_property(vertex_weights_);
        TopologicalMesh::VertexIter v_it, v_end(topologicalMesh.vertices_end());
        for (v_it=topologicalMesh.vertices_begin(); v_it!=v_end; ++v_it)
            topologicalMesh.property(vertex_weights_, *v_it) = 0.0;

        TopologicalMesh::HalfedgeHandle heh0, heh1, heh2;
        TopologicalMesh::VertexHandle v0, v1;
        TopologicalMesh::Point *p0, *p1, *p2;
        TopologicalMesh::Normal d0, d1;
        TopologicalMesh::Scalar weight, lb(-1.0), ub(1.0);

        TopologicalMesh::EdgeIter e_it, e_end(topologicalMesh.edges_end());
        for (e_it=topologicalMesh.edges_begin(); e_it!=e_end; ++e_it)
        {
            weight = 0.0;

            heh0   = topologicalMesh.halfedge_handle(*e_it, 0);
            v0     = topologicalMesh.to_vertex_handle(heh0);
            p0     = &topologicalMesh.point(v0);

            heh1   = topologicalMesh.halfedge_handle(*e_it, 1);
            v1     = topologicalMesh.to_vertex_handle(heh1);
            p1     = &topologicalMesh.point(v1);

            heh2   = topologicalMesh.next_halfedge_handle(heh0);
            p2     = &topologicalMesh.point(topologicalMesh.to_vertex_handle(heh2));
            d0     = (*p0 - *p2); d0.normalize();
            d1     = (*p1 - *p2); d1.normalize();
            weight += static_cast<TopologicalMesh::Scalar>(1.0) / std::tan(acos(std::max(lb, std::min(ub, d0.dot(d1) ))));

            heh2   = topologicalMesh.next_halfedge_handle(heh1);
            p2     = &topologicalMesh.point(topologicalMesh.to_vertex_handle(heh2));
            d0     = (*p0 - *p2); d0.normalize();
            d1     = (*p1 - *p2); d1.normalize();
            weight += static_cast<TopologicalMesh::Scalar>(1.0) / std::tan(acos(std::max(lb, std::min(ub, d0.dot(d1) ))));

            topologicalMesh.property(vertex_weights_, v0) += weight;
            topologicalMesh.property(vertex_weights_, v1) += weight;
        }

        // invert vertex weights:
        // before: sum of edge weights
        // after: one over sum of edge weights
        for (v_it=topologicalMesh.vertices_begin(); v_it!=v_end; ++v_it)
        {
            weight = topologicalMesh.property(vertex_weights_, *v_it);
            if (weight)
                topologicalMesh.property(vertex_weights_, *v_it) = static_cast<TopologicalMesh::Scalar>(1.0) / weight;
        }

        std::vector<TopologicalMesh::Point> cogs;
        std::vector<TopologicalMesh::Point>::iterator cog_it;
        cogs.reserve(topologicalMesh.n_vertices());
        TopologicalMesh::Point cog;

        v_end = topologicalMesh.vertices_end();
        for (v_it=topologicalMesh.vertices_begin(); v_it!=v_end; ++v_it)
        {
            cog[0] = cog[1] = cog[2] = 0.f;
            TopologicalMesh::Scalar sum = 0.f;

            TopologicalMesh::VertexVertexIter vv_it;
            for (vv_it=topologicalMesh.vv_iter( v_it ); vv_it; ++vv_it) {
                // Note: if we set w = 1.0f then the procedures operates
                // an uniform smoothing, the initial distribution of
                // the triangles will not be preserved
                // TopologicalMesh::Scalar w = 1.f;

                TopologicalMesh::Scalar w = topologicalMesh.property(vertex_weights_, *vv_it);

                cog += topologicalMesh.point(vv_it) * w;
                sum += w;
            }
            assert(sum);
            cogs.push_back(cog / sum);
        }

        for (v_it=topologicalMesh.vertices_begin(), cog_it=cogs.begin(); v_it!=v_end; ++v_it, ++cog_it) {
            topologicalMesh.set_point( v_it, (*cog_it));
        }

        mesh->loadGeometry( topologicalMesh.toTriangleMesh() );
    }
}

} // namespace MyPlugin
