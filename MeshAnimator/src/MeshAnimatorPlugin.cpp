#include <MeshAnimatorPlugin.hpp>

#include <QAction>
#include <QIcon>
#include <QToolBar>

#include <Core/Animation/KeyFramedValueInterpolators.hpp>
#include <Core/Utils/Log.hpp>

#include <Engine/Component/Component.hpp>
#include <Engine/Managers/SignalManager/SignalManager.hpp>
#include <Engine/RadiumEngine.hpp>
#include <Engine/Renderer/Mesh/Mesh.hpp>
#include <Engine/Renderer/RenderObject/RenderObject.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>

#include <GuiBase/SelectionManager/SelectionManager.hpp>
#include <GuiBase/Timeline/Timeline.h>
#include <GuiBase/Utils/PickingManager.hpp>

#include <MeshAnimatorSystem.hpp>

#include "ui_MeshAnimatorUI.h"
#include <UI/MeshAnimatorUI.h>

using namespace Ra::Core::Utils;

namespace MeshAnimatorPlugin {

MeshAnimatorPluginC::MeshAnimatorPluginC() {}

MeshAnimatorPluginC::~MeshAnimatorPluginC() {
    delete m_system;
    for ( auto kf : m_positionKeyframes )
    {
        for ( auto f : kf.second )
        {
            delete f.second.first;
        }
    }
    for ( auto kf : m_normalKeyframes )
    {
        for ( auto f : kf.second )
        {
            delete f.second.first;
        }
    }
}

void MeshAnimatorPluginC::registerPlugin( const Ra::Plugins::Context& context ) {
    // register system
    m_system = new MeshAnimatorSystem( this );
    context.m_engine->registerSystem( "MeshAnimatorSystem", m_system );
    // register selection context
    m_selectionManager = context.m_selectionManager;
    m_PickingManager   = context.m_pickingManager;
    m_timeline         = context.m_timeline;
    connect( m_selectionManager,
             &Ra::GuiBase::SelectionManager::currentChanged,
             this,
             &MeshAnimatorPluginC::onCurrentChanged );
}

bool MeshAnimatorPluginC::doAddWidget( QString& name ) {
    name = "MeshAnimator";
    return true;
}

QWidget* MeshAnimatorPluginC::getWidget() {
    if ( m_widget == nullptr )
    {
        m_widget = new MeshAnimatorUI();
        connect( m_widget,
                 &MeshAnimatorUI::indexChanged,
                 this,
                 &MeshAnimatorPluginC::indexChanged );
        connect( m_widget,
                 &MeshAnimatorUI::animatePosition,
                 this,
                 &MeshAnimatorPluginC::animatePosition );
        connect( m_widget,
                 &MeshAnimatorUI::animateNormal,
                 this,
                 &MeshAnimatorPluginC::animateNormal );
    }
    return m_widget;
}

bool MeshAnimatorPluginC::doAddMenu() {
    return false;
}

QMenu* MeshAnimatorPluginC::getMenu() {
    return nullptr;
}

bool MeshAnimatorPluginC::doAddAction( int& nb ) {
    nb = 0;
    return false;
}

QAction* MeshAnimatorPluginC::getAction( int /*id*/ ) {
    return nullptr;
}

void MeshAnimatorPluginC::onCurrentChanged( const QModelIndex& /*current*/,
                                            const QModelIndex& /*prev*/ ) {
    using PickingMode = Ra::Engine::Renderer::PickingMode;
    using RenderMode  = Ra::Engine::Mesh::MeshRenderMode;
    auto pick         = m_PickingManager->getCurrent();
    m_currentRO       = pick.m_roIdx;
    m_currentVertex   = -1;
    if ( pick.m_mode == PickingMode::VERTEX )
    {
        auto roMgr = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();
        if ( roMgr->exists( m_currentRO ) )
        {
            auto ro                = roMgr->getRenderObject( m_currentRO );
            Ra::Engine::Mesh* mesh = dynamic_cast<Ra::Engine::Mesh*>( ro->getMesh().get() );
            if ( mesh )
            {
                auto rm = mesh->getRenderMode();
                switch ( rm )
                {
                case RenderMode::RM_POINTS:
                { m_currentVertex = pick.m_elementIdx[0]; }
                break;
                case RenderMode::RM_TRIANGLES:
                {
                    m_currentVertex = mesh->getCoreGeometry().m_indices[pick.m_elementIdx[0]][pick.m_vertexIdx[0]];
                }
                break;
                default:
                {
                    LOG( logWARNING ) << "[MeshAnimatorPlugin] Mesh::RenderMode "
                                      << rm << " not handled yet.";
                }
                break;
                }
            }
        }
    }
    m_component = nullptr;
    if ( m_selectionManager->hasSelection() )
    { m_component = m_selectionManager->currentItem().m_component; }
    update();
}

void MeshAnimatorPluginC::update() {
    if ( m_widget == nullptr ) { return; }
    auto roMgr = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();
    if ( roMgr->exists( m_currentRO ) && m_currentVertex != -1 )
    {
        auto ro                = roMgr->getRenderObject( m_currentRO );
        Ra::Engine::Mesh* mesh = dynamic_cast<Ra::Engine::Mesh*>( ro->getMesh().get() );
        if ( mesh )
        {
            const auto& V = mesh->getCoreGeometry().vertices();
            const auto& N = mesh->getCoreGeometry().normals();
            m_widget->ui->m_vertexIdx->setEnabled( true );
            m_widget->ui->m_vertexIdx->setValue( int( m_currentVertex ) );
            m_widget->ui->m_vertexIdx->setMaximum( int( V.size() ) );
            m_widget->ui->m_position->setEnabled( true );
            bool framed = false;
            auto it_p   = m_positionKeyframes.find( m_currentRO );
            if ( it_p != m_positionKeyframes.end() )
            { framed = it_p->second.find( m_currentVertex ) != it_p->second.end(); }
            m_widget->ui->m_position->setChecked( framed );
            m_widget->ui->m_vertexPX->setText(
                QString::number( double( V[uint( m_currentVertex )]( 0 ) ) ) );
            m_widget->ui->m_vertexPY->setText(
                QString::number( double( V[uint( m_currentVertex )]( 1 ) ) ) );
            m_widget->ui->m_vertexPZ->setText(
                QString::number( double( V[uint( m_currentVertex )]( 2 ) ) ) );
            m_widget->ui->m_normal->setEnabled( true );
            framed    = false;
            auto it_n = m_normalKeyframes.find( m_currentRO );
            if ( it_n != m_normalKeyframes.end() )
            { framed = it_n->second.find( m_currentVertex ) != it_n->second.end(); }
            m_widget->ui->m_normal->setChecked( framed );
            m_widget->ui->m_vertexNX->setText(
                QString::number( double( N[uint( m_currentVertex )]( 0 ) ) ) );
            m_widget->ui->m_vertexNY->setText(
                QString::number( double( N[uint( m_currentVertex )]( 1 ) ) ) );
            m_widget->ui->m_vertexNZ->setText(
                QString::number( double( N[uint( m_currentVertex )]( 2 ) ) ) );
        }
        else
        {
            m_widget->ui->m_vertexIdx->setEnabled( false );
            m_widget->ui->m_vertexIdx->setMaximum( 0 );
            m_widget->ui->m_position->setEnabled( false );
            m_widget->ui->m_position->setChecked( false );
            m_widget->ui->m_vertexPX->setText( "" );
            m_widget->ui->m_vertexPY->setText( "" );
            m_widget->ui->m_vertexPZ->setText( "" );
            m_widget->ui->m_normal->setEnabled( false );
            m_widget->ui->m_normal->setChecked( false );
            m_widget->ui->m_vertexPX->setText( "" );
            m_widget->ui->m_vertexPY->setText( "" );
            m_widget->ui->m_vertexPZ->setText( "" );
            return;
        }
    }
    else
    {
        m_widget->ui->m_vertexIdx->setEnabled( false );
        m_widget->ui->m_vertexIdx->setMaximum( 0 );
        m_widget->ui->m_position->setEnabled( false );
        m_widget->ui->m_position->setChecked( false );
        m_widget->ui->m_vertexPX->setText( "" );
        m_widget->ui->m_vertexPY->setText( "" );
        m_widget->ui->m_vertexPZ->setText( "" );
        m_widget->ui->m_normal->setEnabled( false );
        m_widget->ui->m_normal->setChecked( false );
        m_widget->ui->m_vertexPX->setText( "" );
        m_widget->ui->m_vertexPY->setText( "" );
        m_widget->ui->m_vertexPZ->setText( "" );
        return;
    }
}

void MeshAnimatorPluginC::indexChanged( int index ) {
    m_currentVertex = index;
    update();
}

void MeshAnimatorPluginC::animatePosition( bool animate ) {
    using namespace Ra::Core::Animation;
    auto roMgr = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();
    if ( roMgr->exists( m_currentRO ) && m_currentVertex != -1 )
    {
        auto ro                = roMgr->getRenderObject( m_currentRO );
        Ra::Engine::Mesh* mesh = dynamic_cast<Ra::Engine::Mesh*>( ro->getMesh().get() );
        if ( mesh )
        {
            if ( animate )
            {
                auto& P = mesh->getCoreGeometry().vertices()[m_currentVertex];
                auto keyframedPos = new KeyFramedValue( P, 0, linearInterpolate<Ra::Core::Vector3> );
                m_positionKeyframes[m_currentRO][m_currentVertex] = {keyframedPos, P};
                m_timeline->registerKeyFramedValue(
                    m_component,
                    m_component->getName() + "_vertex" + std::to_string( m_currentVertex ) +
                        "_Position",
                    m_positionKeyframes[m_currentRO][m_currentVertex].first,
                    [keyframedPos, &P, mesh]( const Scalar& t ) {
                        keyframedPos->insertKeyFrame(
                            t, P + Ra::Core::Vector3{0.01_ra, 0.01_ra, 0.01_ra} );
                        mesh->setDirty( Ra::Engine::Mesh::VERTEX_POSITION );
                    },
                    [keyframedPos, this, mesh]( const Scalar& t ) {
                        mesh->getCoreGeometry().verticesWithLock()[m_currentVertex] = keyframedPos->at( t );
                        mesh->getCoreGeometry().verticesUnlock();
                    } );
            }
            else
            {
                mesh->getCoreGeometry().verticesWithLock()[m_currentVertex] =
                    m_positionKeyframes[m_currentRO][m_currentVertex].second;
                mesh->getCoreGeometry().verticesUnlock();
                delete m_positionKeyframes[m_currentRO][m_currentVertex].first;
                m_positionKeyframes[m_currentRO].erase( m_currentVertex );
                if ( m_positionKeyframes[m_currentRO].size() == 0 )
                { m_positionKeyframes.erase( m_currentRO ); }
                m_timeline->unregisterKeyFramedValue( m_component,
                                                      m_component->getName() + "_vertex" +
                                                          std::to_string( m_currentVertex ) +
                                                          "_Position" );
            }
            update();
        }
    }
}

void MeshAnimatorPluginC::animateNormal( bool animate ) {
    using namespace Ra::Core::Animation;
    auto roMgr = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();
    if ( roMgr->exists( m_currentRO ) && m_currentVertex != -1 )
    {
        auto ro                = roMgr->getRenderObject( m_currentRO );
        Ra::Engine::Mesh* mesh = dynamic_cast<Ra::Engine::Mesh*>( ro->getMesh().get() );
        if ( mesh )
        {
            if ( animate )
            {
                auto& N = mesh->getCoreGeometry().normals()[m_currentVertex];
                auto keyframedNor = new KeyFramedValue( N, 0, linearInterpolate<Ra::Core::Vector3> );
                m_normalKeyframes[m_currentRO][m_currentVertex] = {keyframedNor, N};
                m_timeline->registerKeyFramedValue(
                    m_component,
                    m_component->getName() + "_vertex" + std::to_string( m_currentVertex ) +
                        "_Normal",
                    m_normalKeyframes[m_currentRO][m_currentVertex].first,
                    [keyframedNor, &N, mesh]( const Scalar& t ) {
                        keyframedNor->insertKeyFrame(
                            t, N + Ra::Core::Vector3{-0.1_ra, -0.1_ra, -0.1_ra} );
                        mesh->setDirty( Ra::Engine::Mesh::VERTEX_NORMAL );
                    },
                    [keyframedNor, this, mesh]( const Scalar& t ) {
                        mesh->getCoreGeometry().normalsWithLock()[m_currentVertex] = keyframedNor->at( t );
                        mesh->getCoreGeometry().normalsUnlock();
                    } );
            }
            else
            {
                mesh->getCoreGeometry().normalsWithLock()[m_currentVertex] =
                    m_normalKeyframes[m_currentRO][m_currentVertex].second;
                mesh->getCoreGeometry().verticesUnlock();
                delete m_normalKeyframes[m_currentRO][m_currentVertex].first;
                m_normalKeyframes[m_currentRO].erase( m_currentVertex );
                if ( m_normalKeyframes[m_currentRO].size() == 0 )
                { m_normalKeyframes.erase( m_currentRO ); }
                m_timeline->unregisterKeyFramedValue( m_component,
                                                      m_component->getName() + "_vertex" +
                                                          std::to_string( m_currentVertex ) +
                                                          "_Normal" );
            }
            update();
        }
    }
}

} // namespace MeshAnimatorPlugin
