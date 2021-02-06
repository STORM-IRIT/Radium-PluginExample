#include "DummyComponent.hpp"

#include <Engine/Data/Mesh.hpp>
#include <Engine/Data/DrawPrimitives.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Rendering/RenderTechnique.hpp>
#include <Engine/Data/ShaderConfiguration.hpp>
#include <Engine/Data/ShaderProgramManager.hpp>

namespace DummyPlugin {
DummyComponent::DummyComponent( const std::string& name, Ra::Engine::Scene::Entity* entity ) : Ra::Engine::Scene::Component( name, entity ) {}

DummyComponent::~DummyComponent() {}

void DummyComponent::initialize() {}

} // namespace DummyPlugin
