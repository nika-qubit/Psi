#include "NAS.h"
#include "labelled_fstab_nas.h"

#include <memory>
#include <iostream>
#include "absl/strings/str_join.h"

namespace {
using std::cout;
using std::endl;
using nika::nas::NAS;
using nika::nas::FstabNAS;
}  // namespace

int main() {
  std::unique_ptr<NAS> nas = std::make_unique<FstabNAS>();
  cout << "Devices: " << endl << absl::StrJoin(nas->ListMountedDevices(), "\n") << endl;
  return 0;
}

