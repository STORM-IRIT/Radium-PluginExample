#include <CameraManipPlugin.hpp>

#include <fstream>

#include <QAction>
#include <QFileDialog>
#include <QSettings>

#include <Engine/Scene/ItemEntry.hpp>
#include <Engine/Scene/CameraManager.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/Camera.hpp>

#include <Gui/SelectionManager/SelectionManager.hpp>
#include <Gui/Viewer/TrackballCameraManipulator.hpp>
#include <Gui/Viewer/Viewer.hpp>

#include "ui_CameraManipUI.h"
#include <UI/CameraManipUI.hpp>

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
    connect( m_selectionManager,
             &Ra::Gui::SelectionManager::currentChanged,
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
    connect(
        m_widget->ui->m_saveCamera, &QPushButton::clicked, this, &CameraManipPluginC::saveCamera );
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
        const Ra::Engine::Scene::ItemEntry& ent = m_selectionManager->currentItem();
        if ( ent.m_component == nullptr ) { return; }
        if ( ent.m_component->getName().compare( 0, 7, "CAMERA_" ) == 0 )
        {
            Ra::Engine::Scene::Camera* camera = static_cast<Ra::Engine::Scene::Camera*>( ent.m_component );
            m_viewer->getCameraManipulator()->getCamera()->show(true );
            m_viewer->getCameraManipulator()->setCamera(camera );
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

    auto manip  = static_cast<Ra::Gui::TrackballCameraManipulator*>( m_viewer->getCameraManipulator() );
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
        static_cast<Ra::Engine::Scene::CameraManager*>( m_engine->getSystem( "DefaultCameraManager" ) );
    std::string camName = "CAMERA_" + std::to_string( camMngr->count() );
    auto entity         = m_engine->getEntityManager()->createEntity( camName );
    Ra::Engine::Scene::Camera* cam =
        new Ra::Engine::Scene::Camera( entity, camName, m_viewer->width(), m_viewer->height() );
    // Copy Camera data
    auto manip  = static_cast<Ra::Gui::TrackballCameraManipulator*>( m_viewer->getCameraManipulator() );
    auto camera = manip->getCamera();
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

void CameraManipPluginC::onCurrentChanged( const QModelIndex& current, const QModelIndex& prev ) {
    m_widget->ui->m_useCamera->setEnabled( false );
    if ( m_selectionManager->hasSelection() )
    {
        const Ra::Engine::Scene::ItemEntry& ent = m_selectionManager->currentItem();
        if ( ent.m_component == nullptr ) { return; }
        if ( ent.m_component->getName().compare( 0, 7, "CAMERA_" ) == 0 )
        { m_widget->ui->m_useCamera->setEnabled( true ); }
    }
}

} // namespace CameraManipPlugin
