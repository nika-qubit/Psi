#include "exif.h"
#include "fs_nas.h"
#include "NAS.h"

#include <memory>
#include <iostream>
#include "absl/base/log_severity.h"
#include "absl/strings/str_join.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/log/globals.h"

namespace {
using nika::nas::Exif;
using nika::nas::FsNAS;
using nika::nas::NAS;
}  // namespace

int main() {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
  //std::unique_ptr<NAS> fs_nas = std::make_unique<FsNAS>();
  //LOG(INFO) << "Fs Devices: " << endl << absl::StrJoin(fs_nas->ListMountedDevices(), "\n");
  //LOG(INFO) << "Compact Devices: " << endl << absl::StrJoin(fs_nas->CompactDevices(), "\n");
  Exif exif;
  LOG(INFO) << "EXIF: "
    <<exif.read(
       // "/media/wonder_land/nas/public/2025/01/PXL_20250101_200924840.jpg"
          "/home/nika/workspace/PXL_20250102_200924840.jpg"
        ).dump();
  return 0;
}

