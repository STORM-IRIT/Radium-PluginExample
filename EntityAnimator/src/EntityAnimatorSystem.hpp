#ifndef ENTITYANIMATORSYSTEM_HPP
#define ENTITYANIMATORSYSTEM_HPP

#include <EntityAnimatorPluginMacros.hpp>

#include <Core/Animation/KeyFramedValue.hpp>
#include <Core/Animation/KeyFramedValueInterpolators.hpp>

#include <Engine/Entity/Entity.hpp>
#include <Engine/System/System.hpp>

#include <GuiBase/Timeline/Timeline.h>

namespace EntityAnimatorPlugin {

class ENTITY_ANIMATOR_PLUGIN_API EntityAnimatorSystem : public Ra::Engine::System
{
  public:
    EntityAnimatorSystem() {}
    ~EntityAnimatorSystem() override {}

    void setTimeline( Ra::GuiBase::Timeline* timeline ) {
        m_timeline = timeline;
    }

    void handleAssetLoading( Ra::Engine::Entity* entity,
                             const Ra::Core::Asset::FileData* data ) override {
        if ( !m_timeline ) return;
        using namespace Ra::Core::Animation;
        using Ra::Core::Transform;
        auto keyframe = new KeyFramedValue<Transform>( entity->getTransform(), 0_ra, linearInterpolate<Transform> );
        auto inserter = [keyframe, entity]( const Scalar& t ) {
            keyframe->insertKeyFrame( t, entity->getTransform() );
        };
        auto updater = [keyframe, entity]( const Scalar& t ) {
            entity->setTransform( keyframe->at( t ) );
        };
        m_timeline->registerKeyFramedValue( entity, "Entity Transform", keyframe, inserter, updater );
    }

    void generateTasks( Ra::Core::TaskQueue* taskQueue,
                        const Ra::Engine::FrameInfo& frameInfo ) override {}

  private:
    Ra::GuiBase::Timeline* m_timeline;
};

} // namespace EntityAnimatorPlugin

#endif // ENTITYANIMATORSYSTEM_HPP
