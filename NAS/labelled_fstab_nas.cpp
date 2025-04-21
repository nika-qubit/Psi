#include "NAS.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "labelled_fstab_nas.h"

namespace nika::nas {

std::vector<std::string> FstabNAS::ListMountedDevices() const {
  std::ifstream mounts_file("/etc/fstab");
  
  if (!mounts_file.is_open()) {
    std::cerr << "Error opening." << std::endl;
  }
  std::string line;
  std::vector<std::string> mounted_devices;
  while (std::getline(mounts_file, line)) {
    mounted_devices.push_back(line);
  }
  return mounted_devices;
}

}  // namespace nika::nas

