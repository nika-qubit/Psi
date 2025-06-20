#pragma once

#include "task_executor.h"
#include "exif.h"

#include "absl/strings/string_view.h"

namespace nika::nas {

class Assorter : public MutexTask {
 public:
  void Do() override;
  void RelocateStaging(absl::string_view staging_location);

 private:
  Exif exif_;
};

}  // namespace nika::nas
