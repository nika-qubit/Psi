#include "fs_nas.h"

#include <filesystem>

#include "absl/strings/string_view.h"
#include "absl/log/log.h"

namespace nika::nas {

namespace {

namespace fs = std::filesystem;

constexpr absl::string_view kRootDir = "/media";

}  // namespace

std::vector<std::string> FsNAS::ListMountedDevices() const {
  std::vector<std::string> devices;
  for (const auto& entry : fs::directory_iterator(kRootDir)) {
    if (fs::is_directory(entry)) {
      devices.push_back(entry.path());
    } else if (fs::is_regular_file(entry)) {
      LOG(INFO) << "Skipping regular file: " << entry.path();
    }
  }
  return devices;
}

}  // namespace nika::nas
