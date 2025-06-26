#pragma once

#include "conf.h"
#include "task_executor.h"
#include "exif.h"

#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"

namespace nika::nas {

class Assorter : public MutexTask {
 public:
  void Do() override;
  void RelocateStaging(absl::string_view staging_location);

 private:
  Exif exif_;
  std::string assort_root_ = absl::StrCat(kRootDir, "/wonder_land", kSambaPath);
};

}  // namespace nika::nas
