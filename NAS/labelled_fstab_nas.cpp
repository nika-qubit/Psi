#include "labelled_fstab_nas.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace nika::nas {

std::vector<MountedDevice> FstabNAS::ListMountedDevices() const {
  std::ifstream mounts_file("/etc/fstab");
  
  if (!mounts_file.is_open()) {
    std::cerr << "Error opening." << std::endl;
  }
  std::string line;
  std::vector<MountedDevice> mounted_devices;
  while (std::getline(mounts_file, line)) {
    mounted_devices.push_back({line, line});
  }
  return mounted_devices;
}

}  // namespace nika::nas

