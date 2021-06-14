#include <MeshPaintComponent.hpp>

#include <iostream>

#include <Core/Utils/Log.hpp>

#include <Core/Geometry/TriangleMesh.hpp>
#include <Core/Tasks/TaskQueue.hpp>

#include <Engine/Data/Material.hpp>
#include <Engine/Data/Mesh.hpp>
#include <Engine/Data/ShaderConfigFactory.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Rendering/RenderTechnique.hpp>
#include <Engine/Scene/Entity.hpp>

using namespace Ra::Core::Utils; // log

namespace MeshPaintPlugin {

static const std::string colAttribName =
    Ra::Engine::Data::Mesh::getAttribName( Ra::Engine::Data::Mesh::VERTEX_COLOR );

MeshPaintComponent::MeshPaintComponent( const std::string& name,
                                        Ra::Engine::Scene::Entity* entity ) :
    Ra::Engine::Scene::Component( name, entity ), m_mesh( nullptr ) {}

MeshPaintComponent::~MeshPaintComponent() {}

void MeshPaintComponent::addTasks( Ra::Core::TaskQueue* /*taskQueue*/,
                                   const Ra::Engine::FrameInfo& /*info*/ ) {}

void MeshPaintComponent::setDataId( const std::string& id ) {
    m_dataId = id;
}

void MeshPaintComponent::initialize() {
    using Ra::Engine::Data::Mesh;
    auto compMess = Ra::Engine::Scene::ComponentMessenger::getInstance();
    // Look for the data we need
    bool geometryData = compMess->canGet<Ra::Core::Geometry::TriangleMesh>( getEntity(), m_dataId );

    if ( !geometryData )
    {
        LOG( logWARNING )
            << "Cannot initialize MeshPaint Component: no TriangleMesh in geometry data.";
        return;
    }

    m_renderObjectReader =
        compMess->getterCallback<Ra::Core::Utils::Index>( getEntity(), m_dataId );

    auto ro      = getRoMgr()->getRenderObject( *m_renderObjectReader() );
    m_baseConfig = ro->getRenderTechnique()->getConfiguration();

    m_mesh = dynamic_cast<Mesh*>( ro->getMesh().get() );
    CORE_ASSERT( m_mesh != nullptr, "Cannot access Mesh from Ro containing a TriangleMesh" );
}

void MeshPaintComponent::startPaint( bool on ) {
    if ( !getRoMgr()->exists( *m_renderObjectReader() ) ) { return; }
    CORE_ASSERT( m_mesh != nullptr, "We should be have a Mesh storing a TriangleMesh here" );

    if ( on )
    {
        // Could also be accessed using
        // auto triangleMesh = compMess->get<Ra::Core::Geometry::TriangleMesh>( getEntity(),
        // m_dataId ); however here we skip the search in the component map
        Ra::Core::Geometry::TriangleMesh& triangleMesh = m_mesh->getCoreGeometry();

        // If any, save colors from the CPU object, otherwise set default color and notify GPU
        // object
        m_currentColorAttribHdl = triangleMesh.getAttribHandle<Ra::Core::Vector4>( colAttribName );
        if ( m_currentColorAttribHdl.idx().isInvalid() )
        {
            m_baseColors.clear();
            m_isBaseColorValid = false;

            m_currentColorAttribHdl = triangleMesh.addAttrib<Ra::Core::Vector4>( colAttribName );
            triangleMesh.getAttrib( m_currentColorAttribHdl )
                .setData( Ra::Core::Vector4Array( triangleMesh.vertices().size(),
                                                  Ra::Core::Utils::Color::Skin() ) );
        }
        else
        {
            m_isBaseColorValid = true;

            m_baseColors = triangleMesh.getAttrib( m_currentColorAttribHdl ).data(); // copy
        }
    }
    else
    {
        ON_DEBUG( auto colAttrib = m_mesh->getCoreGeometry().getAttribHandle<Ra::Core::Vector4>(
                      colAttribName ) );
        CORE_ASSERT( colAttrib == m_currentColorAttribHdl, "Inconsistent AttribHandle used" );
        // Could also be accessed using
        // auto triangleMesh = compMess->get<Ra::Core::Geometry::TriangleMesh>( getEntity(),
        // m_dataId ); however here we skip the search in the component map
        Ra::Core::Geometry::TriangleMesh& triangleMesh = m_mesh->getCoreGeometry();
        if ( m_isBaseColorValid )
            triangleMesh.getAttrib( m_currentColorAttribHdl ).setData( m_baseColors );
        else
        {
            triangleMesh.removeAttrib( m_currentColorAttribHdl );
            CORE_ASSERT( !triangleMesh.isValid( m_currentColorAttribHdl ),
                         "Color attrib should be invalid now" );
        }
    }
}

void MeshPaintComponent::bakePaintToDiffuse() {
    // internally, this function simply overwrite the base colors with the current color attrib

    if ( !getRoMgr()->exists( *m_renderObjectReader() ) ) { return; }
    CORE_ASSERT( m_mesh != nullptr, "We should be have a Mesh storing a TriangleMesh here" );

    ON_DEBUG( auto colAttrib =
                  m_mesh->getCoreGeometry().getAttribHandle<Ra::Core::Vector4>( colAttribName ) );
    CORE_ASSERT( colAttrib == m_currentColorAttribHdl, "Inconsistent AttribHandle used" );
    // Could also be accessed using
    // auto triangleMesh = compMess->get<Ra::Core::Geometry::TriangleMesh>( getEntity(),
    // m_dataId ); however here we skip the search in the component map
    Ra::Core::Geometry::TriangleMesh& triangleMesh = m_mesh->getCoreGeometry();
    m_baseColors       = triangleMesh.getAttrib( m_currentColorAttribHdl ).data();
    m_isBaseColorValid = true;
}

void MeshPaintComponent::paintMesh( const Ra::Engine::Rendering::Renderer::PickingResult& picking,
                                    const Ra::Core::Utils::Color& color ) {
    using Ra::Engine::Data::Mesh;
    if ( !getRoMgr()->exists( *m_renderObjectReader() ) ) { return; }
    CORE_ASSERT( m_mesh != nullptr, "We should be have a Mesh storing a TriangleMesh here" );

    // check it's for us
    if ( *m_renderObjectReader() != picking.getRoIdx() ||
         picking.getMode() == Ra::Engine::Rendering::Renderer::RO )
    { return; }
    auto pickingRenderMode = m_mesh->pickingRenderMode();

    if ( pickingRenderMode == Ra::Engine::Data::Displayable::NO_PICKING ) { return; }

    if ( ( m_mesh->getRenderMode() == Ra::Engine::Data::Mesh::RM_LINES ||
           m_mesh->getRenderMode() == Ra::Engine::Data::Mesh::RM_LINE_LOOP ||
           m_mesh->getRenderMode() == Ra::Engine::Data::Mesh::RM_LINE_STRIP ||
           m_mesh->getRenderMode() == Ra::Engine::Data::Mesh::RM_LINES_ADJACENCY ||
           m_mesh->getRenderMode() == Ra::Engine::Data::Mesh::RM_LINE_STRIP_ADJACENCY ||
           m_mesh->getRenderMode() == Ra::Engine::Data::Mesh::RM_TRIANGLE_STRIP ||
           m_mesh->getRenderMode() == Ra::Engine::Data::Mesh::RM_TRIANGLE_FAN ) )
    { return; }
    if ( pickingRenderMode == Ra::Engine::Data::Displayable::PKM_POINTS &&
         ( picking.getMode() != Ra::Engine::Rendering::Renderer::VERTEX &&
           picking.getMode() != Ra::Engine::Rendering::Renderer::C_VERTEX ) )
    { return; }

    // Could also be accessed using
    // auto triangleMesh = compMess->get<Ra::Core::Geometry::TriangleMesh>( getEntity(),
    // m_dataId ); however here we skip the search in the component map

    const auto& t = m_mesh->getCoreGeometry().getIndices();
    ON_DEBUG( auto colAttrib =
                  m_mesh->getCoreGeometry().getAttribHandle<Ra::Core::Vector4>( colAttribName ) );
    CORE_ASSERT( colAttrib == m_currentColorAttribHdl, "Inconsistent AttribHandle used" );

    auto& colorAttrib    = m_mesh->getCoreGeometry().getAttrib( m_currentColorAttribHdl );
    auto& colorContainer = colorAttrib.getDataWithLock();

    picking.removeDuplicatedIndices();

    switch ( picking.getMode() )
    {
    case Ra::Engine::Rendering::Renderer::VERTEX:
        [[fallthrough]];
    case Ra::Engine::Rendering::Renderer::C_VERTEX: {
        if ( pickingRenderMode == Ra::Engine::Data::Displayable::PKM_POINTS )
        {
            for ( const auto& [f, v, e] : picking.getIndices() )
            {
                colorContainer[size_t( v )] = color;
            }
        }
        else
        {
            for ( const auto& [f, v, e] : picking.getIndices() )
            {
                const auto& elt               = t[size_t( f )]( v );
                colorContainer[size_t( elt )] = color;
            }
        }
        m_mesh->setDirty( Mesh::VERTEX_COLOR );
        break;
    }
    case Ra::Engine::Rendering::Renderer::EDGE:
        [[fallthrough]];
    case Ra::Engine::Rendering::Renderer::C_EDGE: {
        for ( const auto& [f, v, e] : picking.getIndices() )
        {
            const auto& elt    = t[size_t( f )];
            size_t v1          = elt( ( e + 1 ) % 3 );
            size_t v2          = elt( ( e + 2 ) % 3 );
            colorContainer[v1] = colorContainer[v2] = color;
        }

        break;
    }
    case Ra::Engine::Rendering::Renderer::TRIANGLE:
        [[fallthrough]];
    case Ra::Engine::Rendering::Renderer::C_TRIANGLE: {
        for ( const auto& [f, v, e] : picking.getIndices() )
        {
            const auto& elt    = t[size_t( f )];
            size_t v1          = elt( 0 );
            size_t v2          = elt( 1 );
            size_t v3          = elt( 2 );
            colorContainer[v1] = colorContainer[v2] = colorContainer[v3] = color;
        }

        break;
    }
    default:
        break;
    }
    colorAttrib.unlock();
}

} // namespace MeshPaintPlugin
