#include <system.hpp>

#include "FakeToonMaterial.hpp"
#include <Core/Asset/FileData.hpp>
#include <Core/Utils/Index.hpp>
#include <Core/Utils/Log.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/ComponentMessenger.hpp>
#include <Engine/RadiumEngine.hpp>
#include <Engine/Data/Mesh.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>

namespace FakeToonPluginExample {
using namespace Ra::Core::Utils;
MySystem::MySystem() : Ra::Engine::Scene::System() {
    LOG( logINFO ) << "FakeToon Plugin System created.";
}

MySystem::~MySystem() {
    FakeToonMaterial::unregisterMaterial();
    LOG( logINFO ) << "FakeToon Plugin System destroyed.";
}

void MySystem::handleAssetLoading( Ra::Engine::Scene::Entity* entity,
                                   const Ra::Core::Asset::FileData* fileData ) {
    using Ra::Core::Utils::Index;

    auto roMgr = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();

    auto builder = Ra::Engine::Rendering::EngineRenderTechniques::getDefaultTechnique( "FakeToon" );

    if ( builder.first )
    {

        auto data = fileData->getGeometryData();
        for ( auto geom : data )
        {
            if ( Ra::Engine::Scene::ComponentMessenger::getInstance()->canGet<Index>( entity,
                                                                               geom->getName() ) )
            {
                auto idx = *Ra::Engine::Scene::ComponentMessenger::getInstance()->getterCallback<Index>(
                    entity, geom->getName() )();

                if ( idx.isValid() && roMgr->exists( idx ) )
                {
                    auto ro = roMgr->getRenderObject( idx );
                    ro->setMaterial( std::make_shared<FakeToonMaterial>( "FakeToon" ) );
                }
            }
            else
            {
                // This might happen when ShaderFakeToon is loaded before FancyMeshPlugin
                LOG( logWARNING )
                    << "Cannot set FakeToon Material (geometry component not yet created).";
            }
        }
    }
}

void MySystem::generateTasks( Ra::Core::TaskQueue* /*taskQueue*/,
                              const Ra::Engine::FrameInfo& /*frameInfo*/ ) {}

} // namespace FakeToonPluginExample
