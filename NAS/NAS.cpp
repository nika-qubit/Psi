#include "NAS.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace nika::nas {

std::vector<std::string> NAS::ListMountedDevices() {
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
                                        jajaah

}  // namespace nika::nas

