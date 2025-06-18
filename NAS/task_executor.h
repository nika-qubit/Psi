#pragma once

#include <vector>
#include <memory>

namespace nika::nas {

class MutexTask {
 public:
  virtual void Do() = 0;
  virtual ~MutexTask() = default;
};

class TaskExecutor {
 public:
  void Append(std::unique_ptr<MutexTask> task);
  void Start();
  bool IsBusy();

 private:
  bool busy_ = false;
  std::vector<std::unique_ptr<MutexTask>> tasks_;
};

}  // namespace nika::nas
