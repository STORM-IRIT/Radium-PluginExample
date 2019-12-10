#ifndef DUMMYPLUGIN_DUMMYTASK_HPP
#define DUMMYPLUGIN_DUMMYTASK_HPP

#include <Core/Tasks/Task.hpp>

namespace DummyPlugin {
struct DummyData;

struct DummyParams {
    DummyData* data;
};

class DummyTask : public Ra::Core::Task
{
  public:
    DummyTask() = default;
    std::string getName() const override;
    void process() override;
    void init( const DummyParams* params );

  private:
    DummyData* m_data{nullptr};
};

class DummyOtherTask : public Ra::Core::Task
{
  public:
    DummyOtherTask() = default;
    std::string getName() const override;
    void process() override;
    void init( const DummyParams* params );

  private:
    DummyData* m_data{nullptr};
};
} // namespace DummyPlugin

#endif
