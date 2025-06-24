#pragma once

#include "NAS.h"

#include <string>
#include <vector>

namespace nika::nas {

class FstabNAS final : public NAS {
public:
	std::vector<MountedDevice> ListMountedDevices() const override;
};

}  // namespace nika::nas
