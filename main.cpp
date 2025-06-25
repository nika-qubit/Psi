#include "assorter.h"
#include "exif.h"
#include "fs_nas.h"
#include "NAS.h"

#include <iostream>
#include <memory>
#include <utility>

#include "absl/base/log_severity.h"
#include "absl/strings/string_view.h"
#include "absl/strings/str_join.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/log/globals.h"

namespace {
using nika::nas::Assorter;
using nika::nas::Exif;
using nika::nas::FsNAS;
using nika::nas::Metadata;
using nika::nas::MutexTask;
using nika::nas::NAS;
using nika::nas::TaskExecutor;

constexpr absl::string_view kModuleExif = "exif";
}  // namespace

int main(int argc, char* argv[]) {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
  absl::SetGlobalVLogLevel(1);
  absl::SetVLogLevel(kModuleExif, 1);
  std::unique_ptr<FsNAS> fs_nas = std::make_unique<FsNAS>();
  std::unique_ptr<Assorter> assorter = std::make_unique<Assorter>();
  TaskExecutor executor;
  // executor.Append(std::move(fs_nas));
  executor.Append(std::move(assorter));
  executor.Start();
  // Exif exif;
  // auto json_metadata = exif.Read(
  //     // "/media/wonder_land/nas/public/2025/01/PXL_20250101_200924840.jpg"
  //     //   "/home/nika/workspace/PXL_20250102_200924840.jpg"
  //     argv[1]
  // );
  // LOG(INFO) << "EXIF: " <<json_metadata.dump();
  // Metadata metadata = exif.Distill(json_metadata);
  // LOG(INFO) << "EXIF metadata: " << metadata;
  return 0;
}

