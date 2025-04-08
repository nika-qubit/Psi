#include "NAS.h"

#include <string>
#include <vector>

namespace nika::nas {

std::vector<std::string> NAS::ListMountedDevices() {
  return {"test_a", "test_b"};
}


}  // namespace nika::nas

