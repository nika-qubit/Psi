// NAS.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <string>
#include <vector>

namespace nika::nas {

class NAS {
public:
  virtual std::vector<std::string> ListMountedDevices() const = 0;

  NAS() = default;
  virtual ~NAS() = default;
};

}  // namespace nika::nas
