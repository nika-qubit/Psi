// NAS.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace nika::nas {

struct MountedDevice {
  std::string device_name;
  std::string mount_path;
};

inline std::ostream& operator<< (std::ostream& os, const MountedDevice& val) {
  os << "device_name: " << val.device_name
     << "mount_path: " << val.mount_path;
  return os;
}

class NAS {
public:
  virtual std::vector<MountedDevice> ListMountedDevices() const = 0;
  virtual std::vector<std::string> CompactDevices() { return {"Not implemented."}; }

  NAS() = default;
  virtual ~NAS() = default;
};

}  // namespace nika::nas
