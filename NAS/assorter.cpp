#include "assorter.h"

#include "exif.h"

#include "absl/log/log.h"
#include "absl/strings/string_view.h"

namespace nika::nas {

void Assorter::Do () {
  RelocateStaging("/tmp");
}

void Assorter::RelocateStaging(absl::string_view staging_location) {
  LOG(INFO) << "Relocating staged files from " << staging_location;
};

}  // namespace nika::nas
