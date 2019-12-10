#include "DummyComponent.hpp"

#include <Engine/Renderer/Mesh/Mesh.hpp>
#include <Engine/Renderer/RenderObject/Primitives/DrawPrimitives.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>
#include <Engine/Renderer/RenderTechnique/RenderTechnique.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderConfiguration.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderProgramManager.hpp>

namespace DummyPlugin {
DummyComponent::DummyComponent( const std::string& name, Ra::Engine::Entity* entity ) : Ra::Engine::Component( name, entity ) {}

DummyComponent::~DummyComponent() {}

void DummyComponent::initialize() {}

} // namespace DummyPlugin
