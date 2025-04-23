#include "NAS.h"
#include "labelled_fstab_nas.h"
#include "fs_nas.h"

#include <memory>
#include <iostream>
#include "absl/base/log_severity.h"
#include "absl/strings/str_join.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/log/globals.h"

namespace {
using std::cout;
using std::endl;
using nika::nas::NAS;
using nika::nas::FsNAS;
using nika::nas::FstabNAS;
}  // namespace

int main() {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
  std::unique_ptr<NAS> fs_nas = std::make_unique<FsNAS>();
  LOG(INFO) << "Fs Devices: " << endl << absl::StrJoin(fs_nas->ListMountedDevices(), "\n") << endl;
  LOG(INFO) << "Compact Devices: " << endl << absl::StrJoin(fs_nas->CompactDevices(), "\n") << endl;
  return 0;
}

