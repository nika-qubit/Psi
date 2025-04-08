// NAS.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <string>
#include <vector>

namespace nika::nas {

class NAS {
 public:
  std::vector<std::string> ListMountedDevices();
};

}  // namespace nika::nas
