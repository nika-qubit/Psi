#include "fs_nas.h"

namespace nika::nas {

std::vector<std::string> FsNAS::ListMountedDevices() const {
  return {"ok"};
}

}  // namespace nika::nas
