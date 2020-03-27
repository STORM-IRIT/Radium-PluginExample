#include <CameraManipPlugin.hpp>

#include <fstream>

#include <QAction>
#include <QFileDialog>
#include <QSettings>

#include <Core/Animation/KeyFramedValueInterpolators.hpp>

#include <Engine/ItemModel/ItemEntry.hpp>
#include <Engine/Managers/CameraManager/CameraManager.hpp>
#include <Engine/Managers/EntityManager/EntityManager.hpp>
#include <Engine/Renderer/Camera/Camera.hpp>

#include <GuiBase/SelectionManager/SelectionManager.hpp>
#include <GuiBase/Timeline/Timeline.h>
#include <GuiBase/Viewer/TrackballCameraManipulator.hpp>
#include <GuiBase/Viewer/FlightCameraManipulator.hpp>
#include <GuiBase/Viewer/Viewer.hpp>

#include "ui_CameraManipUI.h"
#include <UI/CameraManipUI.h>

namespace CameraManipPlugin {

using namespace Ra::Core::Utils; // log

CameraManipPluginC::CameraManipPluginC() :
    m_widget( nullptr ),
    m_engine( nullptr ),
    m_selectionManager( nullptr ),
    m_viewer( nullptr ) {}

CameraManipPluginC::~CameraManipPluginC() {}

void CameraManipPluginC::registerPlugin( const Ra::Plugins::Context& context ) {
    // register selection context
    m_engine           = context.m_engine;
    m_selectionManager = context.m_selectionManager;
    m_viewer           = context.m_viewer;
    m_timeline         = context.m_timeline;
    connect( m_selectionManager,
             &Ra::GuiBase::SelectionManager::currentChanged,
             this,
             &CameraManipPluginC::onCurrentChanged );
    connect( this,
             &CameraManipPluginC::askForUpdate,
             &context,
             &Ra::Plugins::Context::askForUpdate );
}

bool CameraManipPluginC::doAddWidget( QString& name ) {
    name = "CameraManip";
    return true;
}

QWidget* CameraManipPluginC::getWidget() {
    m_widget = new CameraManipUI();
    connect( m_widget->ui->m_useCamera,
             &QPushButton::clicked,
             this,
             &CameraManipPluginC::useSelectedCamera );
    connect( m_widget->ui->m_saveCamera,
             &QPushButton::clicked,
             this,
             &CameraManipPluginC::saveCamera );
    connect( m_widget->ui->m_keyFrameCamera,
             &QCheckBox::toggled,
             this,
             &CameraManipPluginC::keyFrameCamera );
    connect( m_widget->ui->m_createCamera,
             &QPushButton::clicked,
             this,
             &CameraManipPluginC::createCamera );
    return m_widget;
}

bool CameraManipPluginC::doAddMenu() {
    return false;
}

QMenu* CameraManipPluginC::getMenu() {
    return nullptr;
}

bool CameraManipPluginC::doAddAction( int& nb ) {
    nb = 0;
    return false;
}

QAction* CameraManipPluginC::getAction( int id ) {
    return nullptr;
}

void CameraManipPluginC::useSelectedCamera() {
    if ( m_selectionManager->hasSelection() )
    {
        const Ra::Engine::ItemEntry& ent = m_selectionManager->currentItem();
        if ( ent.m_component == nullptr ) { return; }
        if ( ent.m_component->getName().compare( 0, 7, "CAMERA_" ) == 0 )
        {
            Ra::Engine::Camera* camera = static_cast<Ra::Engine::Camera*>( ent.m_component );
            m_viewer->getCameraManipulator()->getCamera()->show( true );
            m_viewer->getCameraManipulator()->setCamera( camera );
            emit askForUpdate();
        }
    }
}

void CameraManipPluginC::saveCamera() {
    QSettings settings;
    QString path = settings.value( "CameraManip::camera_file", QDir::homePath() ).toString();
    path         = QFileDialog::getSaveFileName( nullptr, "Open Camera", path, "*.cam" );
    if ( path.size() == 0 ) { return; }
    settings.setValue( "CameraManip::camera_file", path );

    std::ofstream outFile( path.toStdString() );
    if ( !outFile.is_open() )
    {
        LOG( logWARNING ) << "Could not open file to save the camera: " << path.toStdString();
        return;
    }

    auto manip  = m_viewer->getCameraManipulator();
    outFile << "#Radium_camera_manipulator" << std::endl;
    if ( auto trackball = static_cast<Ra::Gui::TrackballCameraManipulator*>( manip ) )
    {
        outFile << "Trackball";
        outFile << trackball->getTrackballCenter();
        outFile << trackball->getTrackballRadius();
    }
    else if ( auto flight = static_cast<Ra::Gui::FlightCameraManipulator*>( manip ) )
    {
        outFile << "Flight";
        // FIXME: should save m_target but cannot access it!
    }
    else
    { LOG( logWARNING ) << "[CameraManip]: Unknown CameraManpulator type."; }
    auto camera = manip->getCamera();
    outFile << "#Radium_camera_state" << std::endl;
    outFile << (int)camera->getType() << std::endl;
    outFile << camera->getFrame().matrix() << std::endl;
    outFile << std::endl;
    outFile << camera->getFOV() << " " << camera->getZNear() << " " << camera->getZFar() << " "
            << camera->getZoomFactor() << " " << camera->getAspect() << std::endl;
    outFile << std::endl;
}

void CameraManipPluginC::createCamera() {
    // Create new entity with camera component only
    auto camMngr =
        static_cast<Ra::Engine::CameraManager*>( m_engine->getSystem( "DefaultCameraManager" ) );
    std::string camName = "CAMERA_" + std::to_string( camMngr->count() );
    auto entity         = m_engine->getEntityManager()->createEntity( camName );
    auto* cam = new Ra::Engine::Camera( entity, camName, m_viewer->width(), m_viewer->height() );
    // Copy Camera data
    auto camera = m_viewer->getCameraManipulator()->getCamera();
    cam->resize( camera->getWidth(), camera->getHeight() );
    cam->setType( camera->getType() );
    cam->setFrame( camera->getFrame() );
    cam->setFOV( camera->getFOV() );
    cam->setZNear( camera->getZNear() );
    cam->setZFar( camera->getZFar() );
    cam->setZoomFactor( camera->getZoomFactor() );
    cam->initialize();
    cam->show( true );
    // Register entity and camera in Camera manager
    camMngr->addCamera( cam );
}

void CameraManipPluginC::keyFrameCamera( bool on ) {
    auto manip = m_viewer->getCameraManipulator();
    auto camera = manip->getCamera();
    if ( on && m_timeline )
    {
        auto cameraPath = new CameraPath(
            camera->getFrame(), 0, Ra::Core::Animation::linearInterpolate<Ra::Core::Transform> );
        m_cameraPaths[camera] = {cameraPath, camera->getFrame()};
        // FIXME: should use different interpolation scheme depending on the CameraManipulator.
        //        see branch https://github.com/hoshiryu/Radium-Engine/tree/camera_path
        m_timeline->registerKeyFramedValue( camera,
                                            "CameraPath",
                                            cameraPath,
                                            [cameraPath, camera, this]( const Scalar& time ) {
                                                cameraPath->insertKeyFrame( time,
                                                                            camera->getFrame() );
                                                m_currentTime = m_timeline->getTime();
                                            },
                                            [cameraPath, camera, this]( const Scalar& time ) {
                                                Scalar t = m_timeline->getTime();
                                                if ( t != m_currentTime )
                                                {
                                                    m_currentTime = t;
                                                    camera->setFrame( cameraPath->at( time ) );
                                                }
                                            } );
        m_timeline->selectionChanged( Ra::Engine::ItemEntry( camera->getEntity(), camera ) );
    }
    else if ( m_cameraPaths.find( camera ) != m_cameraPaths.end() )
    {
        camera->setFrame( m_cameraPaths[camera].second );
        m_timeline->unregisterKeyFramedValue( camera, "CameraPath" );
        delete m_cameraPaths[camera].first;
        m_cameraPaths.erase( camera );
        m_timeline->selectionChanged( Ra::Engine::ItemEntry() );
    }
}

void CameraManipPluginC::onCurrentChanged( const QModelIndex& current, const QModelIndex& prev ) {
    m_widget->ui->m_useCamera->setEnabled( false );
    m_widget->ui->m_keyFrameCamera->blockSignals( true );
    m_widget->ui->m_keyFrameCamera->setChecked( false );
    m_widget->ui->m_keyFrameCamera->blockSignals( false );
    if ( m_selectionManager->hasSelection() )
    {
        const Ra::Engine::ItemEntry& ent = m_selectionManager->currentItem();
        if ( ent.m_component == nullptr ) { return; }
        if ( ent.m_component->getName().compare( 0, 7, "CAMERA_" ) == 0 )
        {
            m_widget->ui->m_useCamera->setEnabled( true );
            m_widget->ui->m_keyFrameCamera->blockSignals( true );
            m_widget->ui->m_keyFrameCamera->setChecked(
                m_cameraPaths.find( static_cast<Ra::Engine::Camera*>( ent.m_component ) ) !=
                m_cameraPaths.end() );
            m_widget->ui->m_keyFrameCamera->blockSignals( false );
        }
    }
}

} // namespace CameraManipPlugin
