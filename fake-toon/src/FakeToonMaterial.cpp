#include "FakeToonMaterial.hpp"

#include <Core/Resources/Resources.hpp>
#include <Core/Utils/Color.hpp>

#include <Engine/Renderer/RenderTechnique/RenderTechnique.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderConfigFactory.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderProgram.hpp>

#include <Core/Utils/Log.hpp>
using namespace Ra::Core::Utils;

namespace FakeToonPluginExample {
static const std::string materialName{"FakeToon"};

FakeToonMaterial::FakeToonMaterial( const std::string& instanceName ) :
    Ra::Engine::Material( instanceName, materialName ) {}

void FakeToonMaterial::updateGL() {
    if ( !m_isDirty ) { return; }

    using namespace Ra::Core::Utils;
    Color m_kd( 0.9, 0.9, 0.9, 1.0 );
    Color m_ks( 0.0, 0.0, 0.0, 1.0 );
    Scalar m_ns( 1.0 );
    Scalar m_alpha( 1.0 );

    m_renderParameters.addParameter( "material.kd", m_kd );
    m_renderParameters.addParameter( "material.ks", m_ks );
    m_renderParameters.addParameter( "material.ns", m_ns );
    m_renderParameters.addParameter( "material.alpha", m_alpha );

    m_isDirty = false;
}

void FakeToonMaterial::registerMaterial() {
    // gets the resource path of the plugins
    Ra::Core::Resources::ResourcesLocator locator(
        reinterpret_cast<void*>( &FakeToonMaterial::registerMaterial ), "" );
    auto pluginPath = locator.getBasePath();
    LOG( logINFO ) << "Plugin Path is " << pluginPath;
    auto shaderPath = pluginPath.substr( 0, pluginPath.find_last_of( '/' ) ) +
                      "/Resources/ShaderFakeToon/Shaders";

    //  // 1- register the Material converter for loading
    //  EngineMaterialConverters::registerMaterialConverter(
    //      "FakeToon",
    //      FakeToonMaterialConverter() );
    // 2- register the technique builder for rendering
    Ra::Engine::ShaderConfiguration lpconfig(
        "FakeToon", shaderPath + "/VERTEX_SHADER.vert", shaderPath + "/FRAGMENT_SHADER.frag" );
    Ra::Engine::ShaderConfigurationFactory::addConfiguration( lpconfig );

    Ra::Engine::ShaderConfiguration dpconfig( "DepthAmbiantFakeToon",
                                              shaderPath + "/VERTEX_SHADER.vert",
                                              shaderPath + "/FRAGMENT_SHADER.frag" );
    Ra::Engine::ShaderConfigurationFactory::addConfiguration( dpconfig );

    Ra::Engine::EngineRenderTechniques::registerDefaultTechnique(
        "FakeToon", []( Ra::Engine::RenderTechnique& rt, bool /*isTransparent*/ ) {
            // Configuration for RenderTechnique::LIGHTING_OPAQUE (Mandatory)
            auto lpconfig = Ra::Engine::ShaderConfigurationFactory::getConfiguration( "FakeToon" );
            rt.setConfiguration( *lpconfig, Ra::Engine::DefaultRenderingPasses::LIGHTING_OPAQUE );
            // Configuration for RenderTechnique::Z_PREPASS
            auto dpconfig =
                Ra::Engine::ShaderConfigurationFactory::getConfiguration( "DepthAmbiantFakeToon" );
            rt.setConfiguration( *dpconfig, Ra::Engine::DefaultRenderingPasses::Z_PREPASS );
            // Configuration for RenderTechnique::LIGHTING_TRANSPARENT
        } );
}
void FakeToonMaterial::unregisterMaterial() {
    Ra::Engine::EngineRenderTechniques::removeDefaultTechnique( "FakeToon" );
}

} // namespace FakeToonPluginExample
