#include "assorter.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "conf.h"
#include "exif.h"

#include "absl/log/log.h"
#include "absl/strings/string_view.h"
#include <nlohmann/json.hpp>

namespace nika::nas {

namespace {

using nlohmann::json;
using ofs = std::ofstream;

namespace fs = std::filesystem;

void MoveFile(absl::string_view file, const Metadata& metadata) {
  if (!metadata.is_comprehensive) return;
  LOG(INFO) << "Moving staging file: " << file << " with metadata: " << metadata;
}

}  // namespace

void Assorter::Do() {
  RelocateStaging(kStaging);
}

void Assorter::RelocateStaging(absl::string_view staging_location) {
  LOG(INFO) << "Relocating staged files from " << staging_location;
  for (const auto& entry : fs::recursive_directory_iterator(staging_location)) {
    LOG(INFO) << entry.path().string();
    if (fs::is_regular_file(entry)) {
      std::string file_path = entry.path().string();
      json json_meta = exif_.Read(file_path);
      const Metadata metadata = exif_.Distill(json_meta);
      MoveFile(file_path, metadata);
    }
  }
  LOG(INFO) << "Completed relocating from " << staging_location;
}

}  // namespace nika::nas
