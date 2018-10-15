#include <system.hpp>

#include <Core/Log/Log.hpp>
#include <Core/File/FileData.hpp>
#include <Core/File/GeometryData.hpp>
#include <Engine/Managers/ComponentMessenger/ComponentMessenger.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>
#include <Engine/Renderer/RenderObject/RenderObject.hpp>
#include <Engine/Renderer/Mesh/Mesh.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/RadiumEngine.hpp>
#include "FakeToonMaterial.hpp"

namespace FakeToonPluginExample
{

    MySystem::MySystem()
        : Ra::Engine::System()
    {
        LOG(logINFO) << "FakeToon Plugin System created.";
    }

    MySystem::~MySystem()
    {
        LOG(logINFO) << "FakeToon Plugin System destroyed.";
    }

    void MySystem::handleAssetLoading( Ra::Engine::Entity* entity,
                                       const Ra::Asset::FileData* fileData )
    {

      auto roMgr = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();

      auto builder = Ra::Engine::EngineRenderTechniques::getDefaultTechnique( "FakeToon" );

      if (builder.first) {

          auto data = fileData->getGeometryData();
          for (auto geom : data){
              if ( Ra::Engine::ComponentMessenger::getInstance()->canGet <Ra::Core::Index>( entity, geom->getName()) ) {
                  auto idx = * Ra::Engine::ComponentMessenger::getInstance()->getterCallback<Ra::Core::Index>( entity, geom->getName())();

                  if(idx.isValid() && roMgr->exists( idx ) ){
                      auto rt = std::make_shared<Ra::Engine::RenderTechnique>();
                      builder.second( *rt, false );
                      rt->setMaterial( std::make_shared<FakeToonMaterial>("FakeToon") );

                      auto ro = roMgr->getRenderObject( idx );
                      ro->setRenderTechnique( rt );
                  }
              }
              else
              {
                  // This might happen when ShaderFakeToon is loaded before FancyMeshPlugin
                  LOG(logWARNING) << "Cannot set FakeToon Material (geometry component not yet created).";
              }
          }
      }

    }

    void MySystem::generateTasks( Ra::Core::TaskQueue* /*taskQueue*/,
                                  const Ra::Engine::FrameInfo& /*frameInfo*/ )
    {
    }

} // namespace MyPlugin
