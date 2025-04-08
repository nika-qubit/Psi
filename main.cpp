#include "NAS.h"

#include <iostream>
#include "absl/strings/str_join.h"

namespace {
using std::cout;
using std::endl;
using nika::nas::NAS;
}  // namespace

int main()
{
  NAS nas;
	cout << "Hello CMake." << endl;
  cout << "Devices: " << endl << absl::StrJoin(nas.ListMountedDevices(), "\n") << endl;
	return 0;
}

