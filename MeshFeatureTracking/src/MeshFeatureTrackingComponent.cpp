#include <MeshFeatureTrackingComponent.hpp>

#include <Core/Geometry/MeshPrimitives.hpp>
#include <Core/Geometry/TriangleMesh.hpp>
#include <Core/Utils/Log.hpp>

#include <Engine/Data/BlinnPhongMaterial.hpp>
#include <Engine/Data/Material.hpp>
#include <Engine/Data/Mesh.hpp>
#include <Engine/RadiumEngine.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/ComponentMessenger.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

constexpr Scalar POINT_SCALE_FACTOR = 1_ra / 500;

using Ra::Engine::Scene::ComponentMessenger;
using MeshRenderMode = Ra::Engine::Data::Mesh::MeshRenderMode;
using PickingMode    = Ra::Engine::Rendering::Renderer::PickingMode;
using Ra::Core::Utils::TLogLevel;

namespace MeshFeatureTrackingPlugin {
MeshFeatureTrackingComponent::MeshFeatureTrackingComponent( const std::string& name ) :
    Component( name, Ra::Engine::Scene::SystemEntity::getInstance() ), m_pickedMesh( nullptr ) {
    m_data.m_mode = Ra::Engine::Rendering::Renderer::PickingMode::RO;
}

MeshFeatureTrackingComponent::~MeshFeatureTrackingComponent() {}

void MeshFeatureTrackingComponent::initialize() {
    std::shared_ptr<Ra::Engine::Data::Mesh> display(
        new Ra::Engine::Data::Mesh( "PickingManagerSphere" ) );
    display->loadGeometry( Ra::Core::Geometry::makeParametricSphere( 1_ra ) );
    std::shared_ptr<Ra::Engine::Data::Material> material;
    auto bpMaterial  = new Ra::Engine::Data::BlinnPhongMaterial( "PickingManageSphereMaterial" );
    bpMaterial->m_kd = Ra::Core::Utils::Color::Green();
    material.reset( bpMaterial );
    m_RO = Ra::Engine::Rendering::RenderObject::createRenderObject(
        "FeaturePickingManagerSphereRO",
        this,
        Ra::Engine::Rendering::RenderObjectType::Geometry,
        display,
        Ra::Engine::Rendering::RenderTechnique::createDefaultRenderTechnique() );
    m_RO->setMaterial( material );
    m_RO->setPickable( false );
    m_RO->setVisible( false );
    addRenderObject( m_RO );
}

void MeshFeatureTrackingComponent::setPosition( Ra::Core::Vector3 position ) {
    Ra::Core::Translation aa( position );
    Ra::Core::Transform rot( aa );
    m_RO->setLocalTransform( rot );
}

void MeshFeatureTrackingComponent::setScale( Scalar scale ) {
    auto T = m_RO->getLocalTransform();
    m_RO->setLocalTransform( T.scale( scale ) );
}

int MeshFeatureTrackingComponent::getMaxV() const {
    if ( m_data.m_mode != PickingMode::RO && getRoMgr()->exists( m_pickedRoIdx ) )
    { return int( m_pickedMesh->getNumVertices() ); }
    return 0;
}

int MeshFeatureTrackingComponent::getMaxT() const {
    if ( m_data.m_mode != PickingMode::RO && getRoMgr()->exists( m_pickedRoIdx ) )
    { return int( m_pickedMesh->getNumFaces() ); }
    return 0;
}

namespace { // anonymous namespace for line mesh indices retrieval from triangles -- according to
            // the render mode
// returns the triangles where the vertices can be found, along with the corresponding vertex
// indices within them.
void getPos_L2T( uint l, uint& v0, uint& v1, uint& t0, uint& t1 ) {
    v0 = 0;
    v1 = 1;
    t0 = l / 3 * 2;
    t1 = t0;
    if ( l % 3 == 1 )
    {
        v0 = 2;
        v1 = 0;
        ++t1;
    }
    else if ( l % 3 == 2 )
    {
        v0 = 1;
        v1 = 2;
        ++t0;
        ++t1;
    }
}
void getPos_L2T_strip( uint l, uint& v0, uint& v1, uint& t0, uint& t1 ) {
    v0 = 0;
    v1 = 1;
    t0 = l / 3;
    t1 = t0;
    if ( l % 3 == 1 )
    {
        ++v0;
        ++v1;
    }
    else if ( l % 3 == 2 )
    {
        v0 = 2;
        v1 = 0;
        ++t1;
    }
}
void getPos_L2T_adjacency( uint l, uint& v0, uint& v1, uint& t0, uint& t1 ) {
    v0 = 1;
    v1 = 2;
    t0 = ( l * 4 ) / 3;
    t1 = t0;
    if ( l % 3 == 1 )
    {
        v0 = 2;
        v1 = 0;
        ++t1;
    }
    else if ( l % 3 == 2 )
    {
        v0 = 0;
        v1 = 1;
        ++t0;
        ++t1;
    }
}
void getPos_L2T_strip_adjacency( uint l, uint& v0, uint& v1, uint& t0, uint& t1 ) {
    v0 = 0;
    v1 = 1;
    t0 = ( l + 1 ) / 3;
    t1 = t0;
    if ( l % 3 == 0 )
    {
        ++v0;
        ++v1;
    }
    else if ( l % 3 == 1 )
    {
        v0 = 2;
        v1 = 0;
        ++t1;
    }
}

void getPos_TS2T( uint ts, uint& v0, uint& v1, uint& v2, uint& t0, uint& t1, uint& t2 ) {
    v0 = 0;
    v1 = 1;
    v2 = 2;
    t0 = ts / 3;
    t1 = t0;
    t2 = t0;
    if ( ts % 3 == 1 )
    {
        v0 = 1;
        v1 = 2;
        v2 = 0;
        ++t2;
    }
    else if ( ts % 3 == 2 )
    {
        v0 = 2;
        v1 = 0;
        v2 = 1;
        ++t1;
        ++t2;
    }
}

// for fans, first triangle vertex is always triangle[0](0)
void getPos_TF2T( uint tf, uint& v1, uint& v2, uint& t1, uint& t2 ) {
    v1 = 0;
    v2 = 1;
    t1 = ( tf + 1 ) / 3;
    t2 = t1;
    if ( tf % 3 == 0 )
    {
        v1 = 1;
        v2 = 2;
    }
    else if ( tf % 3 == 1 )
    {
        v1 = 2;
        v2 = 0;
        ++t2;
    }
}
} // namespace

void MeshFeatureTrackingComponent::setData(
    const Ra::Engine::Rendering::Renderer::PickingResult& data ) {
    using Ra::Engine::Data::Displayable;

    if ( !getRoMgr()->exists( data.getRoIdx() ) )
    {
        m_data.m_mode = PickingMode::RO;
        return;
    }

    {
        auto ro                      = getRoMgr()->getRenderObject( data.getRoIdx() );
        Ra::Engine::Data::Mesh* mesh = dynamic_cast<Ra::Engine::Data::Mesh*>( ro->getMesh().get() );
        if ( mesh == nullptr )
        {
            LOG( TLogLevel::logWARNING ) << "MeshFeatureTracking is available only for meshes";
            return;
        }

        // We set both objects at once, they should remain consistent while m_pickedRoIdx is valid
        m_pickedRoIdx = data.getRoIdx();
        m_pickedMesh  = mesh;
    }
    // check picking mode / render config OK
    if ( data.getMode() == PickingMode::RO || data.getMode() > PickingMode::TRIANGLE ) { return; }
    m_data.m_mode = data.getMode();
    m_data.m_data = {-1, -1, -1, -1};

    auto rm = m_pickedMesh->getRenderMode();
    if ( rm == MeshRenderMode::RM_POINTS && m_data.m_mode != PickingMode::VERTEX )
    {
        m_data.m_mode = PickingMode::RO;
        return;
    }
    if ( ( rm == MeshRenderMode::RM_LINES || rm == MeshRenderMode::RM_LINE_LOOP ||
           rm == MeshRenderMode::RM_LINE_STRIP || rm == MeshRenderMode::RM_LINES_ADJACENCY ||
           rm == MeshRenderMode::RM_LINE_STRIP_ADJACENCY ) &&
         m_data.m_mode == PickingMode::TRIANGLE )
    {
        m_data.m_mode = PickingMode::RO;
        return;
    }
    // fill data accordingly
    auto tuple = uint( data.getIndices()[0] );
    if ( rm == MeshRenderMode::RM_POINTS )
    {
        m_data.m_data[0] = int( element );
        return;
    }
    // if lines, retrieve triangle-based indices
    const auto& t = m_pickedMesh->getCoreGeometry().getIndices();
    if ( rm == MeshRenderMode::RM_LINES )
    {
        uint v0, v1, t0, t1;
        getPos_L2T( element, v0, v1, t0, t1 );
        if ( m_data.m_mode == PickingMode::VERTEX )
        {
            m_data.m_data[0] = data.m_vertexIdx[0] == 0 ? t[t0]( v0 ) : t[t1]( v1 );
            m_data.m_data[1] = data.m_vertexIdx[0] == 0 ? t[t1]( v1 ) : t[t0]( v0 );
        }
        else
        {
            m_data.m_data[0] = t[t0]( v0 );
            m_data.m_data[1] = t[t1]( v1 );
        }
        return;
    }
    if ( rm == MeshRenderMode::RM_LINE_LOOP || rm == MeshRenderMode::RM_LINE_STRIP )
    {
        uint v0, v1, t0, t1;
        getPos_L2T_strip( element, v0, v1, t0, t1 );
        if ( m_data.m_mode == PickingMode::VERTEX )
        {
            m_data.m_data[0] = data.m_vertexIdx[0] == 0 ? t[t0]( v0 ) : t[t1]( v1 );
            m_data.m_data[1] = data.m_vertexIdx[0] == 0 ? t[t1]( v1 ) : t[t0]( v0 );
        }
        else
        {
            m_data.m_data[0] = t[t0]( v0 );
            m_data.m_data[1] = t[t1]( v1 );
        }
        return;
    }
    if ( rm == MeshRenderMode::RM_LINES_ADJACENCY )
    {
        uint v0, v1, t0, t1;
        getPos_L2T_adjacency( element, v0, v1, t0, t1 );
        if ( m_data.m_mode == PickingMode::VERTEX )
        {
            m_data.m_data[0] = data.m_vertexIdx[0] == 0 ? t[t0]( v0 ) : t[t1]( v1 );
            m_data.m_data[1] = data.m_vertexIdx[0] == 0 ? t[t1]( v1 ) : t[t0]( v0 );
        }
        else
        {
            m_data.m_data[0] = t[t0]( v0 );
            m_data.m_data[1] = t[t1]( v1 );
        }
        return;
    }
    if ( rm == MeshRenderMode::RM_LINE_STRIP_ADJACENCY )
    {
        uint v0, v1, t0, t1;
        getPos_L2T_strip_adjacency( element, v0, v1, t0, t1 );
        if ( m_data.m_mode == PickingMode::VERTEX )
        {
            m_data.m_data[0] = data.m_vertexIdx[0] == 0 ? t[t0]( v0 ) : t[t1]( v1 );
            m_data.m_data[1] = data.m_vertexIdx[0] == 0 ? t[t1]( v1 ) : t[t0]( v0 );
        }
        else
        {
            m_data.m_data[0] = t[t0]( v0 );
            m_data.m_data[1] = t[t1]( v1 );
        }
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLES && m_data.m_mode == PickingMode::VERTEX )
    {
        m_data.m_data[0] = t[element]( data.m_vertexIdx[0] );
        m_data.m_data[1] = t[element]( ( data.m_vertexIdx[0] + 1 ) % 3 );
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLES && m_data.m_mode == PickingMode::EDGE )
    {
        m_data.m_data[0] = t[element]( ( data.m_edgeIdx[0] + 1 ) % 3 );
        m_data.m_data[1] = t[element]( ( data.m_edgeIdx[0] + 2 ) % 3 );
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLES )
    {
        m_data.m_data[0] = t[element]( 0 );
        m_data.m_data[1] = t[element]( 1 );
        m_data.m_data[2] = t[element]( 2 );
        m_data.m_data[3] = data.m_elementIdx[0];
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_STRIP && m_data.m_mode == PickingMode::VERTEX )
    {
        uint v0, v1, v2, t0, t1, t2;
        getPos_TS2T( element, v0, v1, v2, t0, t1, t2 );
        m_data.m_data[0] = data.m_vertexIdx[0] == 0
                               ? t[t0]( v0 )
                               : ( data.m_vertexIdx[0] == 1 ? t[t1]( v1 ) : t[t2]( v2 ) );
        m_data.m_data[1] = data.m_vertexIdx[0] == 0 ? t[t1]( v1 ) : t[t0]( v0 );
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_STRIP && m_data.m_mode == PickingMode::EDGE )
    {
        uint v0, v1, v2, t0, t1, t2;
        getPos_TS2T( element, v0, v1, v2, t0, t1, t2 );
        m_data.m_data[0] = data.m_edgeIdx[0] == 0 ? t[t1]( v1 ) : t[t0]( v0 );
        m_data.m_data[1] = data.m_edgeIdx[0] == 2 ? t[t1]( v1 ) : t[t2]( v2 );
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_STRIP )
    {
        uint v0, v1, v2, t0, t1, t2;
        getPos_TS2T( element, v0, v1, v2, t0, t1, t2 );
        m_data.m_data[0] = t[t0]( v0 );
        m_data.m_data[1] = t[t1]( v1 );
        m_data.m_data[2] = t[t2]( v2 );
        m_data.m_data[3] = element;
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_FAN && m_data.m_mode == PickingMode::VERTEX )
    {
        uint v1, v2, t1, t2;
        getPos_TF2T( element, v1, v2, t1, t2 );
        m_data.m_data[0] = data.m_vertexIdx[0] == 0
                               ? t[0]( 0 )
                               : ( data.m_vertexIdx[0] == 1 ? t[t1]( v1 ) : t[t2]( v2 ) );
        m_data.m_data[1] = data.m_vertexIdx[0] == 0 ? t[t1]( v1 ) : t[0]( 0 );
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_FAN && m_data.m_mode == PickingMode::EDGE )
    {
        uint v1, v2, t1, t2;
        getPos_TF2T( element, v1, v2, t1, t2 );
        m_data.m_data[0] = data.m_edgeIdx[0] == 0 ? t[t1]( v1 ) : t[0]( 0 );
        m_data.m_data[1] = data.m_edgeIdx[0] == 2 ? t[t1]( v1 ) : t[t2]( v2 );
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_FAN )
    {
        uint v1, v2, t1, t2;
        getPos_TF2T( element, v1, v2, t1, t2 );
        m_data.m_data[0] = t[0]( 0 );
        m_data.m_data[1] = t[t1]( v1 );
        m_data.m_data[2] = t[t2]( v2 );
        m_data.m_data[3] = element;
        return;
    }
}

void MeshFeatureTrackingComponent::setVertexIdx( uint idx ) {
    if ( !getRoMgr()->exists( m_pickedRoIdx ) ) { return; }
    CORE_ASSERT( m_pickedMesh != nullptr, "m_pickedRoIdx and m_pickedMesh are inconsistent" );

    m_data.m_data[0] = idx;
    // also need to change second for feature Scale
    const auto rm = m_pickedMesh->getRenderMode();
    if ( rm == MeshRenderMode::RM_POINTS ) { return; }
    const auto& triangles = m_pickedMesh->getCoreGeometry().getIndices();
    if ( rm == MeshRenderMode::RM_LINES )
    {
        for ( uint i = 0; i < triangles.size(); ++i )
        {
            const auto& t = triangles[i];
            if ( t( 0 ) == idx )
            {
                m_data.m_data[1] = ( i % 2 == 0 ? t( 1 ) : triangles[i - 1]( 2 ) );
                return;
            }
            if ( t( 1 ) == idx )
            {
                m_data.m_data[1] = ( i % 2 == 0 ? t( 0 ) : t( 2 ) );
                return;
            }
            if ( t( 2 ) == idx )
            {
                m_data.m_data[1] = ( i % 2 == 0 ? triangles[i + 1]( 0 ) : t( 1 ) );
                return;
            }
        }
    }
    if ( rm == MeshRenderMode::RM_LINE_LOOP || rm == MeshRenderMode::RM_LINE_STRIP )
    {
        for ( uint i = 0; i < triangles.size(); ++i )
        {
            const auto& t = triangles[i];
            if ( t( 0 ) == idx )
            {
                m_data.m_data[1] = t( 1 );
                return;
            }
            if ( t( 1 ) == idx )
            {
                m_data.m_data[1] = t( 0 );
                return;
            }
            if ( t( 2 ) == idx )
            {
                m_data.m_data[1] = t( 1 );
                return;
            }
        }
    }
    if ( rm == MeshRenderMode::RM_LINES_ADJACENCY )
    {
        for ( uint i = 0; i < triangles.size(); ++i )
        {
            const auto& t = triangles[i];
            if ( t( 0 ) == idx && i % 4 > 1 )
            {
                m_data.m_data[1] = ( i % 4 == 2 ? triangles[i - 1]( 2 ) : t( 1 ) );
                return;
            }
            if ( t( 1 ) == idx && ( i + 3 ) % 4 > 1 )
            {
                m_data.m_data[1] = ( i % 4 == 0 ? t( 2 ) : t( 0 ) );
                return;
            }
            if ( t( 2 ) == idx && i % 4 < 2 )
            {
                m_data.m_data[1] = ( i % 4 == 0 ? t( 1 ) : triangles[i + 1]( 0 ) );
                return;
            }
        }
    }
    if ( rm == MeshRenderMode::RM_LINE_STRIP_ADJACENCY )
    {
        for ( uint i = 0; i < triangles.size(); ++i )
        {
            const auto& t = triangles[i];
            if ( t( 0 ) == idx && i != 0 )
            {
                m_data.m_data[1] = t( 1 );
                return;
            }
            if ( t( 1 ) == idx && i != 0 )
            {
                m_data.m_data[1] = t( 0 );
                return;
            }
            if ( t( 2 ) == idx )
            {
                m_data.m_data[1] = t( 1 );
                return;
            }
        }
    }
    if ( rm == MeshRenderMode::RM_TRIANGLES || rm == MeshRenderMode::RM_TRIANGLE_STRIP )
    {
        for ( uint i = 0; i < triangles.size(); ++i )
        {
            const auto& t = triangles[i];
            if ( t( 0 ) == idx )
            {
                m_data.m_data[1] = t( 1 );
                return;
            }
            if ( t( 1 ) == idx || t( 2 ) == idx )
            {
                m_data.m_data[1] = t( 0 );
                return;
            }
        }
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_FAN )
    {
        m_data.m_data[1] = ( idx == 0 ? triangles[0]( 1 ) : 0 );
        return;
    }
}

void MeshFeatureTrackingComponent::setTriangleIdx( uint idx ) {
    if ( !getRoMgr()->exists( m_pickedRoIdx ) ) { return; }
    CORE_ASSERT( m_pickedMesh != nullptr, "m_pickedRoIdx and m_pickedMesh are inconsistent" );

    // also need to change all for feature Scale, Position and Vector
    const auto rm         = m_pickedMesh->getRenderMode();
    const auto& triangles = m_pickedMesh->getCoreGeometry().getIndices();
    if ( rm == MeshRenderMode::RM_TRIANGLES )
    {
        const auto& t    = triangles[idx];
        m_data.m_data[0] = t( 0 );
        m_data.m_data[1] = t( 1 );
        m_data.m_data[2] = t( 2 );
        m_data.m_data[3] = idx;
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_STRIP )
    {
        uint v0, v1, v2, t0, t1, t2;
        getPos_TS2T( idx, v0, v1, v2, t0, t1, t2 );
        m_data.m_data[0] = triangles[t0]( v0 );
        m_data.m_data[1] = triangles[t1]( v1 );
        m_data.m_data[2] = triangles[t2]( v2 );
        m_data.m_data[3] = idx;
        return;
    }
    if ( rm == MeshRenderMode::RM_TRIANGLE_FAN )
    {
        uint v1, v2, t1, t2;
        getPos_TF2T( idx, v1, v2, t1, t2 );
        m_data.m_data[0] = triangles[0]( 0 );
        m_data.m_data[1] = triangles[t1]( v1 );
        m_data.m_data[2] = triangles[t2]( v2 );
        m_data.m_data[3] = idx;
        return;
    }
}

void MeshFeatureTrackingComponent::update() {
    // check supported picking mode
    if ( m_data.m_mode != PickingMode::RO && m_data.m_mode <= PickingMode::TRIANGLE &&
         getRoMgr()->exists( m_pickedRoIdx ) )
    {
        CORE_ASSERT( m_pickedMesh != nullptr, "m_pickedRoIdx and m_pickedMesh are inconsistent" );

        setPosition( getFeaturePosition() );
        setScale( getFeatureScale() );
        m_RO->setVisible( true );
    }
    else
    { m_RO->setVisible( false ); }
}

FeatureData MeshFeatureTrackingComponent::getFeatureData() const {
    return m_data;
}

Scalar MeshFeatureTrackingComponent::getFeatureScale() const {
    if ( !getRoMgr()->exists( m_pickedRoIdx ) ) { return 1_ra; }
    CORE_ASSERT( m_pickedMesh != nullptr, "m_pickedRoIdx and m_pickedMesh are inconsistent" );

    // check supported picking mode
    if ( m_data.m_mode == PickingMode::RO ) { return 1_ra; }

    // manage picking mode
    auto ro = getRoMgr()->getRenderObject( m_pickedRoIdx );
    if ( m_pickedMesh->getRenderMode() == MeshRenderMode::RM_POINTS )
    { return ro->computeAabb().sizes().norm() * POINT_SCALE_FACTOR; }
    const auto& T = ro->getTransform().linear();
    const auto& v = m_pickedMesh->getCoreGeometry().vertices();
    switch ( m_data.m_mode )
    {
    case PickingMode::VERTEX: {
        // return 1 fourth of the edge length of the first edge we can find with the vertex
        return ( T * ( v[m_data.m_data[1]] - v[m_data.m_data[0]] ) ).norm() / 4_ra;
        ;
    }
    case PickingMode::EDGE: {
        // return 1 fourth of the edge length
        return ( T * ( v[m_data.m_data[1]] - v[m_data.m_data[0]] ) ).norm() / 4_ra;
    }
    case PickingMode::TRIANGLE: {
        // return half the smallest distance from C to an edge
        const Ra::Core::Vector3& V0 = v[m_data.m_data[0]];
        const Ra::Core::Vector3& V1 = v[m_data.m_data[1]];
        const Ra::Core::Vector3& V2 = v[m_data.m_data[2]];
        const Ra::Core::Vector3 C   = ( V0 + V1 + V2 ) / 3_ra;
        const Ra::Core::Vector3 C0  = C - V0;
        const Ra::Core::Vector3 C1  = C - V1;
        const Ra::Core::Vector3 C2  = C - V2;
        return sqrt( std::min(
                   std::min( C0.squaredNorm() *
                                 ( V1 - V0 ).normalized().cross( C0.normalized() ).squaredNorm(),
                             C1.squaredNorm() *
                                 ( V2 - V1 ).normalized().cross( C1.normalized() ).squaredNorm() ),
                   C2.squaredNorm() *
                       ( V0 - V2 ).normalized().cross( C2.normalized() ).squaredNorm() ) ) /
               2_ra;
    }
    default:
        return 1_ra;
    }
}

Ra::Core::Vector3 MeshFeatureTrackingComponent::getFeaturePosition() const {
    if ( !getRoMgr()->exists( m_pickedRoIdx ) ) { return Ra::Core::Vector3(); }
    CORE_ASSERT( m_pickedMesh != nullptr, "m_pickedRoIdx and m_pickedMesh are inconsistent" );

    // check supported picking mode
    if ( m_data.m_mode == PickingMode::RO ) { return Ra::Core::Vector3(); }

    // manage picking mode
    const auto& v = m_pickedMesh->getCoreGeometry().vertices();
    Ra::Core::Vector3 P( 0, 0, 0 );
    switch ( m_data.m_mode )
    {
    case PickingMode::VERTEX: {
        P = v[m_data.m_data[0]];
        break;
    }
    case PickingMode::EDGE: {
        P = ( v[m_data.m_data[0]] + v[m_data.m_data[1]] ) / 2_ra;
        break;
    }
    case PickingMode::TRIANGLE: {
        P = ( v[m_data.m_data[0]] + v[m_data.m_data[1]] + v[m_data.m_data[2]] ) / 3_ra;
        break;
    }
    default:
        break;
    }

    // deal with transformations
    auto ro = getRoMgr()->getRenderObject( m_pickedRoIdx );
    return ro->getTransform() * P;
}

Ra::Core::Vector3 MeshFeatureTrackingComponent::getFeatureVector() const {
    if ( !getRoMgr()->exists( m_pickedRoIdx ) ) { return Ra::Core::Vector3(); }
    CORE_ASSERT( m_pickedMesh != nullptr, "m_pickedRoIdx and m_pickedMesh are inconsistent" );

    // check supported picking mode
    if ( m_data.m_mode == PickingMode::RO ) { return Ra::Core::Vector3(); }

    // manage picking mode
    const auto& n = m_pickedMesh->getCoreGeometry().normals();
    if ( m_data.m_mode == PickingMode::VERTEX )
    {
        if ( !n.empty() ) { return n[m_data.m_data[0]]; }
        return Ra::Core::Vector3();
    }
    const auto& v = m_pickedMesh->getCoreGeometry().vertices();
    Ra::Core::Vector3 V( 0, 0, 0 );
    switch ( m_data.m_mode )
    {
    case PickingMode::EDGE: {
        // for edges, the edge vector
        V = v[m_data.m_data[0]] - v[m_data.m_data[1]];
        break;
    }
    case PickingMode::TRIANGLE: {
        // for triangles, the normal
        const Ra::Core::Vector3& p0 = v[m_data.m_data[0]];
        const Ra::Core::Vector3& p1 = v[m_data.m_data[1]];
        const Ra::Core::Vector3& p2 = v[m_data.m_data[2]];
        V                           = ( p1 - p0 ).cross( p2 - p0 ).normalized();
        break;
    }
    default:
        break;
    }

    // deal with transformations
    auto ro = getRoMgr()->getRenderObject( m_pickedRoIdx );
    return ( ro->getTransformAsMatrix().inverse().transpose() *
             Ra::Core::Vector4( V( 0 ), V( 1 ), V( 2 ), 0_ra ) )
        .head<3>();
}

} // namespace MeshFeatureTrackingPlugin
