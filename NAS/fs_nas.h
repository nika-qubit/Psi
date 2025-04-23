#pragma once

#include "NAS.h"

#include <string>
#include <vector>

namespace nika::nas {

class FsNAS final : public NAS {
public:
  std::vector<std::string> ListMountedDevices() const override;
  std::vector<std::string> CompactDevices() const override;
};

}  // namespace nika::nas
