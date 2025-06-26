#include "assorter.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

#include "conf.h"
#include "exif.h"

#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"
#include <nlohmann/json.hpp>

namespace nika::nas {

namespace {

using nlohmann::json;
using ofs = std::ofstream;

namespace fs = std::filesystem;

void MoveFile(absl::string_view filename, absl::string_view file_path, absl::string_view assort_root, const Metadata& metadata) {
  if (!metadata.is_comprehensive) return;
  std::string dest = absl::StrCat(assort_root, "/", metadata.year, "/", metadata.month, "/", filename);
  VLOG(1) << "Moving staging file: " << file_path << " to: " << dest;
  try {
    fs::rename(file_path, dest);
  } catch (fs::filesystem_error const& ex) {
    LOG(ERROR) << "Failed to move staging file: " << file_path << "to: " << dest;
  }
}

// Utility to check file type.
void FileType(fs::file_type type) {
  switch (type) {
    case fs::file_type::none:
        LOG(WARNING) << " has `not-evaluated-yet` type";
        break;
    case fs::file_type::not_found:
        LOG(WARNING) << " does not exist";
        break;
    case fs::file_type::regular:
        LOG(WARNING) << " is a regular file";
        break;
    case fs::file_type::directory:
        LOG(WARNING) << " is a directory";
        break;
    case fs::file_type::symlink:
        LOG(WARNING) << " is a symlink";
        break;
    case fs::file_type::block:
        LOG(WARNING) << " is a block device";
        break;
    case fs::file_type::character:
        LOG(WARNING) << " is a character device";
        break;
    case fs::file_type::fifo:
        LOG(WARNING) << " is a named IPC pipe";
        break;
    case fs::file_type::socket:
        LOG(WARNING) << " is a named IPC socket";
        break;
    case fs::file_type::unknown:
        LOG(WARNING) << " has `unknown` type";
        break;
    default:
        LOG(WARNING) << " has `implementation-defined` type";
        break;
  }
}

}  // namespace

void Assorter::Do() {
  RelocateStaging(absl::StrCat(assort_root_, kStaging));
}

void Assorter::RelocateStaging(absl::string_view staging_location) {
  LOG(INFO) << "Relocating staged files from " << staging_location;
  for (const auto& entry : fs::recursive_directory_iterator(staging_location)) {
    VLOG(1) << entry.path().string();
    if (fs::is_regular_file(entry)) {
      std::string file_path = entry.path().string();
      json json_meta = exif_.Read(file_path);
      const Metadata metadata = exif_.Distill(json_meta);
      MoveFile(entry.path().stem().string(), file_path, assort_root_, metadata);
    }
  }
  LOG(INFO) << "Completed relocating from " << staging_location;
}

}  // namespace nika::nas
