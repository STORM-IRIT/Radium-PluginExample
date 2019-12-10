#ifndef FANCYMESHPLUGIN_FANCYMESHCOMPONENT_HPP
#define FANCYMESHPLUGIN_FANCYMESHCOMPONENT_HPP

#include <DummyPluginMacros.hpp>

#include <Engine/Component/Component.hpp>

namespace Ra {
namespace Engine {
struct RenderTechnique;
}
} // namespace Ra

namespace DummyPlugin {
class DUMMY_PLUGIN_API DummyComponent : public Ra::Engine::Component
{
  public:
    DummyComponent( const std::string& name, Ra::Engine::Entity* entity );
    virtual ~DummyComponent();

    virtual void initialize() override;

  private:
};

} // namespace DummyPlugin

#endif // FANCYMESHPLUGIN_FANCYMESHCOMPONENT_HPP
