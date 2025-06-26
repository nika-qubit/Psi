#include "fs_nas.h"

#include <iostream>
#include <utility>
#include <filesystem>
#include <system_error>
#include <fstream>

#include "conf.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "absl/log/log.h"
#include "absl/time/time.h"
#include "absl/strings/escaping.h"

namespace nika::nas {

namespace {

using std::endl;
using ofs = std::ofstream;
using ifs = std::ifstream;

namespace fs = std::filesystem;

absl::flat_hash_map<std::string, ofs> manifests;

struct MovedFile {
  std::string filename;
  std::string file_path;
  std::string first_seen_path;
};

void RecordInManifest(std::string manifest_path, const MovedFile& moved) {
  if (
      (!manifests.contains(manifest_path)) ||
      (!manifests[manifest_path].is_open())) {
    manifests[manifest_path] = ofs{manifest_path, std::ios::app};
  }
  auto& manifest = manifests[manifest_path];
  if (manifest.is_open()) {
    manifest << moved.filename << "," << moved.file_path << "," << moved.first_seen_path << endl;
  } else {
    LOG(WARNING) << "Didn't write manifest " << manifest_path << ", moved file was " << moved.file_path << endl;
  }
}

void FlushAndCloseManifest(std::string manifest_path) {
  manifests[manifest_path].flush();
  manifests[manifest_path].close();
}

std::string BuildCompactionPath(MountedDevice device) {
  std::string compaction_path = absl::StrCat(
      device.mount_path, "/compaction-", device.device_name, absl::Base64Escape(
        absl::FormatTime("%Y-%m-%d %H:%M:%S", absl::Now(), absl::LocalTimeZone())));
  compaction_path = compaction_path.substr(0, compaction_path.length() - 2);
  try {
    fs::create_directory(compaction_path);
  } catch (fs::filesystem_error const& ex) {
    LOG(ERROR) << "Failed to create compaction path " << compaction_path << ", code: " << ex.code().message();
  }
  return compaction_path;
}

std::string BuildManifestPath(absl::string_view compaction_path) {
  return absl::StrCat(compaction_path, "/manifest.csv");
}

void MoveFileToCompaction(const MovedFile& moved, std::string compaction_path) {
  try {
    fs::rename(moved.file_path, absl::StrCat(compaction_path, "/", absl::ToUnixMicros(absl::Now()), moved.filename));
  } catch (fs::filesystem_error const& ex) {
    LOG(ERROR) << "Failed to compact duplicated file" << moved.file_path << ", code: " << ex.code().message();
  }
}

}  // namespace

void FsNAS::Do() {
  CompactDevices();
}

std::vector<MountedDevice> FsNAS::ListMountedDevices() const {
  std::vector<MountedDevice> devices;
  for (const auto& entry : fs::directory_iterator(kRootDir)) {
    if (fs::is_directory(entry)) {
      devices.push_back({
        .device_name = entry.path().stem().string(),
        .mount_path = absl::StrCat(entry.path().string(), kSambaPath)
      });
    } else if (fs::is_regular_file(entry)) {
      LOG(INFO) << "Skipping regular file: " << entry.path();
    }
  }
  return devices;
}

std::vector<std::string> FsNAS::CompactDevices() {
  std::vector<MountedDevice> mounted_devices = ListMountedDevices();
  // Dir layers: root --> year --> month --> plain file or event dir.
  std::vector<std::string> compact_found;
  for (const auto& device : mounted_devices) {
    std::vector<std::string> found = CompactDevice(device);
    compact_found.insert(compact_found.end(), found.begin(), found.end());
  }
  return compact_found;
}

std::vector<std::string> FsNAS::CompactDevice(const MountedDevice& device) {
  LOG(INFO) << "Device: " << device.device_name << ", path: " << device.mount_path;
  std::string compaction_path = BuildCompactionPath(device);
  // Compactable for the current device.
  std::vector<std::string> device_found;
  for (const auto& entry : fs::directory_iterator(device.mount_path)) {
    LOG(INFO) << entry.path().stem();
    if (fs::is_directory(entry) && RE2::FullMatch(entry.path().stem().string(), *kYearDirMatcher)) {
      std::vector<std::string> found = CompactYear(entry.path().string(), compaction_path);
      device_found.insert(device_found.end(), found.begin(), found.end());
    }
  }
  LOG(INFO) << "End Device: " << device;
  if (device_found.empty()) {
    fs::remove(compaction_path);
  }
  return device_found;
}

std::vector<std::string> FsNAS::CompactYear(absl::string_view year, std::string compaction_path) {
  LOG(INFO) << "Year: " << year;
  std::vector<std::string> year_found;
  for (const auto& entry: fs::directory_iterator(year)) {
    LOG(INFO) << entry.path().stem();
    if (fs::is_directory(entry) && RE2::FullMatch(entry.path().stem().string(), *kMonthDirMatcher)) {
      std::vector<std::string> found = CompactMonth(entry.path().string(), compaction_path);
      year_found.insert(year_found.end(), found.begin(), found.end());
    }
  }
  LOG(INFO) << "End Year: " << year;
  return year_found;
}

std::vector<std::string> FsNAS::CompactMonth(absl::string_view month, std::string compaction_path) {
  LOG(INFO) << "Month: " << month;
  std::vector<std::string> month_found;
  absl::flat_hash_set<std::string> seen;
  absl::flat_hash_map<std::string, std::string> seen_map;
  std::vector<std::string> events;
  std::string manifest_path = BuildManifestPath(compaction_path);
  for (const auto& entry: fs::directory_iterator(month)) {
    if (fs::is_directory(entry)) {  // event
      events.push_back(entry.path());
    }
  }
  for (const auto& event: events) {
    for (const auto& entry: fs::recursive_directory_iterator(event)) {
      if (fs::is_regular_file(entry)) {
        std::string filename = entry.path().filename().string();
        VLOG(1) << "Seen " << filename << " at " << entry.path();
        if (!seen.insert(filename).second) {
          MovedFile moved = {filename, entry.path().string(), seen_map[filename]};
          LOG(WARNING) << "Duplicated " << filename << " at " << entry.path() << ", first seen in " << seen_map[filename];
          RecordInManifest(manifest_path, moved);
          MoveFileToCompaction(moved, compaction_path);
          month_found.push_back(entry.path());
        } else {
          seen_map[filename] = entry.path().string();
        }
      }
    }
  }
  for (const auto& entry : fs::directory_iterator(month)) {
    if (fs::is_regular_file(entry)) {  // plain files
      std::string filename = entry.path().filename().string();
      VLOG(1) << "Seen " << filename << " at " << entry.path();
      if (!seen.insert(filename).second) {
        MovedFile moved = {filename, entry.path().string(), seen_map[filename]};
        LOG(WARNING) << "Duplicated " << filename << " at " << entry.path() << ", first seen in " << seen_map[filename];
        RecordInManifest(manifest_path, moved);
        MoveFileToCompaction(moved, compaction_path);
        month_found.push_back(entry.path());
      } else {
        seen_map[filename] = entry.path().string();
      }
    }
  }
  FlushAndCloseManifest(manifest_path);
  LOG(INFO) << "End Month: " << month;
  return month_found;
}

}  // namespace nika::nas
