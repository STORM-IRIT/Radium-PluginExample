#include "FakeToonMaterial.hpp"

#include <Core/Math/LinearAlgebra.hpp>
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
    //  // 1- register the Material converter for loading
    //  EngineMaterialConverters::registerMaterialConverter(
    //      "FakeToon",
    //      FakeToonMaterialConverter() );
    // 2- register the technique builder for rendering
    Ra::Engine::EngineRenderTechniques::registerDefaultTechnique(
        "FakeToon", []( Ra::Engine::RenderTechnique& rt, bool /*isTransparent*/ ) {
            // Configuration for RenderTechnique::LIGHTING_OPAQUE (Mandatory)
            Ra::Engine::ShaderConfiguration lpconfig(
                "FakeToon",
                "Plugins/FakeToon/Shaders/VERTEX_SHADER.vert",
                "Plugins/FakeToon/Shaders/FRAGMENT_SHADER.frag" );
            Ra::Engine::ShaderConfigurationFactory::addConfiguration( lpconfig );
            rt.setConfiguration( lpconfig, Ra::Engine::RenderTechnique::LIGHTING_OPAQUE );
            // Configuration for RenderTechnique::Z_PREPASS
            Ra::Engine::ShaderConfiguration dpconfig(
                "DepthAmbiantFakeToon",
                "Plugins/FakeToon/Shaders/VERTEX_SHADER.vert",
                "Plugins/FakeToon/Shaders/FRAGMENT_SHADER.frag" );
            Ra::Engine::ShaderConfigurationFactory::addConfiguration( dpconfig );
            rt.setConfiguration( dpconfig, Ra::Engine::RenderTechnique::Z_PREPASS );
            // Configuration for RenderTechnique::LIGHTING_TRANSPARENT
        } );
}
void FakeToonMaterial::unregisterMaterial() {
    //  Ra::Engine::EngineMaterialConverters::removeMaterialConverter( "FakeToon" );
    Ra::Engine::EngineRenderTechniques::removeDefaultTechnique( "FakeToon" );
}

} // namespace FakeToonPluginExample
