#include "task_executor.h"

#include <memory>
#include <utility>

namespace nika::nas {

void TaskExecutor::Append(std::unique_ptr<MutexTask> task) {
  tasks_.push_back(std::move(task));
}

void TaskExecutor::Start() {
  busy_ = true;
  for (const auto& task : tasks_) {
    task->Do();
  }
}

bool TaskExecutor::IsBusy() {
  return busy_;
}

}  // namespace nika::nas
