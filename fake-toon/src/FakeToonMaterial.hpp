#pragma once

#include "plugin.hpp"
#include <Engine/Renderer/Material/Material.hpp>

namespace FakeToonPluginExample {

/// Simple shader class implementing basic toon shader
class FAKE_TOON_PLUGIN_EXAMPLE_API FakeToonMaterial final : public Ra::Engine::Material
{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    explicit FakeToonMaterial( const std::string& instanceName );

    void updateGL() override {}
    void bind( const Ra::Engine::ShaderProgram* shader ) override;

  private:
  public:
    static void registerMaterial();
    static void unregisterMaterial();
};
} // namespace FakeToonPluginExample
