#pragma once

#include "NAS.h"

#include <string>
#include <vector>

#include "absl/strings/string_view.h"

namespace nika::nas {

class FsNAS final : public NAS {
public:
  std::vector<std::string> ListMountedDevices() const override;
  std::vector<std::string> CompactDevices() override;

private:
  std::vector<std::string> CompactDevice(absl::string_view device);
  std::vector<std::string> CompactYear(absl::string_view year, std::string compaction_path);
  std::vector<std::string> CompactMonth(absl::string_view month, std::string compaction_path);
};

}  // namespace nika::nas
