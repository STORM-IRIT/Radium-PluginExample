#include "FakeToonMaterial.hpp"

#include <Core/Resources/Resources.hpp>
#include <Core/Utils/Color.hpp>

#include <Engine/Renderer/RenderTechnique/RenderTechnique.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderConfigFactory.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderProgram.hpp>

namespace FakeToonPluginExample {
static const std::string materialName{"FakeToon"};

FakeToonMaterial::FakeToonMaterial( const std::string& instanceName ) :
    Ra::Engine::Material( instanceName, materialName ) {}

void FakeToonMaterial::bind( const Ra::Engine::ShaderProgram* shader ) {
    if ( !m_isDirty ) { return; }

    using namespace Ra::Core::Utils;
    Color m_kd( 0.9, 0.9, 0.9, 1.0 );
    Color m_ks( 0.0, 0.0, 0.0, 1.0 );
    Scalar m_ns( 1.0 );
    Scalar m_alpha( 1.0 );

    shader->setUniform( "material.kd", m_kd );
    shader->setUniform( "material.ks", m_ks );
    shader->setUniform( "material.ns", m_ns );
    shader->setUniform( "material.alpha", m_alpha );

    m_isDirty = false;
}

void FakeToonMaterial::registerMaterial() {
    // gets the base path for accessing plugins
    auto pluginPath = Ra::Core::Resources::getPluginsDir();
    // Compute the path of the shader :
    auto shaderPath = pluginPath.substr( 0, pluginPath.find_last_of( '/' ) ); // .../Plugins/lib
    shaderPath      = shaderPath.substr( 0, shaderPath.find_last_of( '/' ) ) +
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
            rt.setConfiguration( lpconfig, Ra::Engine::RenderTechnique::LIGHTING_OPAQUE );
            // Configuration for RenderTechnique::Z_PREPASS
            auto dpconfig =
                Ra::Engine::ShaderConfigurationFactory::getConfiguration( "DepthAmbiantFakeToon" );
            rt.setConfiguration( dpconfig, Ra::Engine::RenderTechnique::Z_PREPASS );
            // Configuration for RenderTechnique::LIGHTING_TRANSPARENT
        } );
}
void FakeToonMaterial::unregisterMaterial() {
    Ra::Engine::EngineRenderTechniques::removeDefaultTechnique( "FakeToon" );
}

} // namespace FakeToonPluginExample
