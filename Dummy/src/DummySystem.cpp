#include "DummySystem.hpp"

#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/FrameInfo.hpp>
#include <Engine/RadiumEngine.hpp>
#include <Engine/Renderer/RenderTechnique/RenderTechnique.hpp>

#include "DummyComponent.hpp"
#include "DummyTask.hpp"

namespace DummyPlugin {

DummySystem::DummySystem() : Ra::Engine::System() {
    m_data = new DummyData;
    m_data->foo = 42;
    m_data->bar = 1337;
}

DummySystem::~DummySystem() {
    delete m_data;
}

void DummySystem::handleAssetLoading( Ra::Engine::Entity *entity, const Ra::Core::Asset::FileData *data ) {
    std::string componentName = "DummyComponent_" + entity->getName();
    DummyComponent* component = new DummyComponent( componentName, entity );
    registerComponent( entity, component );
}

void DummySystem::generateTasks( Ra::Core::TaskQueue* taskQueue,
                                 const Ra::Engine::FrameInfo& /*frameInfo*/ ) {
    DummyTask* task1      = new DummyTask;
    DummyOtherTask* task2 = new DummyOtherTask;

    DummyParams p;
    p.data = m_data;

    task1->init( &p );
    task2->init( &p );

    auto task2Id = taskQueue->registerTask( task2 );
    auto task1Id = taskQueue->registerTask( task1 );

    taskQueue->addDependency( task1Id, task2Id );
}

} // namespace DummyPlugin
